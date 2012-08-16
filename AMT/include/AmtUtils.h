#include <list>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include "MusicEvaluator.h"
#include "NotesGenome.h"
#include "NoteRenderer.h"
#include "MusicGA.h"
#include "MusicGA.h"

#define SAMPLE_NOTES 0
#define SAMPLE_CMAJOR 1
#define SAMPLE_TWONOTES 2
#define SAMPLE_ADAGIOSOSTENUTO 3

using namespace AMT;
using namespace std;

typedef bool (*Test)(void);

namespace AMT
{
	class AmtUtils {
	private:
		static ofstream TestResults;
		static void ExecuteTest(Test test);
		static bool AddNoteTest();
		static bool FlipSilenceTest();
		static bool GetNoteAtTest();
		static bool ChangePitchTest();
		static bool ChangeDurationTest();
		static bool ReplaceNotesTest();
		static bool CombineNotesTest();
		static bool SplitNoteTest();
		static bool MergeNotesTest();
		static bool CropAtTest();

	public:
		static void CreateSampleFile(int sampleNumber, int length = 8);
		static void CheckAndCreateDirectory(string directoryName);
		static void RunTests();
		static void OtherTests(Notes &notes);
		static void SaveArray(int n, double* input, string fileName);
		static void SaveAudio(Notes& notes, string fileName);
		static Notes GetSampleNotes();

		static float StandardDeviation(int n, float avg, vector<float> values);
	};
}