
% findSl.m Programm zur Bestimmung des skalaren Faktors der Essentiellen Matrix (Distanz zwischen den Kameras), sowie der Projektionsmatrix der zweiten Kamera
% [R,T,winkel,mlanda,P,landalist]=findsl(E,coord,distant)
% E (3x3) Essentielle Matrix, coord (2x2xN) Complexe Matrix mit den Koordinaten, distant der bekannte Abstand zwischen den Lichtpunkten
% mlanda ermittelte Distanz zwischen den Kamerazentren(skalarer Faktor), P (3x4) Projektionsmatrix der Kamera 2
% landalist (1xN) list der ermittelten Distanzen der Punktepaare
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [P,mlanda,delta_lenght_list]=findsl(E,coord,distant)

% Initalisierung
option=optimset('largescale','off','LevenbergMarquardt','on','display','none','showstatus','iterplus','gradobj','on','Diagnostics','off');
landa0=1;                       %Startwert für die Distanz
length=size(coord,3);           %Anzahl Bildpaare  

% Bestimmung der richtigen Projektionsmatrix aus den vier Möglichkeiten
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% SVD Zehrlegung, Rotations und Translationsanteil
[U,D,V]=svd(E);                 
W=[0,-1,0;1,0,0;0,0,1];
u3=U(:,3);

%Die vier Möglichkeiten der P-Matrix
Pm(:,:,1)=[U*W*V',u3];          % P-Marix Variante 1+2 wobei u3=+/-U(:,3)
Pm(:,:,2)=[U*W'*V',u3];         % P-Marix Variante 3+4 wobei u3=+/-U(:,3)

% Test der P-Matrix mit Bedingung Punkt muss in positiver z-Richtung liegen und die Länge des Translationsvektors muss grösser 0 sein.
% Verwende einen Testpunkt zur Bestimmung der P-Matrix
%%NEU!!!!!!
%Test der P-Matrix mit Bedingung Punkt muss in NEGATIVER z-Richtung liegen und die Länge des Translationsvektors muss grösser 0 sein.
% Verwende einen Testpunkt zur Bestimmung der P-Matrix
for n=1:2
    P=Pm(:,:,n);
%     [o1,ph1,k1,residual1]=defwink(P(:,1:3));
%     [o2,ph2,k2,residual2]=defwink(-P(:,1:3));
%     
%     if residual1>residual2
%         P=-P;
%     end
    
    [landatest,resnorm,F]=lsqnonlin(@findfkt,landa0,[],[],option,coord(:,:,1),P,distant);
    if landatest<0
        landatest=-landatest;
        P(:,4)=-P(:,4);
        disp('Achtung in findsl')
        
    end
    [Xtest,ktest]=backproj([real(coord(:,1,1));1],[imag(coord(:,1,1));1],P,landatest);
    
    if (ktest(1)>0)&(ktest(2)>0)
        P(:,4)=-P(:,4);
        P(1:3,1:3)=P(1:3,1:3);
             
    elseif (ktest(1)<0)&(ktest(2)>0)
        P(:,4)=-P(:,4);    
        P(1:3,1:3)=-P(1:3,1:3);
                
    elseif (ktest(1)>0)&(ktest(2)<0)
        P(:,4)=P(:,4);
        P(1:3,1:3)=-P(1:3,1:3);
        
    end
    Pm(:,:,n)=P;
    [X2,k2]=backproj([real(coord(:,1,1));1],[imag(coord(:,1,1));1],P,landatest);
end

% Test, welche Rotationsmatrix die Richtige ist.
[o1,ph1,k1,residual1]=defwink(Pm(:,1:3,1));
[o2,ph2,k2,residual2]=defwink(Pm(:,1:3,2));
if residual1 <=residual2
    P=Pm(:,:,1);
    winkel=[o1;ph1;k1];
else
    P=Pm(:,:,2);
    winkel=[o2;ph2;k2];
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Ende Bestimmung der richtigen Projektionsmatrix aus den vier Möglichkeiten


% Berechnung des skalaren Faktors (Distanz zwischen den Kamerazentren)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Berechnung der Länge des Vektors zwischen den Zentren der Kameras mit Kriterium der bekannten Distanz der Lichtpunkte im 3D
% Für des Koordinatenpaar wird Landa ermittelt
disp('Non linear estimation of the essential matrix scale...')
warning off
	option=optimset('LargeScale','off','LevenbergMarquardt','on','Display','off','Diagnostics','off');
	landatest=400;
	[mat_landa,resnorm,delta_lenght_list]=lsqnonlin(@optfkt,landatest,[],[],option,coord,P,distant);
	mlanda=mat_landa;
warning on  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Ende Berechnung des skalaren Faktors (Distanz zwischen den Kamerazentren)

    
    
