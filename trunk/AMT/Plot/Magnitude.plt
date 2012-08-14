cd 'C:\AMT\AMT\AMT\Plot'
set title "Audio Samples and Frequency Magnitude"
plot [-1:] 'magnitude.txt' using 1:2 with impulses, \
			'magnitudeTest1.txt' using 1:2 with impulses, \
			'magnitudeTest2.txt' using 1:2 with impulses, \
			'magnitudeTest3.txt' using 1:2 with impulses, \
			'magnitudeTest4.txt' using 1:2 with impulses, \
			'magnitudeTest5.txt' using 1:2 with impulses
