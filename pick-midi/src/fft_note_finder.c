/* 2014 (C) Davide Berardi, Matteo Martelli */
/* A spectrum analizer with fftw3 */
/* Released under GPLv2 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>

#include <complex.h>
#include <fftw3.h>

#define SAMPLE_RATE 44100
#define BASE_RATE 	44100
#define REPETITION	1
#define SAMPLE_OCTAVE_HARDLIMIT 8
typedef char byte_t;

struct note_s
{
	char tone;
	int diesis;
	int octave;
};

int getOctaveFrom(float freq, int soctave, float* refFreq)
{
	float refFreqStart, refFreqStop;
	if(soctave<SAMPLE_OCTAVE_HARDLIMIT)
	{
		refFreqStart=refFreq[soctave];
		refFreqStop=refFreq[soctave+1];
	}
	else
	{
		printf("in the else!\n");
		int i;
		refFreqStart=refFreq[SAMPLE_OCTAVE_HARDLIMIT-1];
		for(i=0;i<soctave-SAMPLE_OCTAVE_HARDLIMIT-1;i++)
			refFreqStart*=2;
		refFreqStop=refFreqStart*2;
	}
	if( refFreqStart <= freq && freq < refFreqStop)
		return soctave;
	else if(freq < refFreqStart)
		return getOctaveFrom(freq, soctave-1, refFreq);
	else
		return getOctaveFrom(freq, soctave+1, refFreq);
}


int getSimilar(int ind, float *references, float ref)
{
	if(ref-references[ind] < 1 && ref-references[ind] > -1)
		return ind;
	else if(ref-references[ind] > 1)
		return getSimilar(ind+1, references, ref);
	else
		return getSimilar(ind-1, references, ref);
}

char getToneOf(float freq, int octave, float *references, int *diesis)
{
	char out;
	
	float reffreq=freq;
	int i;
	for(i=0;i<octave;i++)
		reffreq=reffreq/2;
	printf("%f\n", reffreq);

	int t=getSimilar(5, references, reffreq);
	*diesis=0;
	switch(t)
	{
		case 0:
			out='C';
			break;
		case 1:
			out='C';
			*diesis=1;
			break;
		case 2:
			out='D';
			break;
		case 3:
			out='D';
			*diesis=1;
			break;
		case 4:
			out='E';
			break;
		case 5:
			out='F';
			break;
		case 6:
			out='F';
			*diesis=1;
			break;
		case 7:
			out='G';
			break;
		case 8:
			out='G';
			*diesis=1;
			break;
		case 9:
			out='A';
			break;
		case 10:
			out='A';
			*diesis=1;
			break;
		case 11:
			out='B';
			break;
	}
	return out;
}

struct note_s getNote(float freq)
{
	struct note_s out;
	float cvector[SAMPLE_OCTAVE_HARDLIMIT]={
		16.0		/*C0*/,
		32.0		/*C1*/,
		64.0		/*C2*/,
		128.0		/*C3*/,
		256.0		/*C4*/,
		512.0		/*C5*/,
		1024.0	/*C6*/,
		2048.0	/*C7*/
	};
	float startNote[12]={
		16.35	/*C0*/,
		17.32	/*C#0*/,
		18.35	/*D0*/,
		19.45	/*D#0*/,
		20.60	/*E0*/,
		21.83	/*F0*/,
		23.12	/*F#0*/,
		24.50	/*G0*/,
		25.96	/*G#0*/,
		27.50	/*A0*/,
		29.14	/*A#0*/,
		30.87	/*B0*/,
	};

	out.octave=getOctaveFrom(freq, 4, cvector);
	printf("octave: %d\n", out.octave);
	out.tone=getToneOf(freq,out.octave, startNote, &out.diesis);
	return out;
}

int getMax(unsigned size, double *invec)
{
	int max=0;
	int i;
	for(i=0;i<size;i++)
		if(invec[i]>invec[max])
			max=i;
	return max;
}

int main(int argc, char **argv)
{
	unsigned i=0;
	ssize_t readed=0;
	int fd=-1;
	byte_t buf[SAMPLE_RATE];

	double *in, *rout;
	fftw_complex *out;
	fftw_plan p1;
	out=(fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (SAMPLE_RATE/2+1));
	in=(double*) fftw_malloc(sizeof(double) * SAMPLE_RATE);
	rout=(double*) fftw_malloc(sizeof(double) * BASE_RATE);

	fd=open(argv[1], O_RDONLY);

	long seeker=atol(argv[2])*SAMPLE_RATE;

	lseek(fd, seeker, SEEK_SET);

	readed=read(fd, buf, SAMPLE_RATE);
	close(fd);

	for(i=0;i<readed;i++)
	{
		in[i]=(double)(buf[i]&0xff);
	}

	/* warp to the complex plane */
	p1=fftw_plan_dft_r2c_1d(readed, in, out, FFTW_ESTIMATE);
	{
		int repeat;
		for(repeat=0;repeat<REPETITION;repeat++)
			fftw_execute(p1);
	}

	int nc=((int)BASE_RATE/(int)SAMPLE_RATE);
	for(i=0;i<readed/2+1;i++)
	{
//		printf("setting %d\n", i*nc);
		rout[i*nc]=abs(cimag(out[i]));
		int k;
		for(k=1;k<=(nc-1);k++)
		{
//			printf("resetting %d\n", i*nc+k);
			rout[i*nc+k]=0.0;
		}
	}

	fftw_destroy_plan(p1);
	fftw_free(out);
	free(in);

	float max=(float)getMax(readed/2+1, rout);

	printf("%f\n", max);

	struct note_s n=getNote(max);
	printf("It's a %c%c%d\n", n.tone, n.diesis*35, n.octave);
	return 0;
}
