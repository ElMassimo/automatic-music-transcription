#include "NoteStruct.h"

Note::Note(short noteNumber, int duration, bool isRest)		
	: isRest(isRest),
	noteNumber(noteNumber),
	duration(duration)
{}

Note::Note(const Note &note)
	: isRest(note.isRest),
	noteNumber(note.noteNumber),
	duration(note.duration)
{}

Note::~Note()
{
}

Note& Note::operator=(const Note &note){
	isRest = note.isRest;
	noteNumber = note.noteNumber;
	duration = note.duration;
	return *this;
}

bool Note::operator==(const Note &note){
	return isRest == note.isRest && noteNumber == note.noteNumber && duration == note.duration;
}

bool Note::operator!=(const Note &note){
	return !(*this == note);
}

std::ostream& operator<<(std::ostream& output, const Note& note)
{
	if(note.isRest)
		output << "Silence" << "\n";
	else
		output << "Note number: " << note.noteNumber << "\n";
	output << "Duration: " << note.duration << " samples\n\n";
	return output;
}

void Note::SetPitch(short pitch)
{
	if(pitch > 127)
		noteNumber = 127;
	else if(pitch < 0)
		noteNumber = 0;
	else
		noteNumber = (pitch) % 128;
}

short Note::ShiftPitch(bool higher, bool octave)
{
	short pitchShift = 1;
	if(octave)
		pitchShift = 12;
	if(!higher)
		pitchShift = -pitchShift;

	SetPitch(noteNumber + pitchShift);
	return noteNumber;
}

int Note::ChangeDuration(int amount)
{
	duration += amount;
	return duration;
}

