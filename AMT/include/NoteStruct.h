#pragma once
#include <iostream>

#define DEFAULT_NOTE_NUMBER 60
#define MIN_NOTE 21
#define MAX_NOTE 108
#define A440 69

class Note {
public:
	bool isRest;             
	short noteNumber;	// MIDI note Value	
	int duration;		// In samples

	Note(short noteNumber = 0, int duration = 0, bool isRest = false);
	Note(const Note &note);
	~Note();

	Note& operator=(const Note &note);
	bool operator==(const Note &note) const;
	bool operator!=(const Note &note) const;

	friend std::ostream& operator<<(std::ostream& output, const Note& note);
	
	void SetPitch(short pitch);
	short ShiftPitch(bool higher, bool octave);
	int ChangeDuration(double percentage);
};
