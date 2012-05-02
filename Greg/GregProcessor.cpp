/*
 * Greg/GregProcessor.cpp
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

#include "GregProcessor.h"
#include "Output.h"
#include "Scale.h"
#include <iostream>
#include <algorithm>

GregProcessor::GregProcessor()
{
	melodyModel = new Markov::MarkovModel("/Users/bill/Desktop/gregmelodymodel.txt");
	if (melodyModel->nodes.empty()) {
		std::cerr << "NOTE MARKOV MODEL CREATION FAILED, ABORTING" << std::endl;
		exit(EXIT_FAILURE);
	}
	rhythmModel = new Markov::MarkovModel("/Users/bill/Desktop/gregrhythmmodel.txt");
	if (rhythmModel->nodes.empty()) {
		std::cerr << "RHYTHM MARKOV MODEL CREATION FAILED, ABORTING" << std::endl;
		exit(EXIT_FAILURE);
	}
	notes = new std::deque<Note>;
	lastPitch = "";
	lastLength = "q";
	noteTimer = NULL;
	beatLength = 0.4;
	lastPitchIndex = 0;
	beatAccumulator = 0;
}

GregProcessor::~GregProcessor()
{
	CFRunLoopTimerInvalidate(noteTimer);
	CFRelease(noteTimer);
	delete notes;
	delete melodyModel;
	delete rhythmModel;
}

void GregProcessor::process(Note &n)
{
	static Output * output = Output::getInstance();
	if (n.velocity == 0) {
		output->noteOff(n);
		if (notes->size() > 10) {
			totalVelocity -= notes->front().velocity;
			notes->pop_front();
		}
	} else {
		output->noteOn(n);
		notes->push_back(n);
		totalVelocity += n.velocity;
	}
}

void GregProcessor::control(uint8_t data1, uint8_t data2)
{
	if (data1 != 64) return; // just in case, we're only interested in pedaling
	if (data2) { // data2 > 0, pedal on - reset and start listening
		while (!notes->empty()) notes->pop_front();
		totalVelocity = 0;
	} else { // data2 == 0, pedal off - process what we heard
		if (notes->empty()) { // nothing got played while the pedal was down
			if (noteTimer) { // we're playing - time to stop!
				CFRunLoopTimerInvalidate(noteTimer);
				CFRelease(noteTimer);
				noteTimer = NULL;
			} else if (lastPitch.compare("") != 0) { // we're not playing but we're ready to - let's go!
				noteTimer = CFRunLoopTimerCreate(NULL, CFAbsoluteTimeGetCurrent()+1, beatLength, 0, 0, ::make_notes, NULL);
				CFRunLoopAddTimer(CFRunLoopGetMain(), noteTimer, kCFRunLoopCommonModes);
			}
		} else { // new notes, figure out our new key
			std::vector<uint8_t> midiNotes;
			double noteLength = 0;
			double curTimestamp = notes->begin()->timestamp;
			baseNote = 255;
			for (std::deque<Note>::iterator i = notes->begin(); i != notes->end(); ++i) {
				uint8_t note = i->midiNote;
				midiNotes.push_back(note);
				if (note < baseNote) baseNote = note;
				noteLength += i->timestamp - curTimestamp;
				curTimestamp = i->timestamp;
			}
			if (!noteTimer) { // we're currently not playing, get the tempo
				double newBeatLength = noteLength / notes->size();
				if (newBeatLength > 0.1) beatLength = newBeatLength; // guard against accidentally stopping and playing a chord to start again
			}
			for (int i = 0; i < midiNotes.size(); i++) {
				midiNotes[i] -= baseNote;
				midiNotes[i] %= 12;
				midiNotes[i]++;
			}
			baseNote %= 12;
			if (baseNote < 6) baseNote += 12;
			baseNote += 48; // orient around Middle C
			std::sort(midiNotes.begin(), midiNotes.end());
			// There's gotta be a better way to do this...
			scale = &Scale::IONIAN;
			for (std::vector<uint8_t>::iterator i = midiNotes.begin(); i != midiNotes.end(); ++i) {
				if (*i == 4) {
					scale = &Scale::DORIAN;
					break;
				} else if (*i == 11) {
					scale = &Scale::MIXOLYDIAN;
					break;
				}
			}
			if (lastPitch.compare("") == 0) lastPitch = "1"; // if we haven't played anything yet, mark us as ready
		}
	}
}

void GregProcessor::make_notes(CFRunLoopTimerRef timer)
{
	std::string nextPitch = melodyModel->next(lastPitch);
	std::string nextLength = rhythmModel->next(lastLength);
	double noteLength = getNoteLength(nextLength);
	beatAccumulator += noteLength;
	if (beatAccumulator > 4) {
		if (rand() % 2) {
			noteLength -= beatAccumulator - 4;
			beatAccumulator = 4;
		}
		beatAccumulator -= 4;
	}
	noteLength *= beatLength;
	int nextPitchIndex = atoi(nextPitch.c_str())-1+(rand() % 3 - 1);
	int octaveChange = rand() % 9;
	if (octaveChange > baseNote / 12 + 1 && nextPitchIndex < lastPitchIndex) octaveChange = 1;
	else if (octaveChange == 0 && nextPitchIndex > lastPitchIndex) octaveChange = -1;
	else octaveChange = 0;
	if (nextPitchIndex > 6) nextPitchIndex -= 7;
	if (nextPitchIndex < 0) nextPitchIndex += 7;
	Note x;
	x.midiNote = baseNote + ((*(scale))[nextPitchIndex]-1) + (12 * octaveChange);
	x.velocity = 127;
	if (totalVelocity) x.velocity = totalVelocity / notes->size();
	if (x.velocity > 127) x.velocity = 127;
	x.channel = 1;
	lastPitch = nextPitch;
	lastPitchIndex = nextPitchIndex;
	lastLength = nextLength;
	CFRunLoopTimerSetNextFireDate(timer, CFAbsoluteTimeGetCurrent()+noteLength+0.01);
	Output::getInstance()->play(x, noteLength);
}

void make_notes(CFRunLoopTimerRef timer, void *)
{
	GregProcessor * processor = (GregProcessor *)Processor::getInstance();
	processor->make_notes(timer);
}