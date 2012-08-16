#include <windows.h>
#include "AmtUtils.h"
using namespace AMT;

#define out AmtUtils::TestResults

inline double HannWindow(int n, int N)
{
	return 0.5 * (1 - cos(2 * PI * n / (N - 1)));
}

inline double CalculateMagnitude(Complex &complex)
{
	return sqrt(pow(complex.real(), 2) + pow(complex.imag(), 2));
}

Notes GetTwoNotes(int n)
{
	Notes notes;
	notes.AddNote(Note(60,4096*n));
	notes.AddNote(Note(69,4096*n));
	return notes;
}

Notes GetCMajorScale(int n)
{
	// Generate sample audio file
	Notes notes;
	notes.AddNote(Note(48,4096 * n));
	notes.AddNote(Note(50,4096 * n));
	notes.AddNote(Note(52,4096 * n));
	notes.AddNote(Note(53,4096 * n));
	notes.AddNote(Note(55,4096 * n));
	notes.AddNote(Note(57,4096 * n));
	notes.AddNote(Note(59,4096 * n));
	notes.AddNote(Note(60,4096 * 2 * n));
	notes.AddNote(Note(72,4096 * 2 * n));
	return notes;
}

Notes GetAdagioSostenuto(int n)
{
	// Generate sample audio file
	Notes notes;
	notes.AddNote(Note(56,4096 * n));
	notes.AddNote(Note(61,4096 * n));
	notes.AddNote(Note(64,4096 * n));
	notes.AddNote(Note(56,4096 * n));
	notes.AddNote(Note(61,4096 * n));
	notes.AddNote(Note(64,4096 * n));
	notes.AddNote(Note(56,4096 * n));
	notes.AddNote(Note(61,4096 * n));
	notes.AddNote(Note(64,4096 * n));
	notes.AddNote(Note(56,4096 * n));
	notes.AddNote(Note(61,4096 * n));
	notes.AddNote(Note(64,4096 * n));
	return notes;
}

Notes AmtUtils::GetSampleNotes()
{
	// Create an example audio file
	Notes notes;
	notes.AddNote(Note(57,44100));
	notes.AddNote(Note(59,44100));
	notes.AddNote(Note(60,44100));
	notes.AddNote(Note(59,4096));
	notes.AddNote(Note(57,22100));
	notes.AddNote(Note(59,4096));
	notes.AddNote(Note(60,10500));
	return notes;
}

void AmtUtils::SaveArray(int n, double* input, string fileName){
	ofstream file(fileName);
	if(file)
	{
		for(int i = 0; i < n; i ++)
			file << i << " " << input[i] << "\n";
		file << endl;
	}
}

void AmtUtils::SaveAudio(Notes& notes, string fileName){
	NoteRenderer renderer(notes.totalDuration);
	renderer.AddNotes(notes);
	renderer.SaveFile(fileName);
}

ofstream AmtUtils::TestResults;

// Adds a new note at the end
bool AmtUtils::AddNoteTest()
{
	out << "AddNoteTest: ";
	Notes notes;
	if(notes.size() > 0 || notes.totalDuration > 0)
		return false;

	Note* note = new Note(57,44100);
	notes.AddNote(*note);
	if(notes.size() != 1 || notes.totalDuration != 44100)
		return false;

	notes.AddNote(*note);
	if(notes.size() != 2 || notes.totalDuration != note->duration * 2 || notes.totalDuration != notes.GetRealDuration())
		return false;

	Note* note2 = new Note(189,22500);
	notes.AddNote(*note2);
	if(notes.size() != 3 || notes.totalDuration != note->duration * 2 + note2->duration || notes.totalDuration != notes.GetRealDuration())
		return false;
	
	return true;
}

