#include "MusicGA.h"
using namespace AMT;

MusicGA::MusicGA(NotesGenome& genome) : GASteadyStateGA(genome)
{
	this->minimize();
	GATournamentSelector selectionScheme;
	this->selector(selectionScheme);
}

MusicGA::~MusicGA()
{
}
