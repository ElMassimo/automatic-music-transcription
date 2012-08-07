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

ofstream AmtUtils::TestResults("Tests Results.txt");



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
	if(notes.size() != 2 || notes.totalDuration != note->duration * 2)
		return false;

	Note* note2 = new Note(189,22500);
	notes.AddNote(*note2);
	if(notes.size() != 3 || notes.totalDuration != note->duration * 2 + note2->duration)
		return false;
	
	return true;
}

// Changes a silence to a note, and vice versa
bool AmtUtils::FlipSilenceTest()
{
	out << "FlipSilenceTest: ";
	return true;
}

// Changes the pitch of a note
bool AmtUtils::ChangePitchTest()
{
	out << "ChangePitchTest: ";
	return true;
}

// Changes the duration of a note, may or not expand the note after (if there is one)
bool AmtUtils::ChangeDurationTest()
{
	out << "ChangeDurationTest: ";
	return true;
}

// Replaces this series of notes copying another one
bool AmtUtils::ReplaceNotesTest()
{
	out << "ReplaceNotesTest: ";
	return true;
}

// Combines two series of notes, copying the first part of one and the second part of the other
bool AmtUtils::CombineNotesTest()
{
	out << "CombineNotesTest: ";
	return true;
}

// Splits a note in two and inserts a silence in between
bool AmtUtils::SplitNoteTest()
{
	out << "SplitNoteTest: ";
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

void AmtUtils::RunTests()
{
	// Run the tests
	ExecuteTest(AddNoteTest);
	ExecuteTest(FlipSilenceTest);
	ExecuteTest(GetNoteAtTest);
	ExecuteTest(ChangePitchTest);
	ExecuteTest(ChangeDurationTest);
	ExecuteTest(ReplaceNotesTest);
	ExecuteTest(CombineNotesTest);
	ExecuteTest(SplitNoteTest);
	ExecuteTest(CropAtTest);
	
	Notes notes = GetSampleNotes();
	SaveAudio(notes, "Test");

	// Initialize the evaluator
	MusicEvaluator musicEvaluator;
	musicEvaluator.LoadAudioFile("Test.wav");

	// Create an ideal genome
	NotesGenome bestNoteEver(musicEvaluator);
	bestNoteEver.ReplaceNotes(notes);
	TestResults << "Ideal transcription fitness: " << bestNoteEver.evaluate();
	bestNoteEver.SaveToFile("test.txt");

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
	SaveArray(nFrames, input, "samples.txt");

	// Hann windowing
	for (int i = 0; i < nFrames; i++)
		input[i] *= HannWindow(i, nFrames);
	SaveArray(nFrames, input, "windowed.txt");

	// Calculate the FFT
	array1<Complex> output(nFrames/2 + 1);
	FourierTransform transform(nFrames, input, output);
	transform.fft(input, output);

	// Calculate the signal magnitude
	for(int i = 0; i < nFrames/2 + 1; i ++)
		input[i] = CalculateMagnitude(output[i]);
	SaveArray(nFrames/2 + 1, input, "magnitude.txt");

	// Calculate the signal magnitude with the music evaluator
	SaveArray(nFrames/2 + 1, musicEvaluator.frequencyMagnitudes[0], "magnitudeTest1.txt");
	SaveArray(nFrames/2 + 1, musicEvaluator.frequencyMagnitudes[11], "magnitudeTest2.txt");
	SaveArray(nFrames/2 + 1, musicEvaluator.frequencyMagnitudes[22], "magnitudeTest3.txt");
	SaveArray(nFrames/2 + 1, musicEvaluator.frequencyMagnitudes[34], "magnitudeTest4.txt");
	SaveArray(nFrames/2 + 1, musicEvaluator.frequencyMagnitudes[35], "magnitudeTest5.txt");
}