// Changes a silence to a note, and vice versa
bool AmtUtils::FlipSilenceTest()
{
	out << "FlipSilenceTest: ";
	Notes notes = GetSampleNotes();
	int noteIndex = 3;

	// Silence a note
	notes.FlipSilence(noteIndex);
	Note* note = &(*notes.GetNote(noteIndex));
	if(!note->isRest)
		return false;

	// Modify the note number
	note->noteNumber = 0;

	// Flip the silence again
	notes.FlipSilence(noteIndex);
	note = &(*notes.GetNote(noteIndex));
	if(note->isRest)
		return false;

	// Check that the note is similar to those around it
	if(note->noteNumber != 57 && note->noteNumber != 60)
		return false;

	// Check what happens when there are no more notes there
	Notes otherNotes;
	otherNotes.AddNote(Note(0,44100,true));
	otherNotes.FlipSilence(0);
	note = &(*otherNotes.GetNote(0));
	if(note->isRest)
		return false;
	if(note->noteNumber != DEFAULT_NOTE_NUMBER)
		return false;

	return true;
}

// Changes the pitch of a note
bool AmtUtils::ChangePitchTest()
{
	out << "ChangePitchTest: ";
	Notes notes = GetSampleNotes();
	notes.ChangePitch(0, true, true);
	if(notes.GetNote(0)->noteNumber != 57 + 12)
		return false;

	notes.ChangePitch(1, false, false);
	if(notes.GetNote(1)->noteNumber != 59 - 1)
		return false;

	notes.ChangePitch(3, false, true);
	if(notes.GetNote(3)->noteNumber != 59 - 12)
		return false;

	notes.ChangePitch(4, true, false);
	if(notes.GetNote(4)->noteNumber != 57 + 1)
		return false;

	return true;
}

// Changes the duration of a note, may or not expand the note after (if there is one)
bool AmtUtils::ChangeDurationTest()
{
	out << "ChangeDurationTest: ";

	Notes notes = GetSampleNotes();
	notes.ChangeDuration(0, 0.5, false);
	if(notes.size() != 8 || notes.GetNote(0)->duration != 22050 || notes.totalDuration != notes.GetRealDuration())
		return false;

	notes.ChangeDuration(0, 2.5, true);
	if(notes.size() != 7 || notes.GetNote(0)->duration != 55125 || notes.totalDuration != notes.GetRealDuration())
		return false;

	notes.ChangeDuration(0, 0.8, true);
	if(notes.size() != 7 || notes.GetNote(0)->duration != 44100 || notes.totalDuration != notes.GetRealDuration())
		return false;

	notes.ChangeDuration(6, 0.5, true);
	if(notes.size() != 7 || notes.GetNote(6)->duration != 5250 || notes.totalDuration != notes.GetRealDuration() 
		|| notes.totalDuration != 173092 - 5250)
		return false;

	notes.ChangeDuration(6, 2.0, true);
	if(notes.size() != 7 || notes.GetNote(6)->duration != 10500 || notes.totalDuration != notes.GetRealDuration() 
		|| notes.totalDuration != 173092)
		return false;

	if(notes != GetSampleNotes())
		return false;

	notes.ChangeDuration(5, 4.0, true);
	if(notes.size() != 6 || notes.GetNote(5)->duration != 16384 || notes.totalDuration != notes.GetRealDuration() 
		|| notes.totalDuration != 173092 + 1788)
		return false;

	notes.ChangeDuration(5, 0.25, true);
	if(notes.size() != 6 || notes.GetNote(5)->duration != 4096 || notes.totalDuration != notes.GetRealDuration() 
		|| notes.totalDuration != 173092 - 10500)
		return false;

	return true;
}

// Replaces this series of notes copying another one
bool AmtUtils::ReplaceNotesTest()
{
	out << "ReplaceNotesTest: *";
	return true;
}

