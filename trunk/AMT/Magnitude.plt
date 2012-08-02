cd 'C:\AMT\AMT\AMT'
set title "Audio Samples and Frequency Magnitude"
plot [-1:] 'magnitude.txt' using 1:2 with lines, \
			'magnitudeTest1.txt' using 1:2 with lines, \
			'magnitudeTest2.txt' using 1:2 with lines, \
			'magnitudeTest3.txt' using 1:2 with lines, \
			'magnitudeTest4.txt' using 1:2 with lines, \
			'magnitudeTest5.txt' using 1:2 with lines
