cd 'C:\AMT\AMT\AMT\Plot'
set title "Fitness evolution"
set autoscale
set multiplot;                     
plot '200 gen.txt' using 1:2 title "Average" with lines ,\
	'200 gen.txt' using 1:3 title "Best individual" with lines,\
	'200 gen alt.txt' using 1:2 title "Average" with lines ,\
	'200 gen alt.txt' using 1:3 title "Best individual" with lines;
unset multiplot
