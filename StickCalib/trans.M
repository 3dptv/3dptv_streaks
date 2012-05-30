
% trans.m  Führt einen Vektor t eines Vektorprodukts t'xr in die Matrix Tr schreibweise über.
% vec (3x1) Vektor
% T (3x3) Matrix
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function T=trans(vec);
T=[0,-vec(3),vec(2);vec(3),0,-vec(1);-vec(2),vec(1),0];