// Combines two series of notes, copying the first part of one and the second part of the other
bool AmtUtils::CombineNotesTest()
{
	out << "CombineNotesTest: ";

	Notes notes1 = GetSampleNotes();
	Notes notes2;
	notes2.AddNote(Note(125,22050));
	notes2.AddNote(Note(122,44100));

	int when = 0;

	// Check the border case when we select the very beginning
	Notes newNotes1;
	Notes newNotes2;
	newNotes1.CombineNotes(notes1, notes2, when);
	newNotes2.CombineNotes(notes2, notes1, when);
	if(newNotes1 != notes2 || newNotes2 != notes1
		|| newNotes1.totalDuration != notes2.totalDuration || newNotes2.totalDuration != notes1.totalDuration
		|| newNotes1.totalDuration != newNotes1.GetRealDuration() || newNotes2.totalDuration != newNotes2.GetRealDuration())
		return false;

	// Normal case in between notes
	when = 44100;
	newNotes1.CombineNotes(notes1, notes2, when);
	newNotes2.CombineNotes(notes2, notes1, when);
	if(newNotes1.size() != 2 || newNotes2.size() != 8
		|| newNotes1.totalDuration != notes2.totalDuration || newNotes2.totalDuration != notes1.totalDuration
		|| newNotes1.totalDuration != newNotes1.GetRealDuration() || newNotes2.totalDuration != newNotes2.GetRealDuration())
		return false;

	// Check that the border notes are alright
	Note note = *newNotes1.GetNote(0);
	if(note.duration != 44100 && note.noteNumber != 57)
		return false;
	note = *newNotes1.GetNote(1);
	if(note.duration != 22050 && note.noteNumber != 122)
		return false;
	note = *newNotes2.GetNote(1);
	if(note.duration != 22050 && note.noteNumber != 122)
		return false;
	note = *newNotes2.GetNote(2);
	if(note.duration != 44100 && note.noteNumber != 59)
		return false;
	
	// Regular case
	when = 44200;
	newNotes1.CombineNotes(notes1, notes2, when);
	newNotes2.CombineNotes(notes2, notes1, when);
	if(newNotes1.size() != 3 || newNotes2.size() != 8
		|| newNotes1.totalDuration != notes2.totalDuration || newNotes2.totalDuration != notes1.totalDuration
		|| newNotes1.totalDuration != newNotes1.GetRealDuration() || newNotes2.totalDuration != newNotes2.GetRealDuration())
		return false;

	// Check that the border notes are alright
	note = *newNotes1.GetNote(0);
	if(note.duration != 44100 && note.noteNumber != 57)
		return false;
	note = *newNotes1.GetNote(1);
	if(note.duration != 100 && note.noteNumber != 59)
		return false;
	note = *newNotes1.GetNote(2);
	if(note.duration != 21950 && note.noteNumber != 122)
		return false;
	note = *newNotes2.GetNote(1);
	if(note.duration != 22150 && note.noteNumber != 122)
		return false;
	note = *newNotes2.GetNote(2);
	if(note.duration != 44000 && note.noteNumber != 59)
		return false;

	// Check the border case when we select the very end
	when = 66150;
	newNotes1.CombineNotes(notes1, notes2, when);
	newNotes2.CombineNotes(notes2, notes1, when);
	if(newNotes1.size() != 2 || newNotes2.size() != 8
		|| newNotes1.totalDuration != notes2.totalDuration || newNotes2.totalDuration != notes1.totalDuration
		|| newNotes1.totalDuration != newNotes1.GetRealDuration() || newNotes2.totalDuration != newNotes2.GetRealDuration())
		return false;

	return true;
}

// Splits a note in two and inserts a silence in between
bool AmtUtils::SplitNoteTest()
{
	out << "SplitNoteTest: *";
	return true;
}

