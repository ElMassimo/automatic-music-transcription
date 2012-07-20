#ifndef __AMT_NOTE__
#define __AMT_NOTE__
#include <iostream>
using namespace std;

namespace AMT 
{
	class Note 
	{
	public:
		bool isRest;             
		short noteNumber; // Midi Note Value	
		int duration;

		inline Note(short noteNumber = 0, double duration = 0, bool isRest = false)		
			: isRest(isRest),
			noteNumber(noteNumber),
			duration(duration)
		{}

		inline Note(const Note &note)
			: isRest(note.isRest),
			noteNumber(note.noteNumber),
			duration(note.duration)
		{
		}

		inline ~Note(void)
		{
		}

		Note& operator=(const Note &note){
			isRest = note.isRest;
			noteNumber = note.noteNumber;
			duration = note.duration;
			return *this;
		}

		bool operator==(const Note &note){
			return isRest == note.isRest && noteNumber == note.noteNumber && duration == note.duration;
		}

		bool operator!=(const Note &note){
			return !(*this == note);
		}

		friend ostream& operator<<(ostream& output, const Note& note)
		{
			if(note.isRest)
				output << "Silence" << "\n";
			else
				output << "Note number: " << note.noteNumber << "\n";
			output << "Duration: " << note.duration << " samples\n\n";
			return output;
		}
	};
}

#endif