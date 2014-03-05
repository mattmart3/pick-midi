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
#include "const.h"
#include "defs.h"
#include "freq_finder.h"
#include "note_finder.h"


int main(int argc, char **argv){
	ssize_t b_read = 0, fft_size;
	int fd;
	short int skip;
	byte_t buf[WINDOW_RATE], 
		note, last_note, peak, last_peak;
	freq_t freq;
	if ((fd = open(argv[1], O_RDONLY)) == -1){
		perror("open ");
		exit(EXIT_FAILURE);
	}
	
	/* Variables initialization */
	last_peak = skip = 0;
	
	while((b_read=read(fd, buf, WINDOW_RATE)) > 0){
		freq = getFrequency(buf, b_read, &peak); 
		/* TODO: Map the frequency intensity to the MIDI velocity */ 
		
		note = getNote(freq);
		if ( peak <= last_peak && last_note == note )
			;//SKIP
		else
			printf("Note (hex: %x, dec: %d, freq: %f, peak %x)\n", note, (int)note, (float)freq, peak); 
		/* TODO: play the note here */	
		
		/* Save peak history and clean the buffers */
		last_peak = peak;
		last_note = note;
		bzero((void *)buf, WINDOW_RATE);
		skip = 0;
	}
	if (b_read == -1){
		perror("read ");
		exit(EXIT_FAILURE);
	}
	close(fd);
	/* TODO: read the TODO file */
	
	return 0;
}
