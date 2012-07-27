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

