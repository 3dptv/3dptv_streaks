
% cormpt Programm zur Korrektur der Koordinaten
% correct=cormpt(list,sx,theta,xh,yh,k1,k2,p1,p2,pixel,ysize,xsize)
% Korrekturfaktoren sx Pixelbreite/Pixelhöhe, theta Scherung, xh,yh Projektionszentrumkorrektur, k1,k2,p1,p2 radiale und tangentiale Verzehrung
% correct (2x2) Korrigierte Koordinaten des Lichtpunktes 
%
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Matthias Machacek
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function correct=normalize(list,focus,sx,theta,xh,yh,k1,k2,p1,p2,pixel,ysize,xsize)
%list(1,:) is the first point 
%list(2,:) is the second point 
for r=1:2
    vec=list(r,:);  
    xc=vec(2);
    yc=vec(1);
  
    %Korrektur der tangentialen und radialen Verzerrung
	option=optimset('LargeScale','off','LevenbergMarquardt','on','Display','off','Diagnostics','off');
    %initial guess
	x0=[(xc-xsize/2)*pixel,(ysize/2-yc)*pixel];
	%Minimierungsverfahren mit Levenberg Marquardt
	[dist,resnorm]=lsqnonlin(@project_fct,x0,[],[],option,xc,yc,sx,theta,xh,yh,k1,k2,p1,p2,pixel,ysize,xsize);
           
    correct(:,r)=[-dist(1)/focus; -dist(2)/focus];      
end

