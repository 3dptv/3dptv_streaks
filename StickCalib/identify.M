
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
    pic = im2double(pic);     
    pic_org=pic;    
    pic=pic-pic_av;
    
    pic=im2bw(pic,treshold);  %Threshold
    
	[labeled,numlab]=bwlabel(pic); 
    if numlab==2 
        blobs = regionprops(labeled,'Area','BoundingBox','Centroid','PixelList');
        %if the blob is too small or too large, do not take
        if blobs(1).Area > 100 | blobs(1).Area < 5 | blobs(2).Area > 100 | blobs(2).Area < 5
            list_coord=zeros(2,2);
            cpara=1;            
            return
        end
        %if the blob is too near to the image boundary, do not take 
        xb1=blobs(1).BoundingBox(1);
        yb1=blobs(1).BoundingBox(2);
        w1= blobs(1).BoundingBox(3);
        h1= blobs(1).BoundingBox(4);
        xb2=blobs(2).BoundingBox(1);
        yb2=blobs(2).BoundingBox(2);
        w2= blobs(2).BoundingBox(3);
        h2= blobs(2).BoundingBox(4);
        if xb1<2 | yb1<2 | (xb1+w1)>(xsize-2) | (yb1+h1)>(ysize-2) | xb2<2 | yb2<2 | (xb2+w2)>(xsize-2) | (yb2+h2)>(ysize-2)
            list_coord=zeros(2,2);
            cpara=1;            
            return
        %caclulate the intensity weighted center of gravity       
        else       
            %get the pixel values
            [nrx,elem]=size(blobs(1).PixelList);
            int_sum=0;
            xysum=0;
            for i=1:nrx
                xysum=xysum+blobs(1).PixelList(i,:)  .*  pic_org(blobs(1).PixelList(i,2),blobs(1).PixelList(i,1));
                int_sum=int_sum+pic_org(blobs(1).PixelList(i,2),blobs(1).PixelList(i,1));
            end
            list_coord(1,2)=xysum(1)./int_sum;  
            list_coord(1,1)=xysum(2)./int_sum; 
            
            %get the pixel values
            [nrx,elem]=size(blobs(2).PixelList);
            int_sum=0;
            xysum=0;
            for i=1:nrx
                xysum=xysum+blobs(2).PixelList(i,:).*pic_org(blobs(2).PixelList(i,2),blobs(2).PixelList(i,1));
                int_sum=int_sum+pic_org(blobs(2).PixelList(i,2),blobs(2).PixelList(i,1));
            end
            list_coord(2,2)=xysum(1)./int_sum;  
            list_coord(2,1)=xysum(2)./int_sum;   
            return
        end
    else
        list_coord=zeros(2,2);
        cpara=1;
        return
    end  
    




    



