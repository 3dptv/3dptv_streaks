
% optesFkt.m Minimierungsfunktion zu Programm optEss.m in Abhängigkeit von der Essentiellen Matrix E, Korrigierten den geometrischen Fehler des Essentiellen Matrix
% Eopt=optesfkt(E,A)
% E (3x3) Essentielle Matrix, A [Nx9] Matrix, die die Bedingung enthält A*f=0
% Eopt (3x3) optimierte Essentielle Matrix

% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function Eopt=optesfkt(E,A);
[U,S,V] = svd(E);
b=size(V,2);
e=V(:,b);                           %Epipolabschätzung der Lösung

% Lösung dieses Problems mit Minimierungsalgorithmus aus Multiple View Geometry in computer vision(Hartley/Zisserman Seite 267
G(1:3,1:3)=trans(e);     
G(4:6,4:6)=trans(e);
G(7:9,7:9)=trans(e);

% Algorithmus Ax minimieren mit Bedingung x=1 und x=G*xstrich, Seite 566 Multiple View Geometry in computer vision(Hartley/Zisserman
r=rank(G);                          % Rank von F
[UG,SG,VG] = svd(G);                % Eigenwertzerlegung
UGstrich=UG(:,r);
[Usol,Ssol,Vsol] = svd(A*UGstrich); % Nullvektror Lösung der Transformation
bsol=size(Vsol,2);
fsol=Vsol(:,bsol);
f=UGstrich*fsol;                    % Rücktransformation

% Testbedingung e(i)'*e(i-1)>0, sonst F=-F;
Etest=reshape(f,3,3)';      
[Utest,Stest,Vtest] = svd(Etest);
btest=size(Vtest,2);
etest=Vtest(:,btest);
if etest'*e<0
    Eopt=-A*f;
else
    Eopt=A*f;
end