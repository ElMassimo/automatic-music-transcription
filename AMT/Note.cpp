#include "NoteStruct.h"

Note::Note(short pitch, int duration, bool isRest)		
	: isRest(isRest),
	duration(duration)
{
	SetPitch(pitch);
}

Note::Note(const Note &note)
	: isRest(note.isRest),
	duration(note.duration)
{
	SetPitch(note.noteNumber);
}

Note::~Note()
{
}

Note& Note::operator=(const Note &note){
	isRest = note.isRest;
	noteNumber = note.noteNumber;
	duration = note.duration;
	return *this;
}

bool Note::operator==(const Note &note) const{
	return isRest == note.isRest && noteNumber == note.noteNumber && duration == note.duration;
}

bool Note::operator!=(const Note &note) const{
	return !(*this == note);
}

std::ostream& operator<<(std::ostream& output, const Note& note)
{
	if(note.isRest)
		output << "Silence" << "\n";
	else
	{
		output << "Note number: " << note.noteNumber << " (" <<  pow(2,(double)(note.noteNumber-A440)/12) * 440 << "Hz)\n";
	}
	output << "Duration: " << note.duration << " samples\n\n";
	return output;
}

void Note::SetPitch(short pitch)
{
	if(pitch > MAX_NOTE)
		noteNumber = MAX_NOTE;
	else if(pitch < MIN_NOTE)
		noteNumber = MIN_NOTE;
	else
		noteNumber = pitch;
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

int Note::ChangeDuration(double percentage)
{
	duration *= percentage;
	if(duration < 0)
		duration = 0;
	return duration;
}

