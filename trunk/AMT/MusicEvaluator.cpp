#include "MusicEvaluator.h"
#include "NoteRenderer.h"
#include "NotesGenome.h"

using namespace AMT;
using namespace std;

inline double HannWindow(int n, int N)
{
	return 0.5 * (1 - cos(2 * PI * n / (N - 1)));
}

inline double CalculateMagnitude(Complex &complex)
{
	return sqrt(pow(complex.real(), 2) + pow(complex.imag(), 2));
}

int MusicEvaluator::SizeInFrames( double samplesCount )
{
	return ceil(samplesCount / frameSize);
}

MusicEvaluator::MusicEvaluator()
{
	totalSamples = 0;
	binCount = 0;

	frameSize = 4096; // Size of the frames (in samples)
	overlapping = 0.0; // Overlapping percentage
	frameDiff = frameSize - (frameSize * overlapping); // Number of samples we have to slide to change the frame

	// Initialize what we need to perform the FFT
	fftResultSize = frameSize/2 + 1;
	int alignSize = sizeof(Complex);
	fftInput.Allocate(frameSize, alignSize);
	fftOutput.Allocate(fftResultSize, alignSize);
	forwardTransform = new FourierTransform(frameSize, fftInput, fftOutput);
}

MusicEvaluator::~MusicEvaluator()
{
	fftInput.Deallocate();
	fftOutput.Deallocate();
	if(forwardTransform != NULL)
		delete forwardTransform;
}

void MusicEvaluator::LoadAudioFile(string fileName)
{
	// Load the file we want to transcribe
	FileWvIn wavFile(fileName);

	totalSamples = wavFile.getSize();
	binCount = ceil(totalSamples / (double)frameDiff); // Number of bins that result from the Fourier transform

	// Read the whole file
	StkFrames audioData(SizeInFrames(totalSamples) * frameSize, 1);
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

		// 		stringstream fileName;
		// 		fileName << "Frequency " << t << ".txt";
		//		SaveArray(fftResultSize, frequencyMagnitudes[t], fileName.str());
	}
}

float MusicEvaluator::NoteErrorEvaluator(GAGenome & c)
{
	NotesGenome& genome = (NotesGenome&)c;
	double error = 1; // The difference between the original audio and the notes in the genome
	
	int rendererSize = SizeInFrames(totalSamples) * frameSize;
	NoteRenderer renderer(rendererSize);

	// Crop the series of notes, as we know the original audio's duration
	genome.CropAt(totalSamples);
	renderer.AddNotes(genome);

	// Perform the fft transform on the samples
	StkFloat* audioData = renderer.frames->getData();
	for (int t = 0; t < frequencyMagnitudes.Nx(); t++)
	{		
		int startFrame = t * frameSize;
		// Here we apply a Hann window to the input
		for (int i = 0; i < frameSize; i++)
			fftInput[i] = audioData[startFrame + i] * HannWindow(i, frameSize);

		// Calculate the FFT
		forwardTransform->fft(fftInput, fftOutput);

		// Calculate frequency magnitudes for each time bin, and obtain the difference with the original audio source
		for(int i = 0; i < fftResultSize; i++)
			error += abs(CalculateMagnitude(fftOutput[i]) - frequencyMagnitudes[t][i]);
	}
	return error;
}

int MusicEvaluator::TotalSamples()
{
	return totalSamples;
}

int MusicEvaluator::FrameSize()
{
	return frameSize;
}
