% rota.m  Bildet die Rotationsmatrix
% R=rota(omega,phi,kappa)
% omega Winkel(um x-Achse),phi Winkel(um y-Achse), kappa Winkel(um z-Achse)
% R (3x3) Rotationsmatrix 
%
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


function R=rota(omega,phi,kappa);               %Rotationmatrix um x-Achse(omega),y-Achse(phi),z-Achse(kappa)

R(1:3,1)=[cos(phi)*cos(kappa);-cos(phi)*sin(kappa);sin(phi)];
R(1:3,2)=[cos(omega)*sin(kappa)+sin(omega)*sin(phi)*cos(kappa);cos(omega)*cos(kappa)-sin(omega)*sin(phi)*sin(kappa);-sin(omega)*cos(phi)];
R(1:3,3)=[sin(omega)*sin(kappa)-cos(omega)*sin(phi)*cos(kappa);sin(omega)*cos(kappa)+cos(omega)*sin(phi)*sin(kappa);cos(omega)*cos(phi)];
