cd 'C:\AMT\AMT\AMT'
set title "Audio Samples and Frequency Magnitude"
set multiplot;                     
plot [-1:] 'magnitude.txt' using 1:2 with lines;
unset multiplot
