/*
 * Greg/Output.cpp
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

#include "Output.h"
#include <iostream>
#include GREG_OUTPUT_H

Output * Output::outputInstance = NULL;

Output * Output::getInstance()
{
	if (!outputInstance)
	{
		outputInstance = new GREG_OUTPUT();
	}
	return outputInstance;
}

void Output::play(Note &note, double seconds)
{
	std::cout << "PLAYING NOTE " << note.pitch() << std::endl;
	noteOn(note);
	CFRunLoopTimerContext context = {0, note.copy()};
	CFRunLoopTimerRef playTimer = CFRunLoopTimerCreate(NULL, CFAbsoluteTimeGetCurrent()+seconds, 0, 0, 0, playCallback, &context);
	CFRunLoopAddTimer(CFRunLoopGetMain(), playTimer, kCFRunLoopCommonModes);
}

void playCallback(CFRunLoopTimerRef timer, void *info)
{
	Note *note = (Note *)info;
	std::cout << "ENDING NOTE " << note->pitch() << std::endl;
	Output::getInstance()->noteOff(*note);
	delete note;
}