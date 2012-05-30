
% findfkt.m.m  Minimierungsfunktion in Abhängigkeit von Landa, dem skalaren Faktor (Distanz zwischen den Kameras), zu Programm findSl.m
%
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function F=finddistfkt(landa,coord,P,distant)
M=P(1:3,1:3);
N=M^-1;
u3=P(:,4);
c=landa*N*u3;
n=size(coord,3);
for i=1:n
    x1=[real(coord(:,1,n));1];      %Erster Punkt in der Kamera 1
    x2=[imag(coord(:,1,n));1];      %Erster Punkt in der Kamera 2
    y1=[real(coord(:,2,n));1];      %Zweiter Punkt in der Kamera 1
    y2=[imag(coord(:,2,n));1];      %Zweiter Punkt in der Kamera 2
    A1=[N*x2,-x1];
    A2=[N*y2,-y1];
    [res1,flag1] = lsqr(A1,c,[],2000);  %Lösung der Tiefe der Punkte in Abhängigkeit von landa
    [res2,flag2] = lsqr(A2,c,[],2000);  %Lösung der Tiefe der Punkte in Abhängigkeit von landa
    f(i)=norm(res1(2)*x1-res2(2)*y1)-distant;
    f2(i)=norm(res1(1)*N*x2-res2(1)*N*y2)-distant;
end

F=[f;f2];
    