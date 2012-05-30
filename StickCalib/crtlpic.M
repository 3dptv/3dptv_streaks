
% crtlIden.m Programm zur Kontrolle der Lichtpunkteerkennung, markiert im Bild die Koordinaten
% crtlpic(coord,name,piclist,f_c1,xh_c1,yh_c1,sx_c1,theta_c1,k1_c1,k2_c1,p1_c1,p2_c1,f_c2,xh_c2,yh_c2,sx_c2,theta_c2,k1_c2,k2_c2,p1_c2,p2_c2,pixel)
% coord (2x2xN)Complexe Matrix mit den Koordinaten, name String, piclist (1xN) Liste der verwendeten Bildpaare, pixel Abmessung eines Pixels
% k1.., k2.., p1.., p2.., sx..,theta.., xh.., yh.. Parameter zur Rücktransformation in den unkorrigierten Zustand
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function  crtlpic(coord,name,fname,piclist,f_c1,xh_c1,yh_c1,sx_c1,theta_c1,k1_c1,k2_c1,p1_c1,p2_c1,f_c2,xh_c2,yh_c2,sx_c2,theta_c2,k1_c2,k2_c2,p1_c2,p2_c2,pixel)

n=size(coord,3);


input=n;             % Standart 50 Bilder
prompt  = {'Anzahl Bildpaare eingeben:'};
title   = 'Anzahl Bildpaare';
lines= 1;
def     = {num2str(input)};
input  = str2double(inputdlg(prompt,title,lines,def));

if (n>input) & (input>0)
    num=input; 
else
    num=n;
end

for i=1:num
    
    nb=piclist(i)+10000;
    pic_c1=imread([name,num2str(nb),'.bmp']);
    markpt(pic_c1,real(coord(:,:,i)),f_c1,xh_c1,yh_c1,sx_c1,theta_c1,k1_c1,k2_c1,p1_c1,p2_c1,[fname,num2str(nb),'.bmp'],pixel)
    nc=piclist(i)+20000;                                                
    pic_c2=imread([name,num2str(nc),'.bmp']); 
    markpt(pic_c2,imag(coord(:,:,i)),f_c2,xh_c2,yh_c2,sx_c2,theta_c2,k1_c2,k2_c2,p1_c2,p2_c2,[fname,num2str(nc),'.bmp'],pixel) 
end

