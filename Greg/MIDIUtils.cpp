/*
 * Greg/MIDIUtils.cpp
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

#include "MIDIUtils.h"

const char * pitches[12] = {
	"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"
};

std::string Note::pitch()
{
	char pitch[7];
	snprintf(pitch, 7, "%s%d|%d", pitches[midiNote % 12], midiNote / 12 - 1, channel);
	return std::string(pitch);
}

Note * Note::copy()
{
	Note * n = new Note;
	n->midiNote = midiNote;
	n->velocity = velocity;
	n->channel = channel;
	return n;
}

double getNoteLength(std::string code)
{
	if (!code.compare("s")) return 0.25;
	if (!code.compare("e")) return 0.5;
	if (!code.compare("h")) return 2;
	return 1;
}