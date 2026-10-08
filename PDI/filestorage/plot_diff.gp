set title "Diferença do Perfil de Linha: YML (32F) vs PNG (8U)"
set xlabel "Coluna (X)"
set ylabel "Diferença Absoluta |YML - PNG|"
set grid
set yrange [0:1]
plot 'diferenca_linha.txt' using 1:4 with lines title "Erro de Quantização"