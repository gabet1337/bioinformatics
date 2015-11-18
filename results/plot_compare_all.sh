#!/usr/bin/gnuplot
set terminal pngcairo enhanced font 'Verdana,12'
set title "Running time"
set output 'running_time_all.png'
set xlabel "N (# sequences)"
set ylabel "time (ms)"
set style line 1 lc rgb '#0000FF' pt 5 ps 1 lt 1 lw 1
set style line 2 lc rgb '#00FF00' pt 4 ps 1 lt 1 lw 1
set style line 3 lc rgb '#FF0000' pt 8 ps 1 lt 1 lw 1
#set xtics autofreq
set grid ytics lc rgb "#bbbbbb" lw 1 lt 0
set grid xtics lc rgb "#bbbbbb" lw 1 lt 0
#set xrange [2**13:2**22]
#set logscale x 2
#set logscale y 2
#set format x "2^{%L}"
set key left top
plot "sn.dat" using 1:2 ls 1 with linespoints title 'Saitou-Nei naive openmp' ,\
"rnj.dat" using 1:2 ls 2 with linespoints title 'RapidNJ' ,\
"qt.dat" using 1:2 ls 3 with linespoints title 'QuickTree'

