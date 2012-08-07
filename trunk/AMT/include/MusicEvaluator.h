#pragma once
#include <fftw++.h>
#include <Array.h>
#include <FileWvIn.h>
#include <ga/GAGenome.h>

#define FourierTransform fftwpp::rcfft1d

using Array::array1;
using Array::array2;

namespace AMT {

	class MusicEvaluator
	{
	private:
		// Analysis of the signal
		int frameSize; // Size of the frames (in samples)
		double overlapping; // Overlapping percentage
		int frameDiff; // Number of samples we have to slide to change the frame

		// FFT-related
		int fftResultSize;
		array1<double> fftInput;
		array1<Complex> fftOutput;
		FourierTransform* forwardTransform; // FFTW Fourier transform

		// Original audio
		int binCount; // Number of bins that result from the Fourier transform
		int totalSamples; // Number of samples of the original audio
		
	public:
		array2<double> frequencyMagnitudes; // Bins of frequency magnitudes of the original audio

		MusicEvaluator();
		~MusicEvaluator(void);

		void LoadAudioFile(std::string fileName); // Loads the audio file to be transcribed
		float NoteErrorEvaluator(GAGenome& c); // Objective function
		int TotalSamples();
		int FrameSize();

		int SizeInFrames(double samplesCount);
	};

}