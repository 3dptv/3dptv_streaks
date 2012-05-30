
% distFkt.m  Minimierungsfunktion zu Programm corMpt.m
%
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function F=project_fct(x,xp_mes,yp_mes,sx,theta,xh,yh,k1,k2,p1,p2,pixel,ysize,xsize) %Minimierungsfunktion zu distort-Funktion

%xc,yc are the given pixel coordinates
%x_p,y_p are the calculated pixel coordinates based on the estimated normalized coordinates x(1),x(2)

x_b = xh + x(1);
y_b = yh + x(2);

r=sqrt(x_b^2+y_b^2);
x_c = x_b + x_b * (k1*r^2 + k2*r^4) + p1*(r^2 + 2 * x_b^2) + 2*p2*x_b*y_b;
y_c = y_b + y_b * (k1*r^2 + k2*r^4) + p2*(r^2 + 2 * y_b^2) + 2*p1*x_b*y_b;		

%shearing and stretching -> pixel coordinates (0,0): upper left corner
x_p = (sx * x_c  - sin(theta) * y_c) / pixel + xsize/2;
y_p = ysize / 2 - y_c * cos(theta) / pixel;
    
F=[(x_p-xp_mes); (y_p-yp_mes)];
