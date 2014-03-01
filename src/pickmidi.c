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
#include <fftw3.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>

/* PickMidi Headers */
#include "fft.h"
#include "fft_note_finder.h"
#include "const.h"
#include "defs.h"

int main(int argc, char **argv){
	ssize_t b_read = 0, fft_size;
	int fd, peak_idx;
	short int skip;
	byte_t buf[FFT_SAMPLE_RATE], note;
	double *fft_out, peak_intensity, last_peak_intensity;
	freq_t peak, last_peak;
	
	
	if ((fd = open(argv[1], O_RDONLY)) == -1){
		perror("open ");
		exit(EXIT_FAILURE);
	}
	
	last_peak = skip = 0;
	last_peak_intensity = 50000;
	
	while((b_read=read(fd, buf, FFT_SAMPLE_RATE)) > 0){
		/* Perform a FFT of size b_read, with buf as input data,
		 * fftPeak return an index of the fft retult array corresponding 
		 * to the resulting peak frequency. */ 
		fft_size = fft(buf, b_read, &fft_out);
		peak_idx = fftPeak(fft_size, fft_out);
		peak_intensity = fft_out[peak_idx];
		peak = (freq_t)peak_idx;
#ifdef FFT_DEBUG
	printf("peak: %f fft_size: %d peak_intensity: %f", peak, fft_size, peak_intensity);
#endif
	
		/* TODO: Map the frequency intensity to the MIDI velocity 
		 * (Is it correct?) At least to understand if any note is playing. */ 
		if(peak_intensity < INTENSITY_THRESHOLD || 
			(last_peak == peak /*&& last_peak_intensity >= peak_intensity TODO:map needed before*/)){ 
			skip = 1;
		}
		
		if(skip == 0){
			note = getNote(peak);
			printf("Note (hex: %x, dec: %d, freq: %d)\n", note, (int)note, peak_idx); 
			/* TODO: play the note here */	
		}
		
		/* Save peak history and clean the buffers */
		last_peak = peak;
		last_peak_intensity = peak_intensity;
		bzero((void *)buf, FFT_SAMPLE_RATE);
		bzero((void *)fft_out, (size_t)fft_size);
		peak_idx = skip = 0;
		peak_intensity=0.0;
	}
	if (b_read == -1){
		perror("read ");
		exit(EXIT_FAILURE);
	}
	close(fd);
	/* TODO: read the TODO file */
	
	return 0;
}
