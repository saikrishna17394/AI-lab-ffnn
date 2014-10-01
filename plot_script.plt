set terminal png
set output "plots/palin/palin_0.5_0.99.png"
set title "Error v/s No. of Iterations (eta = 0.5, mf =0.99)"
set ylabel "error"
set xlabel "iteration"
set datafile separator ' ' 
plot "out" using 1:2 smooth unique 


