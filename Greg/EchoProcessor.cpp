/*
 * Greg/EchoProcessor.cpp
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

#include "EchoProcessor.h"
#include "Output.h"

void EchoProcessor::process(Note &n)
{
	static Output * output = Output::getInstance();
	if (n.velocity == 0) {
		output->noteOff(n);
		output->noteOff(lastNote);
	} else {
		output->noteOn(n);
		lastNote.midiNote = n.midiNote += (rand() % 12 - 5);
		lastNote.velocity = n.velocity;
		output->noteOn(lastNote);
	}
}