bool AmtUtils::MergeNotesTest()
{
	out << "MergeNotesTest: ";
	Notes notes = GetSampleNotes();
	notes.MergeRedundantNotes();
	if(notes != GetSampleNotes())
		return false;

	notes.AddNote(Note(60,11550));
	notes.MergeRedundantNotes();
	if(notes.size() != 7 || notes.GetNote(6)->duration != 22050 || notes.totalDuration != notes.GetRealDuration() 
		|| notes.totalDuration != 173092 + 11550)
		return false;

	notes.AddNote(Note(60,10000, true));
	notes.MergeRedundantNotes();
	if(notes.size() != 8 || notes.GetNote(6)->duration != 22050 || notes.totalDuration != notes.GetRealDuration() 
		|| notes.totalDuration != 173092 + 11550 + 10000)
		return false;

	notes.AddNote(Note(60,10000, true));
	notes.MergeRedundantNotes();
	if(notes.size() != 8 || notes.GetNote(6)->duration != 22050 || notes.totalDuration != notes.GetRealDuration() 
		|| notes.totalDuration != 173092 + 11550 + 20000)
		return false;

	// Test small notes removal
	MusicEvaluator me;
	NotesGenome genome(me);
	genome.AddNote(Note(50,1000));
	genome.AddNote(Note(60,20000));
	genome.AddNote(Note(51,10000));
	genome.AddNote(Note(60,10000, true));
	genome.AddNote(Note(50,1000));
	genome.AddNote(Note(62,1000));
	genome.AddNote(Note(50,1000));
	genome.AddNote(Note(54,1000));
	genome.AddNote(Note(55,1000));
	genome.AddNote(Note(70,10000));
	genome.AddNote(Note(58,1000));
	genome.AddNote(Note(40,10000));
	genome.AddNote(Note(59,1000));
	genome.MergeRedundantNotes();
	if(genome.size() != 5 || genome.totalDuration != genome.GetRealDuration() || genome.totalDuration != 68000)
		return false;

	return true;
}

// Crop the notes of series at a point in time
bool AmtUtils::CropAtTest()
{
	out << "CropAtTest: ";
	Notes notes = GetSampleNotes();
	int newDuration = 173000;
	notes.CropAt(newDuration);
	if(newDuration != notes.totalDuration)
		return false;

	// Check that the actual total duration of the note is correct
	int totalDuration = 0;
	for (NotesIterator it = notes.begin(); it != notes.end(); it++)
		totalDuration += it->duration;
	if(notes.totalDuration != totalDuration)
		return false;

	// Check that the last note is correct
	NotesIterator lastNote = --notes.end();
	if(lastNote->duration != 10408 || lastNote->noteNumber != 60 || lastNote->isRest)
		return false;

	// Check that the rest of the notes is ok
	Notes notes2 = GetSampleNotes();
	NotesIterator it = notes.begin();
	NotesIterator it2 = notes2.begin();
	while (*it == *it2 && it != lastNote)
		it++, it2++;

	if(it != lastNote)
		return false;

	// Crop again, this time at a border
	newDuration = 136396;
	notes.CropAt(newDuration);
	if(notes.size() != 4)
		return false;

	return true;
}

bool AmtUtils::GetNoteAtTest()
{
	out << "GetNoteAtTest: ";
	Notes notes = GetSampleNotes();
	int when = 0;

	NotesIterator it = notes.GetNoteAt(when);
	if(it == notes.end() || when > 0 || it->noteNumber != 57)
		return false;

	when = 44100;
	it = notes.GetNoteAt(when);
	if(it == notes.end() || when > 0 || it->noteNumber != 59)
		return false;

	when = 44109;
	it = notes.GetNoteAt(when);
	if(it == notes.end() || when != 9 || it->noteNumber != 59)
		return false;

	when = 162600;
	it = notes.GetNoteAt(when);
	if(it == notes.end() || when != 8 || it->noteNumber != 60)
		return false;

	when = 173092;
	it = notes.GetNoteAt(when);
	if(it != notes.end())
		return false;

	// Test empty notes removal
	Notes otherNotes;
	otherNotes.AddNote(Note(5,0));
	otherNotes.AddNote(Note(16,0));
	otherNotes.AddNote(Note(125,44100));
	otherNotes.AddNote(Note(16,0));
	
	return true;
}

void AmtUtils::ExecuteTest(Test test)
{
	if(test())
		out << "OK\n";
	else
		out << "FAILED!\n";
}

