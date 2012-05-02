/*
 * Greg/Input.h
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

#ifndef Greg_Input_h
#define Greg_Input_h

#include <CoreMIDI/CoreMIDI.h>

namespace Input {
	void notifyProc(const MIDINotification * message, void *refCon);
	void readProc(const MIDIPacketList * pktlist, void *readProcRefCon, void *srcConnRefCon);
	void initialize();
}


#endif
