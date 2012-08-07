#include "Notes.h"
#include <algorithm>

Notes::Notes()
{
	totalDuration = 0;
}

Notes::Notes(const Notes &notes)
{
	ReplaceNotes(notes);
}

Notes::~Notes()
{
}

void Notes::AddNote(Note& newNote)
{
	if(newNote.duration == 0)
		return;
	this->push_back(newNote);
	totalDuration += newNote.duration;
}

NotesIterator Notes::GetNote(int noteIndex)
{
	NotesIterator it = this->begin();
	advance(it, noteIndex);
	return it;
}

bool IsRest(Note& note)
{
	return note.isRest;
}

void Notes::FlipSilence(int index)
{
	NotesIterator it = GetNote(index);

	// If the note is a silence, we want to set the pitch from another note in the sequence
	if(it->isRest)
	{
		// First search for a note after this one
		NotesIterator afterNote = find_if_not(it, this->end(), IsRest);

		if(afterNote == this->end())
		{
			// If there is none, search for a previous note
			NotesReverseIterator beforeNote(it);
			beforeNote = find_if_not(beforeNote, this->rend(), IsRest);

			if(beforeNote == this->rend())
				// If there is none, set a default note
				it->noteNumber = DEFAULT_NOTE_NUMBER;
			else
				it->noteNumber = beforeNote->noteNumber;
		}
		else
			it->noteNumber = afterNote->noteNumber;
	}

	// Flip the silence
	it->isRest = !it->isRest;
}

void Notes::ChangePitch( int noteIndex, bool octave, bool down )
{
	NotesIterator it = GetNote(noteIndex);
	throw std::exception("Not implemented");
}

void Notes::ChangeDuration( int noteIndex, bool newDurationPercentage, bool expandNext )
{
	NotesIterator it = GetNote(noteIndex);
	throw std::exception("Not implemented");
}

void Notes::ReplaceNotes(const Notes& otherNotes)
{
	this->clear();
	this->insert(this->begin(), otherNotes.begin(), otherNotes.end());
	totalDuration = otherNotes.totalDuration;	
}

void Notes::CombineNotes(Notes& n1, Notes& n2, int when)
{
	int d1,d2;
	d1 = d2 = when;

	NotesIterator it1 = n1.GetNoteAt(d1);
	NotesIterator it2 = n2.GetNoteAt(d2);

	this->clear();

	// Assume that we will copy the whole thing
	if(it1 == n1.end())
	{
		it1--;
		d1 = n1.back().duration;
	}

	NotesIterator it = n1.begin();
	while(it != it1)
		this->push_back(Note(*it++));

	// Copy the last note of the first sequence, crop it, and add it
	Note splitNote1(*it1);
	splitNote1.duration = d1;
	if(splitNote1.duration > 0)
		this->push_back(splitNote1);
	
	// Fill the space in between with silence
	if(n1.totalDuration < when)
		this->push_back(Note(0, when - n1.totalDuration, true));
	
	// Copy the second part only if is not empty
	if(it2 != n2.end())
	{
		// Copy the first note of the second sequence, crop it, and add it
		Note splitNote2(*it2);
		splitNote2.duration -= d2;
		if(splitNote2.duration > 0)
			this->push_back(splitNote2);

		it2++;
		while(it2 != n2.end())
			this->push_back(Note(*it2++));
	}	

	this->totalDuration = max<int>(when, n2.totalDuration);
}

bool Notes::SplitNote(int noteIndex, double when, double silencePercentage)
{
	if(silencePercentage < 0 || when < 0 || when + silencePercentage > 1)
		throw "SplitNote: start and duration of the silence must be a percentage";

	NotesIterator it = GetNote(noteIndex);

	// We don't want to create a note with zero duration, or split a silence
	if(when == 0 || when == 1 || it->isRest)
		return false;

	// Create a copy of the note for the first part, a silence, and leave the original note as the second part
	int noteDuration = it->duration;
	Note* firstNote = new Note(it->noteNumber, noteDuration * when);
	Note* silence = new Note(0, noteDuration * silencePercentage, true);
	it->duration = noteDuration - firstNote->duration - silence->duration;

	this->insert(it, *firstNote);
	this->insert(it, *silence);

	return true;
}

void Notes::CropAt(int when)
{
	int newTotalDuration = when;

	// Obtain a pointer to the note, and the offset inside the note to reach the given time
	NotesIterator it = GetNoteAt(when);
	if(it == this->end())
		return;

	// Crop the note to exactly fill the remaining samples.
	it->duration = when;

	// Remove notes with no duration
	if(it->duration > 0)
		it++;

	// Erase the notes after the point of cut.
	this->erase(it, this->end());

	// Adjust the total duration.
	totalDuration = newTotalDuration;
}

NotesIterator Notes::GetNoteAt(int &when) 
{
	if(totalDuration <= when)
		return this->end();

	int noteStart = 0;

	for(NotesIterator it = this->begin(); it != this->end(); it++)
		// We will get the note that is present at the time.
		if(noteStart + it->duration > when)
		{
			// Calculate the offset inside the note to get to that time.
			when -= noteStart;
			return it;
		}
		else
			noteStart += it->duration;

	throw std::exception("GetNoteAt: We ran out of notes");
}

int Notes::GetRealDuration()
{
	int realDuration = 0;
	for (NotesIterator it = this->begin(); it != this->end(); it++)
		realDuration += it->duration;
	return realDuration;
}

Notes& Notes::operator=(const Notes &notes){
	this->ReplaceNotes(notes);
	return *this;
}
