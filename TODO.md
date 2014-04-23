#TODO

##Notes Recognition:
 * Do some in-depth test with the trigger frequency finder.
 * Check the best parameters configuration trying to cut down the latency.
 * We probably need filters. Check which ones and how to apply them.
 * Do a reasonable peak to MIDI velocity convertion.
 * Test the whole PHASE 1 playing the notes with a MIDI sequencer (focusing on latency, accuracy, aplitute/velocity relation).

##Concurrency
 * Do the notes recognition concurrently with 6 input files running a trigger frequency finder per each file (think about sync problems too).

##Input Reading/Capturing
 * How do we get the 6 signals? 
 	* Capturing from a 6 input channels sound card?
 	* Capturing from  more then one sound card (with less then 6 input channels each)? 
 	* Through an ad-hoc device (are we able to build it)?
 * Do the Concurrency part reading the actual signals from the multi-channels pickup.

##Testing
 * test(); while(!working){ rethink(); test(); } //Hoping not to diverge inside the loop
