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

/* PickMidi Headers */
#include "fft.h"
#include "const.h"
#include "defs.h"


int main(int argc, char **argv){
	ssize_t b_read=0;
	int fd;
	byte_t buf[FFT_SAMPLE_RATE];

	if ((fd = open(argv[1], O_RDONLY)) == -1){
		perror("open ");
		exit(EXIT_FAILURE);
	}
	
	if ((b_read=read(fd, buf, FFT_SAMPLE_RATE)) == -1){
		perror("read ");
		exit(EXIT_FAILURE);
	}
	
	close(fd);

	/* Perform a FFT of size b_read, with buf as input data */ 
	fft(buf, b_read);
	
	/* TODO: read the TODO file */
	
	return 0;
}
