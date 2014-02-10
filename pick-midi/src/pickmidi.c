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
