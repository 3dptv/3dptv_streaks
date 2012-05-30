
% distFkt.m  Minimierungsfunktion zu Programm corMpt.m
%
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function F=distfkt(x,xc,yc,k1,k2,p1,p2) %Minimierungsfunktion zu distort-Funktion

F=[(x(1)+x(1)*(k1*(x(1)^2+x(2)^2)+k2*(x(1)^2+x(2)^2)^2)+p1*((x(1)^2+x(2)^2)+2*x(1)^2)+2*p2*x(1)*x(2)-xc)
   (x(2)+x(2)*(k1*(x(1)^2+x(2)^2)+k2*(x(1)^2+x(2)^2)^2)+p2*((x(1)^2+x(2)^2)+2*x(2)^2)+2*p1*x(1)*x(2)-yc)];
