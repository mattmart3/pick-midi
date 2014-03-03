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


/* PickMidi Headers */
#include "const.h"
#include "defs.h"

freq_t getFrequency(byte_t *buf, ssize_t size, byte_t *peak){
	int i=0, cstate=0, tc=0, threshold=0;
	freq_t frequency=0;
	
	threshold = TRIGGER_THRESHOLD;
	cstate = 0;
	
	for(i = 0; i < (int)size; i++){
		
		if((cstate == 1) && ((buf[i]&0xff) < threshold)){
			cstate = 0;
			tc++;
		}
		
		if(cstate == 0 && ((buf[i]&0xff) >= threshold)){
			cstate = 1;
			tc++;
		}
		/* Do not change otherwise. */ 
	}
	
	(*peak) = 0xff; /* TODO: change this calculating it. */
	
	frequency = (freq_t)((float)tc*((float)BASE_RATE/(float)size)/2.0);
	
	return frequency;
}
