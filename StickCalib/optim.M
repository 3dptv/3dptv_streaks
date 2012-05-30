
% optim.m  Optimiert die Daten zur Berechnung der Essentiellen Matrix. 
% Anhand der ermittelten skalaren Faktoren, werden die Daten mit der grössten Abweichung eliminiert.
% [winkel,T]=startcal
% [Q1,Q2,coordopt,piclist]=optim(landalist,landa,piclist,coord)
% landalist (Nx1) Zeilenvektor der skalaren Faktoren, landa der ermittelte Skalare Faktor
% piclist (Nx1) Zeilenvektor mit der Liste der verwendeten Bildpaare, coord (2x2xN) Complex Matrix mit den Koordinanten,
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [Q1,Q2,coordopt,piclist,n]=optim(landalist,landa,piclist,coord)
length=size(landalist,2);
n=0;
if landa==0;
    disp('Fehler landa=0')
    return
end

for k=1:length
    if (abs((landalist(k)-landa)/landa)<0.02)
        n=n+1;
        coordopt(:,:,n)=coord(:,:,k);
        Q1(2*n-1:2*n,:)=[real(coord(:,1,k))';real(coord(:,2,k))'];
        Q2(2*n-1:2*n,:)=[imag(coord(:,1,k))';imag(coord(:,2,k))'];
        piclist(n)=piclist(k);
    end
end
        

