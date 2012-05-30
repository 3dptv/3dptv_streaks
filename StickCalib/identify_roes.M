
% Programm zur Identifizierung von Lichtpunkten
% [list_coord,cpara]=identify(pic,treshold) 
% pic (NxN) Matrix eines Bildes, 
% list_coord (2x2) k Koordinanten des Lichtpunktzentrums in Pixel, cpara Bedingungen der Lichtpunkterkennung erfüllt

% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch  -> Modifiziert Matthias Machacek 23. Juli 2002
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


function [list_coord,cpara]=identify(pic,pic_av,treshold) 

%Initalisierung
[ysize,xsize,zsize]=size(pic);          %Bildgrösse                             
list=zeros(2,2);                        %Koordinatenliste des Mittelpunktes des Gebiets
cpara=0;                                %Control Parameter


	%correct for the different row intensities
    %average row intensities
    if 0
        pic = im2double(pic);  
        pic_org=pic;
        av_row_int=sum(pic')./xsize;
        %average image intensity  
        av_img_int=sum(av_row_int)/ysize;
        %correct
        for i=1:ysize
            pic(:,i)=pic(:,i).*av_row_int(i)./av_img_int;
        end
    end
    %subtract averaged image
    %figure
    %imshow(pic)
    %title(['Orginal picture']) 
    
    pic = im2double(pic);     
    pic_org=pic;    
    pic=pic-pic_av;
    %figure
    %imshow(pic)
    %title(['Background subtracted']) 
    
    h= fspecial('Gaussian');
    fpic= filter2(h,pic);
    % find coordinates of brightest pixel in filtered image
    [c i] = max(fpic);
    [cc ii]= max(c);
    jj= i(ii);
    
    % take neighbourhood as PSF mask
    psf= pic(jj-15:jj+15,ii-15:ii+15);
    
    % deconvolve using Wiener filter
    peaks= deconvwnr (pic,psf);
    
    % the two largest peaks are the LED locations
    [c i] = max(peaks);
    [cc ii]= max(c);
    jj= i(ii);
    list_coord(1,2)=jj;  
    list_coord(1,1)=ii; 
    
    peaks(jj,ii)= 0;
            
    [c i] = max(peaks);
    [cc ii]= max(c);
    jj= i(ii);
    list_coord(2,2)=jj;  
    list_coord(2,1)=ii; 
    
    




    



