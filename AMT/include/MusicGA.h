#pragma once
#include <ga/GASStateGA.h>
#include "NotesGenome.h"

namespace AMT {
	class MusicGA :	public GASteadyStateGA
	{
	public:
		MusicGA(NotesGenome& genome);
		~MusicGA();
	};
}