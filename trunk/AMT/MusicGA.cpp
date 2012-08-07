#include "MusicGA.h"
using namespace AMT;

MusicGA::MusicGA(NotesGenome& genome) : GASteadyStateGA(genome)
{
	this->minimize();
}

MusicGA::~MusicGA()
{
}
