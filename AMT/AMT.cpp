#include <list>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include "MusicEvaluator.h"
#include "NotesGenome.h"
#include "NoteRenderer.h"
#include "MusicGA.h"

using namespace AMT;
using namespace std;

void SaveArray(int n, double* input, string fileName){
	ofstream file(fileName);
	if(file)
	{
		for(int i = 0; i < n; i ++)
			file << i << " " << input[i] << "\n";
		file << endl;
	}
}

void SaveAudio(Notes& notes, string fileName){
	NoteRenderer renderer(notes.totalDuration);
	renderer.AddNotes(notes);
	renderer.SaveFile(fileName);
}

void Test();

int	main(int argc, char** argv) {
	Test();
	// GARandomSeed();
	
	// Initialize the music evaluator
	MusicEvaluator musicEvaluator;
	musicEvaluator.LoadAudioFile("Test.wav");

	// Create the genome and the genetic algorithm instance
	NotesGenome notesGenome(musicEvaluator);
	MusicGA ga(notesGenome);
	
	// Set the default parameters we want to use, then check the command line for
	// other arguments that might modify these.
	ga.set(gaNpopulationSize, 50);	// population size
	ga.set(gaNnReplacement, 50); // number of replacement
	ga.set(gaNpCrossover, 0.6);		// probability of crossover
	ga.set(gaNpMutation, 0.1);		// probability of mutation
	ga.set(gaNnGenerations, 10);	// number of generations
	ga.set(gaNscoreFrequency, 1);	// how often to record scores
	ga.set(gaNflushFrequency, 10);	// how often to dump scores to file
	ga.set(gaNselectScores,		// which scores should we track?
		GAStatistics::Maximum|GAStatistics::Minimum|GAStatistics::Mean);
	ga.set(gaNscoreFilename, "bog.dat");
	ga.parameters(argc, argv);

	// Calculate the time of evolution
	time_t evolutionStart, evolutionEnd;
	time (&evolutionStart);

	// Evolve the genetic algorithm then dump out the results of the run.
	ga.evolve();
	
	time (&evolutionEnd);

	NotesGenome& best = (NotesGenome&) ga.statistics().bestIndividual();
	best.SaveToFile("best.txt");
	SaveAudio(best,"Best");

	cout << "the ga generated the list:\n" << best << "\n";
	cout << "the ga used the parameters:\n" << ga.parameters() << "\n";	
	cout << "\n" << ga.statistics() << "\n";
	cout << "\nFitness of the best individual: " << best.evaluate();
	cout << "\n\nRunning time: " << difftime (evolutionEnd, evolutionStart) << "s";


	//for(int i = 0; i < ga.populationSize(); i++)
	//{
	//	NotesGenome& individual = (NotesGenome&) ga.population().individual(i);
	//	NoteRenderer rendererIndividual(individual.totalDuration);	//	rendererIndividual.AddNotes(individual);
	//	stringstream fileName;
	//	fileName << "Individual " << i;
	//	rendererIndividual.SaveFile(fileName.str());
	//}
	
	return 0;
}

inline double HannWindow(int n, int N)
{
	return 0.5 * (1 - cos(2 * PI * n / (N - 1)));
}

inline double CalculateMagnitude(Complex &complex)
{
	return sqrt(pow(complex.real(), 2) + pow(complex.imag(), 2));
}

void Test() {
	// Create an example audio file
	Notes notes;
	notes.AddNote(Note(57,44100));
	notes.AddNote(Note(59,44100));
	notes.AddNote(Note(60,44100));
	notes.AddNote(Note(59,4096));
	notes.AddNote(Note(57,22100));
	notes.AddNote(Note(59,4096));
	notes.AddNote(Note(60,10500));
	SaveAudio(notes, "Test");

	// Initialize the evaluator
	MusicEvaluator musicEvaluator;
	musicEvaluator.LoadAudioFile("Test.wav");

	// Create an ideal genome
	NotesGenome bestNoteEver(musicEvaluator);
	bestNoteEver.ReplaceNotes(notes);
	cout << "Ideal transcription fitness: " << bestNoteEver.evaluate();
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

