#ifndef IntroSounds_h
#define IntroSounds_h

#include <Constants.h>  
#include <SensorizerServer.h>  

#include "general_midi_sounds.h"

struct PlaylistNote {
	char pitch;
	char delay;
};

// whether to play fatty drumroll, or simplier startup sound
#define PLAY_FAT_ROLL 0

// the time in ms of one beat
#define BEAT 70


void playIntro(SensorizerServer* server) {
	const unsigned char chan = MIDI_CHANNEL;
	const unsigned char vel = 20;
	const unsigned char velAccent = 35;
	// PlaylistNote[] melody = {

	// };

#if PLAY_FAT_ROLL
	// rolls!
	for (char i = 0; i < 9; i++) {
		unsigned char pitch;

		if (i > 5)
			pitch = LowFloorTom;
		else if (i > 2) 
			pitch = Low_MidTom;
		else 
			pitch = HighTom;

	    server->midiDevice->note(true, chan, pitch, (i % 3 == 0) ? velAccent : vel);
	    delay(BEAT);
	}

	server->midiDevice->note(true, chan, BassDrum1, vel);
#else
	server->midiDevice->note(true, chan, HighFloorTom, vel);	
	delay(BEAT);
	server->midiDevice->note(true, chan, LowFloorTom, velAccent);	
	delay(BEAT * 2);
	server->midiDevice->note(true, chan, ElectricSnare, vel);
	server->midiDevice->note(true, chan, BassDrum1, vel);
#endif	

    server->midiDevice->note(true, chan, CrashCymbal1, velAccent - 20); // crash is REALLY LOUD

    //delay(BEAT);
};

#endif
