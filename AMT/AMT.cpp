#include <list>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <ga/std_stream.h>
#include <ga/GASStateGA.h>
#include <fftw++.h>
#include <Array.h>
#include <FileWvIn.h>
#include "NoteRenderer.h"
#include "NotesGenome.h"

#define cout STD_COUT
#define endl STD_ENDL
#define FourierTransform fftwpp::rcfft1d

using namespace AMT;
using Array::array1;
using Array::array2;

array2<double> frequencyMagnitudes;
FourierTransform* forwardTransform;

inline double HannWindow(int n, int N)
{
	return 0.5 * (1 - cos(2 * PI * n / (N - 1)));
}

inline double CalculateMagnitude(Complex &complex)
{
	return sqrt(pow(complex.real(), 2) + pow(complex.imag(), 2));
}

float Objective(GAGenome & c)
{
	NotesGenome& genome = (NotesGenome&)c;
	return 100 / (genome.size() + 1);
}



void GA(int argc, char** argv) {

	// 	NotesGenome test;
	// 	NotesGenome::Init(test);
	// 	cout << "Before:\n" << test;
	// 	Note* newNote = new Note(55, 50000);
	// 	test.AddNote(0, *newNote);
	// 	newNote = new Note(0, 500000, true);
	// 	test.AddNote(1, *newNote);
	// 	cout << "\nAfter new notes:\n" << test;
	// 	test.EraseNote(1);
	// 	cout << "\nAfter erase silence:\n" << test;
	// 	test.SplitNote(0,0.5,0.01);
	// 	cout << "\nAfter splitting note:\n" << test;

	// GARandomSeed();

	// Create the initial genome for the genetic algorithm to use.
	// It's necessary to set the original audio FFT's
	NotesGenome notesGenome;
	notesGenome.evaluator(Objective);

	// Now that we have a genome, we use it to create our GA.
	GASteadyStateGA ga(notesGenome);
	//ga.populationSize(30);
	//ga.pReplacement(1);

	// Set the default parameters we want to use, then check the command line for
	// other arguments that might modify these.
	ga.set(gaNpopulationSize, 200);	// population size
	ga.set(gaNpCrossover, 0.6);		// probability of crossover
	ga.set(gaNpMutation, 0.1);		// probability of mutation
	ga.set(gaNnGenerations, 1);	// number of generations
	ga.set(gaNscoreFrequency, 1);	// how often to record scores
	ga.set(gaNflushFrequency, 10);	// how often to dump scores to file
	ga.set(gaNselectScores,		// which scores should we track?
		GAStatistics::Maximum|GAStatistics::Minimum|GAStatistics::Mean);
	ga.set(gaNscoreFilename, "bog.dat");
	ga.parameters(argc, argv);

	// Evolve the genetic algorithm then dump out the results of the run.
	ga.evolve();

	NotesGenome& best = (NotesGenome&) ga.statistics().bestIndividual();

	cout << "the ga generated the list:\n" << best << "\n";
	cout << "the list contains " << best.size() << " nodes\n";
	cout << "the ga used the parameters:\n" << ga.parameters() << "\n";	
	cout << "\n" << ga.statistics() << "\n";

	NoteRenderer::Initialize();	
	NoteRenderer renderer(best.totalDuration);
	renderer.AddNotes(best);
	renderer.SaveFile("Best");

	// 	for(int i = 0; i < ga.populationSize(); i++)
	// 	{
	// 		NotesGenome& individual = (NotesGenome&) ga.population().individual(i);
	// 		NoteRenderer rendererIndividual(individual.totalDuration);
	// 		rendererIndividual.AddNotes(individual);
	// 		stringstream fileName;
	// 		fileName << "Individual " << i;
	// 		rendererIndividual.SaveFile(fileName.str());
	// 	}

	Notes notes;
	notes.push_front(*(new Note(57,44100)));
	NoteRenderer renderer3(44100);
	renderer3.AddNotes(notes);
	renderer3.SaveFile("Test");
	NoteRenderer::CleanUp();
}

int	main2(int argc, char** argv) {
		
	int frameSize = 4096; // Size of the frames (in samples)
	double overlapping = 0.0; // Overlapping percentage
	int frameDiff = frameSize - (frameSize * overlapping); // Number of samples we have to slide to change the frame

	// Load the file we want to transcribe
	FileWvIn wavFile("Test.wav");

	int totalSamples = wavFile.getSize();
	int binCount = ceil(totalSamples / (double)frameDiff); // Number of bins that result from the Fourier transform

	// Initialize what we need to perform the FFT
	int fftResultSize = frameSize/2 + 1;
	int alignSize = sizeof(Complex);
	array1<double> fftInput(frameSize, alignSize);
	array1<Complex> fftOutput(fftResultSize, alignSize);
	forwardTransform = new FourierTransform(frameSize, fftInput, fftOutput);

	// Read the whole file
	StkFrames audioData(totalSamples, 1);
	wavFile.tick(audioData);	

	// Perform the fft transform on the samples
	frequencyMagnitudes.Allocate(binCount, fftResultSize);
	for (int t = 0; t < binCount; t++)
	{		
		int startFrame = t * frameDiff;
		// Here we apply a Hann window to the input
		for (int i = 0; i < frameSize; i++)
			fftInput[i] = audioData[startFrame + i] * HannWindow(i, frameSize);

		// Calculate the FFT
		forwardTransform->fft(fftInput, fftOutput);

		// Load the frequency magnitudes for each time bin
		for(int i = 0; i < fftResultSize; i++)
			frequencyMagnitudes[t][i] = CalculateMagnitude(fftOutput[i]);			
	}
	
	return 0;
}

void SaveArray(int n, double* input, string fileName){
	ofstream file(fileName);
	if(file)
	{
		for(int i = 0; i < n; i ++)
			file << i << " " << input[i] << "\n";
		file << endl;
	}
}

void Test() {
	// Create a test genome
	NotesGenome genome;
	NotesGenome::Init(genome);
	genome.SaveToFile("notes.txt");

	// Create an audio file representing the genome
	NoteRenderer renderer(genome.totalDuration);
	renderer.AddNotes(genome);
	renderer.SaveFile("Test");
		
	int nFrames = 4096;
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
}

