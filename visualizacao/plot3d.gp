set title "Perfil 3D de Intensidades Luminosas"
set xlabel "Colunas (X)"
set ylabel "Linhas (Y)"
set zlabel "Intensidade"
set hidden3d
set dgrid3d 100,100
set pm3d
splot 'matriz.txt' matrix with lines