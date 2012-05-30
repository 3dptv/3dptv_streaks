
% defwinkel.m Programm zur Bestimmung der Winkel
% [omega,phi,kappa,residual]=defwin(R)
% R (3x3) Rotationsmatrix, residual Fehler der geometrischen Beziehungen
% omega, phi, kappa Winkel der Rotation
%
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [omega,phi,kappa,residual]=defwin(R)%Bestimmt die Winkel der Rotation
option=optimset('LargeScale','off','LevenbergMarquardt','on','Display','off','Diagnostics','off');
w0=[0;0;0];
[winkel,resnorm,residual]=lsqnonlin(@rotafkt,w0,[],[],option,R);
omega=winkel(1);
phi=winkel(2);
kappa=winkel(3);
residual=residual.^2;
residual=sum(residual(:));

