set title "Perfil de Intensidade - Linha Central"
set xlabel "Coluna (X)"
set ylabel "Intensidade de Pixels"
set yrange [0:255]
plot 'line.txt' with lines title "Linha Central"