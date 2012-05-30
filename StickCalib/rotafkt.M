
% rotaFkt.m Minimierungsfunktion zu Programm defwink.m
%
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


function F=rotafkt(w,R)
F=[R-rota(w(1),w(2),w(3))];