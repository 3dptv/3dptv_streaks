
% crl.m Programm zur Kontrolle der Indices, damit sie nicht aus dem Bild fallen
% point=crl(point,lsize)
% point (Nx1) Vektor oder (1xN) Zeilenvektor mit Koordinaten, lsize maximale Koordinate
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function point=crl(point,lsize) %Kontrolle ob die Koordinaten das Bild verlassen
[ysize,xsize,zsize]=size(point);
if ysize==1
    numb=xsize;
else
    numb=ysize;
end
for n=1:numb
    if point(n) < 1
    point(n)=1;
    elseif point(n) > lsize
    point(n)=lsize;
    else
    point(n)=point(n);
    end
end