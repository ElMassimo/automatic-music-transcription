#include "Notes.h"

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

void Notes::AddNote(int index, Note& newNote)
{
	NotesIterator it = this->begin();
	advance(it, index);
	this->insert(it, newNote);
	totalDuration += newNote.duration;
}

void Notes::FlipSilence(int index)
{
	NotesIterator it = this->begin();
	advance(it, index);
	it->isRest = !it->isRest;
}

NotesIterator Notes::EraseNote(int index)
{
	NotesIterator it = this->begin();
	advance(it, index);
	totalDuration -= it->duration;
	return this->erase(it);
}

void Notes::ReplaceNotes(const Notes& otherNotes)
{
	this->clear();
	this->insert(this->begin(), otherNotes.begin(), otherNotes.end());
	totalDuration = otherNotes.totalDuration;	
}

bool Notes::SplitNote(int noteIndex, double when, double silencePercentage)
{
	if(silencePercentage < 0 || when < 0 || when + silencePercentage > 1)
		throw "SplitNote: start and duration of the silence must be a percentage";
	
	if(when == 0 || when == 1)
		return false; // We don't want to create a note with zero duration.
	
	NotesIterator it = this->begin();
	advance(it, noteIndex);
	int noteDuration = it->duration;
	Note* firstNote = new Note(it->noteNumber, noteDuration * when);
	Note* silence = new Note(0, noteDuration * silencePercentage, true);
	it->duration = noteDuration - firstNote->duration - silence->duration;

	this->insert(it, *firstNote);
	this->insert(it, *silence);

	return true;
}

NotesIterator& Notes::GetNoteAt(int &when) 
{
	if(totalDuration < when)
		return this->end();

	int noteStart = 0;
	for(NotesIterator it = this->begin(); it != this->end(); it++)
		// We will get the note that is present at the time.
		if(noteStart + it->duration >= when)
		{
			// Calculate the offset inside the note to get to that time.
			when -= noteStart;
			return it;
		}
		else
		{
			noteStart += it->duration;
		}
}

void Notes::CropAt(int when)
{
	if(totalDuration < when)
		return;
	
	int onset = 0;
	for(NotesIterator it = this->begin(); it != this->end(); it++)
		// We will cut where the duration of the note exceeds the available samples.
		if(onset + it->duration >= when)
		{
			// Crop the note to exactly fill the remaining samples.
			it->duration = max<int>(when - onset, 0);

			// If the note has duration, we want to keep it.
			if(it->duration > 0)
				it++;

			// Erase the notes after the point of cut
			this->erase(it, this->end());
			break;
		}
		else
		{
			onset += it->duration;
		}

	// Adjust the total duration
	totalDuration = when;
}


