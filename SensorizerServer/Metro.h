/***
	A tick timer for use in loops.
***/
#ifndef Metro_h
#define Metro_h


class Metro {
	int lastTimeTicked;

public:
	Metro();

	/***

	Returns > 0 if a tick has happened.

	Returns 0 if between ticks.

	***/
	int hasTicked();

	/*** 
		The length of time in milliseconds between ticks.

		Set to 0 to disable Metronome.
	***/
	int timePerTick;
};

#endif