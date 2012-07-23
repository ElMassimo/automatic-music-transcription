#include "AmtRenderer.h"
using namespace AMT;

inline long SecondToFrames(double seconds)
{
	return seconds * Stk::sampleRate();
}

AmtRenderer::AmtRenderer(unsigned int samples)
{
	frames = new StkFrames(samples, 1);
}

AmtRenderer::~AmtRenderer()
{
	delete frames;
}

StkFloat* AmtRenderer::noteFrequency = new StkFloat[128];

void AmtRenderer::Initialize()
{
	for(int i = 0; i < 128; i++)
		noteFrequency[i] = pow(2,(double)(i-57)/12) * 440;
}

void AmtRenderer::CleanUp()
{
	delete [] noteFrequency;
}

void AmtRenderer::AddNote(Note &note, int start)
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

void AmtRenderer::AddNotes(Notes &notes, int start)
{
	int onset = start;

	for(NotesIterator it = notes.begin(); it != notes.end(); it++)
	{
		AddNote(*it, onset);
		onset += it->duration;
	}
}

void AmtRenderer::SaveFile(std::string fileName)
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
