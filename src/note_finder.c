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

int getOctaveFrom(float freq, int soctave)
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

int getSimilar(int ind, float *references, float ref, int iter)
{
	int prev_ind, next_ind;
	float sign_dist, dist, dist_prev, dist_next;
	
	if (iter > N_SEMITONES){
		fprintf(stderr, "Error in getSimilar");
		exit(EXIT_FAILURE);
	}
	
	prev_ind = ((ind-1) < 0) ? N_SEMITONES - 1 : (ind - 1);
	next_ind = (ind+1)%N_SEMITONES;
	sign_dist = ref-references[ind];
	dist = fabs(sign_dist);
	dist_prev =  fabs(ref-references[prev_ind]);
	dist_next = fabs(ref-references[next_ind]);
	
	if(dist < dist_prev && dist < dist_next)
		return ind;	
	else if(sign_dist > 1)
		return getSimilar(next_ind, references, ref, iter+1);
	else
		return getSimilar(prev_ind, references, ref, iter+1);
}

char getMidiToneOf(float freq, int octave)
{
	/* TODO: do it kinldy :D */
	float references[N_SEMITONES]={
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
	int t;
	float reffreq=freq;
	int i;
	for(i=0;i<octave;i++)
		reffreq=reffreq/2;

	t=getSimilar((N_SEMITONES - 1)/2, references, reffreq, 0);
	return (char)(t + MIDI_C1);
}

byte_t getNote(freq_t freq)
{
	int octave, tone;
	
	octave = getOctaveFrom(freq, 4);
	tone = getMidiToneOf(freq, octave);
	return (byte_t) (tone + N_SEMITONES * (octave - 1));
}


