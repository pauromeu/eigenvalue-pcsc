set terminal png size 800,600
set output "OUTPUT_FILE"

set xlabel "Re"
set ylabel "Im"

set grid

plot "DATA_FILE" using 1:2 with points pt 3 ps 4 title 'Eigenvalue'
