/*
 * Greg/Output.h
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

#ifndef Greg_Output_h
#define Greg_Output_h

#include "MIDIUtils.h"
#include <CoreFoundation/CoreFoundation.h>

#define GREG_OUTPUT MIDIOutput
#define GREG_OUTPUT_H <GREG_OUTPUT.h>

class Output {
protected:
	static Output * outputInstance;
public:
	virtual void noteOn(Note &note) = 0;
	virtual void noteOff(Note &note) = 0;
	void play(Note &note, double seconds);
	static Output * getInstance();
};

void playCallback(CFRunLoopTimerRef timer, void *info);

#endif
