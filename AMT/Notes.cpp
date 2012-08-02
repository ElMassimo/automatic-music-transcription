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

void Notes::AddNote(Note& newNote)
{
	this->push_back(newNote);
	totalDuration += newNote.duration;
}

void Notes::InsertNote(int index, Note& newNote)
{
	NotesIterator it = this->begin();
	advance(it, index);
	this->insert(it, newNote);
	totalDuration += newNote.duration;
}

void Notes::FlipSilence(int index, int noteNumber)
{
	NotesIterator it = this->begin();
	advance(it, index);
	it->isRest = !it->isRest;
	it->noteNumber = noteNumber;
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
		{
			noteStart += it->duration;
		}

	throw std::exception("GetNoteAt: We ran out of notes");
}

void Notes::CombineNotes(Notes& n1, Notes& n2, int when)
{
	int d1,d2;
	d1 = d2 = when;

	NotesIterator it1 = n1.GetNoteAt(d1);
	NotesIterator it2 = n2.GetNoteAt(d2);

	this->clear();
	
	if(it1 != n1.end())
	{	
		NotesIterator it = n1.begin();
		while(it != it1)
			this->push_back(Note(*it++));
		
		Note splitNote1(*it1);
		splitNote1.duration = d1;
		this->push_back(splitNote1);	
	}
	else
	{
		int i = 0 + 1;
	}

	if(it2 != n2.end())
	{
		Note splitNote2(*it2);
		splitNote2.duration -= d2;
		this->push_back(splitNote2);

		it2++;
		while(it2 != n2.end())
			this->push_back(Note(*it2++));
	}	
	else
	{
		int i = 0 + 1;
	}
	this->totalDuration = n2.totalDuration;
}

void Notes::CropAt(int when)
{
	if(totalDuration <= when)
		return;
	
	// Adjust the total duration.
	totalDuration = when;

	int noteStart = 0;
	for(NotesIterator it = this->begin(); it != this->end(); it++)
		// We will get the note that is present at the time.
		if(noteStart + it->duration > when)
		{
			// Calculate the offset inside the note to get to that time.
			when -= noteStart;

			// Crop the note to exactly fill the remaining samples.
			it->duration = max<int>(0, when);

			// If the note has duration, we want to keep it.
			if(it->duration > 0)
				it++;

			// Erase the notes after the point of cut.
			this->erase(it, this->end());


			return;
		}
		else
		{
			noteStart += it->duration;
		}

		throw std::exception("GetNoteAt: We ran out of notes");

	return;


	// Obtain a pointer to the note, and the adjusted duration of the note.
	NotesIterator it = GetNoteAt(when);
	if(it == this->end())
		return;

	// Crop the note to exactly fill the remaining samples.
	it->duration = max<int>(0, when);

	// If the note has duration, we want to keep it.
	if(it->duration > 0)
		it++;

	// Erase the notes after the point of cut.
	this->erase(it, this->end());
	
	// Adjust the total duration.
	totalDuration = when;
}


