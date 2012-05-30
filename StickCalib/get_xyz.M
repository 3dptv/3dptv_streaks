
% get_xyz.m  Program to calculate and plot the 3D position of the two LED of
%            the calibration bar.
%
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Matthias Machacek 16 Juli 2002
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [p1,p2,delta_bar]=get_xyz(landa,coord,P,distant,delta_landa_list)
M=P(1:3,1:3);
N=M^-1;
u3=P(:,4);
c=landa*N*u3;
n=size(coord,3);

R2=P(1:3,1:3);
u3=P(:,4);
t2=landa*R2'*u3;
    
Pr=zeros(3,4);
Pr(1,1)= 1;
Pr(2,2)= 1;  
Pr(3,3)= 1;  
P1=zeros(4,4);
P1(1,1)=   1;
P1(2,2)=   1; 
P1(3,3)=   1; 

P1=Pr*P1;
P2=Pr*[R2,-R2*t2;0 0 0 1];
    
    
for i=1:n
    x1=[real(coord(:,1,i));1];      %Erster Punkt in der Kamera 1
    x2=[imag(coord(:,1,i));1];      %Erster Punkt in der Kamera 2
    y1=[real(coord(:,2,i));1];      %Zweiter Punkt in der Kamera 1
    y2=[imag(coord(:,2,i));1];      %Zweiter Punkt in der Kamera 2
    %method Michael Studer
    if 0
        A1=[N*x2,-x1];
        A2=[N*y2,-y1];
        [res1,flag1] = lsqr(A1,c,[],2000);  %Lösung der Tiefe der Punkte in Abhängigkeit von landa
        [res2,flag2] = lsqr(A2,c,[],2000);  %Lösung der Tiefe der Punkte in Abhängigkeit von landa
        p1(:,i)=res1(2)*x1; 
        p2(:,i)=res2(2)*y1;
    else
        X=get_3d(x1,x2,P1,P2); 
        p1(1,i)=X(1);p1(2,i)=X(2);p1(3,i)=X(3);
        X=get_3d(y1,y2,P1,P2);
        p2(1,i)=X(1);p2(2,i)=X(2);p2(3,i)=X(3);
        delta_bar(i)=norm(p1(:,i)-p2(:,i))-distant;
    end
end
figure
plot3(p1(1,:),p1(2,:),p1(3,:),'o','MarkerFaceColor','r','MarkerEdgeColor','r','MarkerSize',2);
hold on
plot3(p2(1,:),p2(2,:),p2(3,:),'o','MarkerFaceColor','b','MarkerEdgeColor','b','MarkerSize',2);

figure
plot(delta_landa_list) 
title('Deviation from true cal.-bar length')
ylabel('Difference [mm]')
xlabel('Pair number')

    