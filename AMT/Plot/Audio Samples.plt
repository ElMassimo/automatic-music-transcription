cd 'C:\AMT\AMT\AMT\Plot'
set title "Audio Samples and Frequency Magnitude"
set autoscale
set multiplot;                     
plot 'samples.txt' using 1:2 with lines,\
	'windowed.txt' using 1:2 with lines;
unset multiplot
