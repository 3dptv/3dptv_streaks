
% backProj.m Programm zur Rücktransformation der Projektion in den 3D Raum
% [X,Y,k]=backproj(x_c1,x_c2,P,landa)
% x_c1 (3x1) Vektor der homogenen Koordinaten in Kamera 1, x_c2 (3x1) Vektor der Koordinaten in Kamera 2, P (3x4) Projektionsmatrix
% landa Skalarer Faktor, der Abstand zwischen den Kameras
% X (4x1), Y (4x1) homogene Koordinaten in 3D, k Tiefe in das Bild
%
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [Xmid,k]=backproj(x_c1,x_c2,P,landa)

% Check ob Format in homogenen Koordinaten
if size(x_c1,1)==2
    x_c1=[x_c1;1];
end
if size(x_c2,1)==2
    x_c2=[x_c2;1];
end

M=P(1:3,1:3);
N=M^-1;
u3=P(:,4);
u3=u3/norm(u3);

% Parameter Bestimmung der Tiefe in das Bild
c=landa*N*u3;
A=[-x_c1,N*x_c2];
[k,flag1] = lsqr(A,c,[],2000);

% Rückprojektion 
X=[k(1)*x_c1;1];
Y=[(N)*(k(2)*x_c2-landa*u3);1];
Xmid=(Y-X)/2+X;
