/*
 * Greg/MIDIOutput.cpp
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

#include <iostream>
#include "MIDIOutput.h"
#include "MIDIConstants.h"
#include "MIDIUtils.h"

// This call creates the Graph and the Synth unit...
// Adapted from Mac OS X sample code PlaySoftMIDI
void MIDIOutput::CreateAUGraph (AUGraph &outGraph, AudioUnit &outSynth)
{
	//create the nodes of the graph
	AUNode synthNode, limiterNode, outNode;
	
	AudioComponentDescription cd;
	cd.componentManufacturer = kAudioUnitManufacturer_Apple;
	cd.componentFlags = 0;
	cd.componentFlagsMask = 0;
	
	NewAUGraph(&outGraph);
	
	cd.componentType = kAudioUnitType_MusicDevice;
	cd.componentSubType = kAudioUnitSubType_DLSSynth;
	
	AUGraphAddNode(outGraph, &cd, &synthNode);
	
	cd.componentType = kAudioUnitType_Effect;
	cd.componentSubType = kAudioUnitSubType_PeakLimiter;  
	
	AUGraphAddNode(outGraph, &cd, &limiterNode);
	
	cd.componentType = kAudioUnitType_Output;
	cd.componentSubType = kAudioUnitSubType_DefaultOutput;  
	AUGraphAddNode(outGraph, &cd, &outNode);
	
	AUGraphOpen(outGraph);
	
	AUGraphConnectNodeInput(outGraph, synthNode, 0, limiterNode, 0);
	AUGraphConnectNodeInput(outGraph, limiterNode, 0, outNode, 0);
	
	// ok we're good to go - get the Synth Unit...
	AUGraphNodeInfo(outGraph, synthNode, 0, &outSynth);
}

MIDIOutput::MIDIOutput() {
	CreateAUGraph(graph, synthUnit);
	
	// initialize and start the graph
	AUGraphInitialize(graph);
	
	// set our bank
	MusicDeviceMIDIEvent(synthUnit, CONTROL_CHANGE << 4, 0, 0, 0);
	
	MusicDeviceMIDIEvent(synthUnit, PROGRAM_CHANGE << 4, 0, 0, 0);
	
	MusicDeviceMIDIEvent(synthUnit, CONTROL_CHANGE << 4 | 1, 0, 0, 0);
		
	MusicDeviceMIDIEvent(synthUnit, PROGRAM_CHANGE << 4 | 1, 4, 0, 0);
	
	AUGraphStart(graph);
}

MIDIOutput::~MIDIOutput() {
	if (graph) {
		DisposeAUGraph(graph);
	}
}

void MIDIOutput::noteOn(Note &note) {
	std::cout << "Note On: " << note.pitch() << std::endl;
	MusicDeviceMIDIEvent(synthUnit, NOTE_ON << 4 | note.channel, note.midiNote, note.velocity, 0);
}

void MIDIOutput::noteOff(Note &note) {
	std::cout << "Note Off: " << note.pitch() << std::endl;
	MusicDeviceMIDIEvent(synthUnit, NOTE_OFF << 4 | note.channel, note.midiNote, 0, 0);
}