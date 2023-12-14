set terminal png size 800,600
set output "OUTPUT_FILE"

set xlabel "Eigenvalue"
set ylabel "Magnitude"

set logscale y 
set grid

plot "DATA_FILE" using ($0+1):(sqrt($1**2 + $2**2)) with points pt 3 ps 4 title 'Eigenvalue'

