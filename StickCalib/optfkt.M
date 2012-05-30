%optfkt.m : the function to be minimized
%
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Machacek Matthias 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function F=optfkt(landa,coord,P,distant)
	R2=P(1:3,1:3);
	u3=P(:,4);
	t2=landa*R2'*u3; %is it the right sign??????????????????!!!!!!!!!!!! it should be negative
    
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
   
    n=size(coord,3);
	for i=1:n
        x1=[real(coord(:,1,i));1];      %Erster Punkt in der Kamera 1
        x2=[imag(coord(:,1,i));1];      %Erster Punkt in der Kamera 2
        y1=[real(coord(:,2,i));1];      %Zweiter Punkt in der Kamera 1
        y2=[imag(coord(:,2,i));1];      %Zweiter Punkt in der Kamera 2
        X1=get_3d(x1,x2,P1,P2);
        X2=get_3d(y1,y2,P1,P2);    
        f1(i)= norm(X1-X2)-distant;
	end
	
	F=[f1];
    