#pragma once
#include <iostream>

using namespace std;

class Note {
public:
	bool isRest;             
	short noteNumber;	// MIDI note Value	
	int duration;		// In samples

	Note(short noteNumber = 0, int duration = 0, bool isRest = false);
	Note(const Note &note);
	~Note();

	Note& operator=(const Note &note);
	bool operator==(const Note &note);
	bool operator!=(const Note &note);

	friend ostream& operator<<(ostream& output, const Note& note);
	
	void SetPitch(short pitch);
	short ShiftPitch(bool higher, bool octave);
	int ChangeDuration(int amount);
};