#include "NoteRenderer.h"
using namespace AMT;

inline long SecondToFrames(double seconds)
{
	return seconds * Stk::sampleRate();
}

NoteRenderer::NoteRenderer(unsigned int samples)
{
	if(noteFrequency == NULL)
		NoteRenderer::Initialize();
	frames = new StkFrames(samples, 1);
}

NoteRenderer::~NoteRenderer()
{
	delete frames;
}

StkFloat* NoteRenderer::noteFrequency = NULL;

void NoteRenderer::Initialize()
{
	noteFrequency = new StkFloat[128];
	for(int i = 0; i < 128; i++)
		noteFrequency[i] = pow(2,(double)(i-57)/12) * 440;
}

void NoteRenderer::CleanUp()
{
	delete [] noteFrequency;
	noteFrequency = NULL;
}

void NoteRenderer::AddNote(Note &note, int start)
{
	if(note.isRest)
		return;

	// Create our generators
	SineWave sine;
	sine.setFrequency(noteFrequency[note.noteNumber]);

	// Prepare and set the frame window
	long nFrames = note.duration;
	frames->setFrameWindow(start, nFrames);

	// Load the frames
	sine.tick(*frames);

	// Reset the window
	frames->resetFrameWindow();
}

void NoteRenderer::AddNotes(Notes &notes, int start)
{
	int onset = start;

	for(NotesIterator it = notes.begin(); it != notes.end(); it++)
	{
		AddNote(*it, onset);
		onset += it->duration;
	}
}

void NoteRenderer::SaveFile(std::string fileName)
{
	FileWvOut output;
		
	try {
		output.openFile(fileName, 1, FileWrite::FILE_WAV, Stk::STK_SINT16 );
	}
	catch ( StkError & ) {
		std::cout << "Error while opening the file";
	}

	output.tick(*frames);
}
