/* 
 * pick-midi, a guitar to MIDI converter. 
 * 
 * Copyright (C) 2014 Davide Berardi, Matteo Martelli.
 * 
 * This file is part of pick-midi.
 * 
 * pick-midi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * pick-midi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with pick-midi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>

#include "const.h"
#include "defs.h"

int getOctaveFrom(double freq, int soctave)
{
	float refFreq[SAMPLE_OCTAVE_HARDLIMIT]={
		16.0		/*C0*/,
		32.0		/*C1*/,
		64.0		/*C2*/,
		128.0		/*C3*/,
		256.0		/*C4*/,
		512.0		/*C5*/,
		1024.0		/*C6*/,
		2048.0		/*C7*/
	};
	float refFreqStart, refFreqStop;
	if(soctave<SAMPLE_OCTAVE_HARDLIMIT)
	{
		refFreqStart=refFreq[soctave];
		refFreqStop=refFreq[soctave+1];
	}
	else
	{
		int i;
		refFreqStart=refFreq[SAMPLE_OCTAVE_HARDLIMIT-1];
		for(i=0;i<soctave-SAMPLE_OCTAVE_HARDLIMIT-1;i++)
			refFreqStart*=2;
		refFreqStop=refFreqStart*2;
	}
	if( refFreqStart <= freq && freq < refFreqStop)
		return soctave;
	else if(freq < refFreqStart)
		return getOctaveFrom(freq, soctave-1);
	else
		return getOctaveFrom(freq, soctave+1);
}


int getSimilar(int ind, double *references, float ref)
{
	float dist = ref-references[ind];
	
	if(dist < 1 && dist > -1){
		dist = fabs(dist);
		int prev_ind = ((ind-1) < 0) ? N_SEMITONES - 1 : (ind - 1);
		int next_ind = (ind+1)%N_SEMITONES;
		float dist_prev =  fabs(ref-references[prev_ind]);
		float dist_next = fabs(ref-references[next_ind]);
#ifdef GETNOTE_DEBUG
		printf("(%d, %d, %d) ", prev_ind, ind, next_ind);
		printf("%d (%f %f %f) ",ind, dist_prev, dist, dist_next);
#endif
		if (dist < dist_next && dist < dist_prev)
			return ind;
		else if (dist_next < dist)
			return next_ind;
		else
			return prev_ind;
	}	
	else if(dist > 1)
		return getSimilar(ind+1, references, ref);
	else
		return getSimilar(ind-1, references, ref);
}

char getMidiToneOf(float freq, int octave)
{
	int t,i;
	float reffreq;
	
	/* TODO: do it kinldy :D */
	double references[N_SEMITONES]={
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
	
	reffreq = freq;
	
	for(i=0;i<octave;i++)
		reffreq=reffreq/2;
#ifdef GETNOTE_DEBUG
	printf("octave %d reffreq %f\t", octave, reffreq);
#endif
	t = getSimilar((N_SEMITONES - 1)/2, references, reffreq);
	
	return (char)(t + MIDI_C1);
}

byte_t getNote(freq_t freq)
{
	int octave, tone;
	
	octave = getOctaveFrom(freq, 4);
	tone = getMidiToneOf(freq, octave);
	return (byte_t) (tone + N_SEMITONES * (octave - 1));
}


