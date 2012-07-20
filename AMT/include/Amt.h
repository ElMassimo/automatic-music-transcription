
#ifndef __AMT_API__
#define __AMT_API__

#include <cstdlib>

struct Note 
{
	bool isRest;             
	short noteNumber; // Midi Note Value	
	int duration;
	
	inline Note(short noteNumber = 0, double duration = 0, bool isRest = false)		
		: isRest(isRest),
		noteNumber(noteNumber),
		duration(duration)
	{}

	inline ~Note(void)
	{
	}
};

#endif