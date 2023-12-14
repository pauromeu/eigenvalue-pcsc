set terminal png size 800,600
set output results/dum3_spectrum.png

set xlabel "Re"
set ylabel "Im"

set style circle radius graph 0.01
set grid

plot results/dum3_eigenvalues.dat using 1:2 with circles fill solid lc rgb 'black'
