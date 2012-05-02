/*
 * Greg/GregProcessor.h
 *
 * Copyright (c) 2011-2012 Bill Meltsner
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the author be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef Greg_GregProcessor_h
#define Greg_GregProcessor_h

#include "Processor.h"
#include "Markov.h"
#include "Scale.h"
#include <deque>
#include <CoreFoundation/CoreFoundation.h>

class GregProcessor : public Processor {
public:
	GregProcessor();
	virtual ~GregProcessor();
	virtual void process(Note &n);
	virtual void control(uint8_t data1, uint8_t data2);
	void make_notes(CFRunLoopTimerRef timer);
	Markov::MarkovModel * melodyModel;
	Markov::MarkovModel * rhythmModel;
	std::deque<Note> * notes;
	int baseNote;
	uint8_t lastNote;
	const Scale::Scale * scale;
	std::string lastPitch;
	std::string lastLength;
	int lastPitchIndex;
	int totalVelocity;
	double beatLength; // in seconds
	double beatAccumulator;
private:
	CFRunLoopTimerRef noteTimer;
};

void make_notes(CFRunLoopTimerRef timer, void *info);
#endif
