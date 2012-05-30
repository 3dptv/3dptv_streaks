
% imcorrec Programm zur Korrektur eines Bildes mit Hintergrundeffekten und zur Optimierung des Bildes für die Lichtpunkteerkennung
% [I3,I4]=imcorrec(I)
% I (NxM) Matrix eines Bildes
% I3 (NxM) Matrix eines Hintergrund korrigierten Bildes, I4 (NxM) Matrix eines für die Lichtpunkterkennung optimierten Bildes
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


function [I3,I4]=imcorrec(I,treshold)

% Korrektur des Hintergrundes des Bildes und der Einflüsse von Spiegelung der Glasscheibe
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

[ysize,xsize,zsize]=size(I);            % Bildgrösse

%Hintergrundapproximation
backApprox = blkproc(I,[16 16],'min(x(:))');
backApprox = double(backApprox)/255;    % Konvertieren in double
I = im2double(I);                       % Konvertieren in double
backApprox480640 = imresize(backApprox, [ysize xsize], 'bilinear');
I2 = I - backApprox480640;              % Hintergrund abziehen.
I2 = max(min(I2,1),0);                  % Helligkeitsverteilung strecken
I3 = imadjust(I2, [0 max(I2(:))], [0 1]); % Kontrast einstellen

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Ende Korrektur des Hintergrundes des Bildes und der Einflüsse von Spiegelung der Glasscheibe


% Optimierung für die Lichtpunkterkennung
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

I4=I3>treshold;  %Threshold
I4(1:5,:)=0;
I4(ysize-4:ysize,:)=0;
I4(:,1:5)=0;
I4(:,xsize-4:xsize)=0;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Ende Optimierung für die Lichtpunkterkennung



%Zusatz //Abbildung Hintergrundapproximation//
% figure
% imshow(backApprox256)
% title('Hintergrundapproximation')

%// Abbildung Hintergrund korrigiert//
% figure
% imshow(I3,[]);
% title('Hintergrund korrigiert');

%// Abbildung treshold //
% figure
% imshow(I4,[])
% title('threshold>0.3')                