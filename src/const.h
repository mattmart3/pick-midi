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

#ifndef CONST_H
#define CONST_H

#define BASE_RATE		44100
#define WINDOW_RATE 	4410
#define MIDI_C1			24
#define N_SEMITONES		12
#define SAMPLE_OCTAVE_HARDLIMIT 10
#define HIGH_PASS_FILTER_START 50
#define TRIGGER_THRESHOLD	(0xff/2)

#define FREQ_C0	16.35
#define TWELVE_RADIX_OF_TWO 1.0594630

#endif
