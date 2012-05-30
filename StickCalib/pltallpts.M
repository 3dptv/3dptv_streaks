
% ctrallpts.m abgeleites aus crtlIden.m Programm zur Kontrolle der aller Marker
% ctrallpts(coord,name,piclist,f_c1,xh_c1,yh_c1,sx_c1,theta_c1,k1_c1,k2_c1,p1_c1,p2_c1,f_c2,xh_c2,yh_c2,sx_c2,theta_c2,k1_c2,k2_c2,p1_c2,p2_c2,pixel)
% coord (2x2xN)Complexe Matrix mit den Koordinaten, name String, piclist (1xN) Liste der verwendeten Bildpaare, pixel Abmessung eines Pixels
% k1.., k2.., p1.., p2.., sx..,theta.., xh.., yh.. Parameter zur Rücktransformation in den unkorrigierten Zustand
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function  pltallpts(coord,name,fname,f_c1,xh_c1,yh_c1,sx_c1,theta_c1,k1_c1,k2_c1,p1_c1,p2_c1,f_c2,xh_c2,yh_c2,sx_c2,theta_c2,k1_c2,k2_c2,p1_c2,p2_c2,pixel)

pic_all(1:480,1:640,1:3)=1;
num=size(coord,3);
dist=0;

t=[-3 -2 -1  1 2 3 0 0 0 0 0 0 ];
t(2,:)=[0 0 0 0 0 0 -3 -2 -1 1 2 3];

[ysize,xsize,zsize]=size(pic_all);
for ii=1:num
    for k=1:2
        %camera 1
        vec1_c1(1,1)=real(coord(1,k,ii))*-f_c1;
        vec1_c1(2,1)=real(coord(2,k,ii))*-f_c1;
        
        x_b = xh_c1 + vec1_c1(1);
        y_b = yh_c1 + vec1_c1(2);
        r=sqrt(x_b^2+y_b^2);
        x_c=x_b + x_b * (k1_c1*r^2 + k2_c1*r^4) + p1_c1*(r^2 +2.0*x_b^2) + 2*p2_c1*x_b*y_b;
        y_c=y_b + y_b * (k1_c1*r^2 + k2_c1*r^4) + p2_c1*(r^2 +2.0*y_b^2) + 2*p1_c1*x_b*y_b;		
        vec3_c1(2) = (sx_c1 * x_c  - sin(theta_c1) * y_c) / pixel + xsize/2;
        vec3_c1(1) = ysize / 2 - y_c * cos(theta_c1) / pixel;      
        
        %camera 2
        vec1_c2(1,1)=imag(coord(1,k,ii))*-f_c2;
        vec1_c2(2,1)=imag(coord(2,k,ii))*-f_c2;
        
        x_b = xh_c2 + vec1_c2(1);
        y_b = yh_c2 + vec1_c2(2);
        r=sqrt(x_b^2+y_b^2);
        x_c=x_b + x_b * (k1_c2*r^2 + k2_c2*r^4) + p1_c2*(r^2 +2.0*x_b^2) + 2*p2_c2*x_b*y_b;
        y_c=y_b + y_b * (k1_c2*r^2 + k2_c2*r^4) + p2_c2*(r^2 +2.0*y_b^2) + 2*p1_c2*x_b*y_b;		
        vec3_c2(2) = (sx_c2 * x_c  - sin(theta_c2) * y_c) / pixel + xsize/2;
        vec3_c2(1) = ysize / 2 - y_c * cos(theta_c2) / pixel;             

        %camera 1
        px_c1=round(vec3_c1(2));
        py_c1=round(vec3_c1(1));         
        %camera 2         
        px_c2=round(vec3_c2(2));
        py_c2=round(vec3_c2(1));    
        
        x1(2*ii+k-2)=px_c1;
        y1(2*ii+k-2)=py_c1;
        x2(2*ii+k-2)=px_c2;
        y2(2*ii+k-2)=py_c2;  

        dist=dist+sqrt((px_c1-px_c2)^2+(py_c1-py_c2)^2);
        for i=1:12     
            if ((px_c1<640) & (px_c1>0) & (py_c1<480) & (py_c1>0))
                pic_all(crl(py_c1+t(2,i),480),crl(px_c1+t(1,i),640),1)=0;
                pic_all(crl(py_c1+t(2,i),480),crl(px_c1+t(1,i),640),2)=0;                
                pic_all(crl(py_c2+t(2,i),480),crl(px_c2+t(1,i),640),2)=0;  
                pic_all(crl(py_c2+t(2,i),480),crl(px_c2+t(1,i),640),3)=0;                  
            end
        end
	end    
end
dist=dist/num;
imshow(pic_all)
% for i=1:num
%     line([x1(2*i-1) x1(2*i)], [y1(2*i-1) y1(2*i)],'LineWidth',0.1,'Color',[0.0 0.0 1]);
%     line([x2(2*i-1) x2(2*i)], [y2(2*i-1) y2(2*i)],'LineWidth',0.1,'Color',[1 0.0 0.0]);    
% end
    
title('Distribution of the calibration markers');
text(400,110,'blue:  camera 1');
text(400,130,'red: camera 2');
text(400,150,['average shift: ',num2str(dist) ,' [pixel]']);