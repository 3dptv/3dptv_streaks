
% markPt.m Zeigt die erkannten Lichtpunkte im Bild an.
% markpt(pic,coord,f,xh,yh,sx,theta,k1,k2,p1,p2,name
% coord (2x1) Koordinanten, f Fokus der Kamera k1, k2, p1, p2, sx ,theta, xh, yh, Parameter zur Rücktransformation 
% in den unkorrigierten Zustand, name string Dateiname des Bildes
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function markpt(pic,coord,f,xh,yh,sx,theta,k1,k2,p1,p2,name,pixel);

[ysize,xsize,zsize]=size(pic);
t=[-5 -4 -3  3 4 5 0 0 0  0  0 0 ];
t(2,:)=[0 0 0 0  0 0 -5 -4 -3 3 4 5];
for k=1:2
    vec1(1,1)=coord(1,k)*-f;
    vec1(2,1)=coord(2,k)*-f;
    
    x_b = xh + vec1(1);
    y_b = yh + vec1(2);
    r=sqrt(x_b^2+y_b^2);
    x_c=x_b + x_b * (k1*r^2 + k2*r^4) + p1*(r^2 +2.0*x_b^2) + 2*p2*x_b*y_b;
    y_c=y_b + y_b * (k1*r^2 + k2*r^4) + p2*(r^2 +2.0*y_b^2) + 2*p1*x_b*y_b;		
    vec3(2) = (sx * x_c  - sin(theta) * y_c) / pixel + xsize/2;
    vec3(1) = ysize / 2 - y_c * cos(theta) / pixel;   

    for i=1:12        
        px=round(vec3(2))+t(1,i);
        py=round(vec3(1))+t(2,i);
        if ((px>640) | (px<0) | (py>480) | (py<0))
            disp('Punkt ist ausserhalb des Musterbilder')
        else
            pic(crl(py,480),crl(px,640))=255;
        end
    end
end
figure(9)
imshow(pic)
title(['Bild ',name, ' ,für weiter, drücken Sie eine Taste'])
disp('Für weiter, drücken Sie eine Taste')
pause