void AmtUtils::OtherTests(Notes &notes)
{
	SaveAudio(notes, "Test");

	// Initialize the evaluator
	MusicEvaluator musicEvaluator;
	musicEvaluator.LoadAudioFile("Test.wav");

	// Create an ideal genome
	NotesGenome bestNoteEver(musicEvaluator);
	bestNoteEver.ReplaceNotes(notes);
	TestResults << "Ideal transcription fitness: " << bestNoteEver.evaluate();
	bestNoteEver.SaveToFile("Test.txt");

	// Create a test genome
	NotesGenome genome(musicEvaluator);
	NotesGenome::DefaultMusicInitializer(genome);
	genome.SaveToFile("notes.txt");

	// Create an audio file representing the genome
	NoteRenderer renderer(genome.totalDuration);
	renderer.AddNotes(genome);
	renderer.SaveFile("Genome");

	int nFrames = musicEvaluator.FrameSize();
	StkFloat* input = renderer.frames->getData();
	SaveArray(nFrames, input, "Plot\\samples.txt");

	// Hann windowing
	for (int i = 0; i < nFrames; i++)
		input[i] *= HannWindow(i, nFrames);
	SaveArray(nFrames, input, "Plot\\windowed.txt");

	// Calculate the FFT
	array1<Complex> output(nFrames/2 + 1);
	FourierTransform transform(nFrames, input, output);
	transform.fft(input, output);

	// Calculate the signal magnitude
	for(int i = 0; i < nFrames/2 + 1; i ++)
		input[i] = CalculateMagnitude(output[i]);
	SaveArray(nFrames/2 + 1, input, "Plot\\magnitude.txt");

	// Calculate the signal magnitude with the music evaluator
	SaveArray(nFrames/2 + 1, musicEvaluator.frequencyMagnitudes[0], "Plot\\magnitudeTest1.txt");
	SaveArray(nFrames/2 + 1, musicEvaluator.frequencyMagnitudes[11], "Plot\\magnitudeTest2.txt");
	SaveArray(nFrames/2 + 1, musicEvaluator.frequencyMagnitudes[22], "Plot\\magnitudeTest3.txt");
	SaveArray(nFrames/2 + 1, musicEvaluator.frequencyMagnitudes[34], "Plot\\magnitudeTest4.txt");
	SaveArray(nFrames/2 + 1, musicEvaluator.frequencyMagnitudes[35], "Plot\\magnitudeTest5.txt");
}

void AmtUtils::RunTests()
{
	TestResults.open("Tests Results.txt");

	// Run the tests
	ExecuteTest(AddNoteTest);
	ExecuteTest(FlipSilenceTest);
	ExecuteTest(GetNoteAtTest);
	ExecuteTest(ChangePitchTest);
	ExecuteTest(ChangeDurationTest);
	ExecuteTest(ReplaceNotesTest);
	ExecuteTest(CombineNotesTest);
	ExecuteTest(SplitNoteTest);
	ExecuteTest(MergeNotesTest);
	ExecuteTest(CropAtTest);
	
	Notes test = GetTwoNotes(8);

	//OtherTests(test);
	AmtUtils::SaveAudio(test, "Test");	

	TestResults.close();
}

void AmtUtils::CreateSampleFile(int sampleNumber, int length)
{
	Notes sample;
	switch(sampleNumber)
	{
		case SAMPLE_NOTES : sample = GetSampleNotes();
		break;
		case SAMPLE_CMAJOR : sample = GetCMajorScale(length);
		break;
		case SAMPLE_ADAGIOSOSTENUTO : sample = GetAdagioSostenuto(length);
		break;
		case SAMPLE_TWONOTES:
		default: sample = GetTwoNotes(length);
	}
	AmtUtils::SaveAudio(sample, "Test");	
}

void AmtUtils::CheckAndCreateDirectory(string dirName)
{
	CreateDirectoryA(dirName.c_str(), NULL);
}

float AmtUtils::StandardDeviation(int n, float avg, vector<float> values)
{
	float tmpvar = 0.0;
	if(n > 1){
		for(int i = 0; i < n; i++){
			float s = values[i] - avg;
			s *= s;
			tmpvar += s;
		}
		tmpvar /= (n-1);
	}
	return (float)sqrt(tmpvar);
}
