
% get_3d.m  Program to calculate the 3d position from two images
%
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Machacek Matthias 2002 Juli 15
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function X=get_3d(x1,x2,P1,P2)
    A(1,:)=x1(1)*P1(3,:)-P1(1,:);
    A(2,:)=x1(2)*P1(3,:)-P1(2,:);
    A(3,:)=x2(1)*P2(3,:)-P2(1,:);
    A(4,:)=x2(2)*P2(3,:)-P2(2,:); 
    
    [V,D] = eig(A);
    [n,i]=min(abs(diag(D)));
    
    X(1)=V(1,i)/V(4,i);
    X(2)=V(2,i)/V(4,i);
    X(3)=V(3,i)/V(4,i);