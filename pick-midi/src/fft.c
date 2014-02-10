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

/* PickMidi Headers */
#include "const.h"
#include "defs.h"

int fft(byte_t *buf, ssize_t fft_size){
	unsigned i=0;
	double *in;
	fftw_complex *out;
	fftw_plan p1;
	
	in=(double*) fftw_malloc(sizeof(double) * fft_size);
	out=(fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (fft_size/2+1));

	for(i=0;i<fft_size;i++)
	{
		in[i]=(double)(buf[i]&0xff);
	}

	/* warp to the complex plane */
	p1=fftw_plan_dft_r2c_1d(fft_size, in, out, FFTW_ESTIMATE);
	{
		int repeat;
		for(repeat=0;repeat<FFT_REPETITIONS;repeat++)
			fftw_execute(p1);
	}

	for(i=0;i<fft_size/2;i++)
	{
		double val=fabs(cimag(out[i])*creal(out[i]));
		printf("%f\n", val);
		int k;
		for(k=0;k<FFT_BASE_RATE/FFT_SAMPLE_RATE-1;k++)
			printf("0.0\n");
	}

	fftw_destroy_plan(p1);
	fftw_free(out);
	free(in);
	return 0;
}
