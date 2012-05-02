/*
 * Greg/MIDIInput.cpp
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

#include <CoreMIDI/CoreMIDI.h>
#include "Input.h"
#include "Processor.h"
#include "MIDIConstants.h"
#include "MIDIUtils.h"
#include <cstdio>

void Input::initialize()
{
	MIDIEndpointRef source = NULL;
	
	ItemCount sourceCount = MIDIGetNumberOfSources();
	for (ItemCount i = 0; i < sourceCount; i++) {
		MIDIEndpointRef src = MIDIGetSource(i);
		source = src;
		break;
	}
	if (source) {
		MIDIClientRef client;
		MIDIClientCreate(CFSTR("Greg"), Input::notifyProc, NULL, &client);
		MIDIPortRef inputPort;
		MIDIInputPortCreate(client, CFSTR("Greg Input"), Input::readProc, NULL, &inputPort);
		MIDIPortConnectSource(inputPort, source, NULL);
	}
}

void Input::notifyProc(const MIDINotification * message, void *refCon)
{
}

void Input::readProc(const MIDIPacketList * pktlist, void *readProcRefCon, void *srcConnRefCon)
{
	const MIDIPacket * packet = pktlist->packet;
	uint8_t midiCommand = packet->data[0] >> 4;
	if (midiCommand == SYSTEM_EXCLUSIVE) return; // Not dealing with SysEx packets
	Note note;
	static Processor * processor = Processor::getInstance();
	switch (midiCommand) {
		case NOTE_ON:
		case NOTE_OFF:
			note.midiNote = packet->data[1];
			note.velocity = packet->data[2];
			note.channel = 0;
			note.timestamp = CFAbsoluteTimeGetCurrent();
			processor->process(note);
			break;
		case CONTROL_CHANGE:
			printf("CHANGE: %d %d\n", packet->data[1], packet->data[2]);
			processor->control(packet->data[1], packet->data[2]);
			break;
		default:
			printf("%d %d %d %d\n", midiCommand, packet->data[0], packet->data[1], packet->data[2]);
			break;
	}
}