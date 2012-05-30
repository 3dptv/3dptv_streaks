
% optEss.m Programm zur Optimierung der Essentiellen Matrix E, Korrigiert den geometrischen Fehler der Essentiellen Matrix
% E=optess(E0,A)
% E0 (3x3) Essentielle Matrix, A [Nx9] Matrix, die die Bedingung enthält A*f=0
% E (3x3) optimierte Essentielle Matrix

% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function E=optess(E0,A)
E0=10^9*E0;
option=optimset('LargeScale','off','LevenbergMarquardt','on','Display','off','Diagnostics','off');
[E,resnorm]=lsqnonlin(@optesfkt,E0,[],[],option,A);
E=E/10^9;