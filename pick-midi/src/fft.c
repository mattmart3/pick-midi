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

/* External headers */ 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>
#include <strings.h>

/* PickMidi Headers */
#include "const.h"
#include "defs.h"

/* Return the index where there is the peak frequency */
int fftPeak(ssize_t size, double *invec)
{
	int max=HIGH_PASS_FILTER_START;
	int i;
	for(i=HIGH_PASS_FILTER_START;i<size;i++) /* TODO: think about a filter instead */
		if(invec[i]>invec[max])
		{
			max=i;
		}
	return max;
}

ssize_t fft(byte_t *buf, ssize_t fft_size, double **rout){
	int i=0;
	double *in;
	ssize_t out_size = fft_size / 2 + 1;
	fftw_complex *out;
	fftw_plan p1;
	
	int nc=((int)FFT_BASE_RATE/(int)FFT_SAMPLE_RATE);
	in=(double*) fftw_malloc(sizeof(double) * (size_t)fft_size);
	out=(fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (size_t)out_size);
	
	(*rout) = (double *)fftw_malloc(sizeof(double) * (size_t)(out_size * nc));
		for(i=0;i<fft_size;i++)
	{
		in[i]=(double)(buf[i]&0xff);
	}
	/* warp to the complex plane */
	p1=fftw_plan_dft_r2c_1d((int)fft_size, in, out, FFTW_ESTIMATE);
	fftw_execute(p1);
	
	bzero(*rout, (size_t)out_size);
	for(i = 0; i < out_size; i++)
	{
		(*rout)[i*nc] = GetFrequencyIntensity(creal(out[i]), cimag(out[i]));
	}
#ifdef FFT_DEBUG
		printf("%d : %f\n", i*nc, (*rout)[i*nc]);
#endif
	fftw_destroy_plan(p1);
	fftw_free(out);
	free(in);
	return out_size*nc;
}
