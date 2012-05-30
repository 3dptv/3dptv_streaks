
% Programm zur Löschung der Koordinaten einzelner Bilder
% [coord,piclist]=delpic(coord,piclist,name)
% coord (2x2xN) Complexe Matrix mit Koodinaten, piclist(1xN) Zeilenvektor mit Liste der Bilder 
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




function [coord,piclist]=delpic(coord,piclist,name)
del=1;
length=size(piclist,2);

if length ~= size(coord,3)
    disp('Fehler in delpic.m')
    return
end

while 0~=del
    vgl=0;
    k=1;
    prompt  = {['Löschen des Bildes  ',name,'*****.bmp, 0 Ende Löschen']};
    title   = ['Löschen eines Bildes'];
    lines= [1];
    def     = {num2str(0)};
    del  = inputdlg(prompt,title,lines,def);
    
    if 0==size(del,1)
        break,return
    else
        del=str2double(del);
    end
    if (del>20000) & (del<30000)
        vgl=del-20000;
        del2=del-10000;
    elseif (del>10000) & (del<20000)
        vgl=del-10000;
        del2=del+10000;
    elseif del~=0
        disp('Fehler, falsche Bildnummer');
    end
    lg=size(piclist,2);
    while (k<=lg-1) & (vgl~=0) & (vgl~= piclist(k))
        k=k+1;
    end
    if vgl==piclist(k)
        if k==1
            piclist=piclist(2:lg);
            coord=coord(:,:,2:lg);
        elseif k==lg
            piclist=piclist(1:lg-1);
            coord=coord(:,:,1:lg-1);
        else          
            piclist=[piclist(1:k-1),piclist(k+1:lg)];
            coordvgl(:,:,1:k-1)=coord(:,:,1:k-1);
            coordvgl(:,:,k:lg-1)=coord(:,:,k+1:lg);
            coord=coordvgl;
            clear coordvgl
        end
        delete([name,num2str(del),'.bmp'])
        delete([name,num2str(del2),'.bmp'])
        disp(['Bild ',name,num2str(del),'.bmp und Bild ',name,num2str(del2),'.bmp gelöscht'])
    elseif del~=0
        disp('Fehler, falsche Bildnummer');
        
    end
end 