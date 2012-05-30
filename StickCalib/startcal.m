% 
% StarCal.m Programm zur Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts
% [winkel,T]=startcal
% winkel (3x1) Vektor mit den Winkeln, T (3x1) Vektor mit der Verschiebung
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Mit zeitlich synchron aufgenommenen Bildern wird die relative Position der Kameras zueinander bestimmt. 
% Als Hauptkoordinatensystem wird das Koordinatensystem der Kamera 1 mit Fokusierung in die positive z-Richtung verwendet.
%
% Als bekannt werden vorausgesetzt: 
% Distanz(mm) zwischen den 2-Lichtpunkten des Referenzobjektes
% Fokus(mm) der Kamera 1, Kamera 2 
% Korrektur(mm) der Projektion des optischen Zentrums
% Scherung(rad) des Chips
% Parameter k1,k2,p1,p2(dimensionslos) der radialen und tangentialen Verzerrung
% Korrekturfaktor(dimensionslos)des Verhältnis x-Pixelbreite zu y_Pixelhöhe
% Pixelabmessung(mm)


function [resultat]=startcal(coord2,piclist2)
warning off
% Initialisierung
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Parameter Kostfunktion der Lichtpunkterkennung
distant=500.20;                 % Distanz zwischen den beiden Lichtpunkten in Millimeter
del=0;                          % Anfangswert gelöschter Bildpaare
pixel=9E-3;                     % Pixelabmessung
treshold=0.4;                   % Treshold für die Bilderkennung

if ~exist('coord2') | ~exist('piclist2')    
	if 0
        % Intrinsic parameters of camera 1      
        f_c1=8.32939;                   % Fokus 
        xh_c1=-0.0402767;                % Korrekturfaktor x-Koordinate des Central point
        yh_c1=-0.100339;                 % Korrekturfaktor y-Koordinate des Central point
        sx_c1=1.00161;                 % Korrekturfaktor x-Pixelhöhe/y-Pixelhöhe
        theta_c1=0.00113861;           % Korrekturfaktor Scherungswinkel theta              
        k1_c1=-0.00172895;              % Korrekturfaktor radiale Verzerrung
        k2_c1=2.25482e-005;             % Korrekturfaktor radiale Verzerrung
        p1_c1=-0.000213429;             % Korrekturfaktor tangentiale Verzerrung
        p2_c1=-4.89476e-005;              % Korrekturfaktor tangentiale Verzerrung
        % Intrinsic parameters of camera 2
        f_c2=8.38127;                 % Fokus 
        xh_c2=-0.200838;               % Korrekturfaktor x-Koordinate des Central point
        yh_c2=0.0167237;              % Korrekturfaktor y-Koordinate des Central point
        sx_c2=1.0011;                % Korrekturfaktor x-Pixelhöhe/y-Pixelhöhe
        theta_c2=-0.0019483;          % Korrekturfaktor Scherungswinkel theta              
        k1_c2=-0.00199987;            % Korrekturfaktor radiale Verzerrung
        k2_c2=3.87158e-005;             % Korrekturfaktor radiale Verzerrung
        p1_c2=-5.29845e-005;            % Korrekturfaktor tangentiale Verzerrung
        p2_c2=3.4849e-005;                % Korrekturfaktor tangentiale Verzerrung
	elseif 1
        % Intrinsic parameters of REFERENCE CAMERA 1 (short range)
        f_c1=8.37044;                   % Fokus 
        xh_c1=-0.125198;                % Korrekturfaktor x-Koordinate des Central point
        yh_c1=-0.146132;                 % Korrekturfaktor y-Koordinate des Central point
        sx_c1=1.00042;                 % Korrekturfaktor x-Pixelhöhe/y-Pixelhöhe
        theta_c1=0.00206174;           % Korrekturfaktor Scherungswinkel theta              
        k1_c1=-0.00205357;              % Korrekturfaktor radiale Verzerrung
        k2_c1=3.62777e-005;             % Korrekturfaktor radiale Verzerrung
        p1_c1=-0.000265353;             % Korrekturfaktor tangentiale Verzerrung
        p2_c1=-6.86173e-005;              % Korrekturfaktor tangentiale Verzerrung
        % Intrinsic parameters of REFERENCE CAMERA 2 (short range)
        %f_c2=8.36968;                 % Fokus original
        f_c2=9.3;                      % Fokus modified
        xh_c2=-0.310135;               % Korrekturfaktor x-Koordinate des Central point
        yh_c2=-0.0399336;              % Korrekturfaktor y-Koordinate des Central point
        sx_c2=1.00036;                % Korrekturfaktor x-Pixelhöhe/y-Pixelhöhe
        theta_c2=0.00176281;          % Korrekturfaktor Scherungswinkel theta              
        k1_c2=-0.00215218;            % Korrekturfaktor radiale Verzerrung
        k2_c2=4.2455e-005;             % Korrekturfaktor radiale Verzerrung
        p1_c2=-0.000440001;            % Korrekturfaktor tangentiale Verzerrung
        p2_c2=1.82907e-005;            % Korrekturfaktor tangentiale Verzerrung
	elseif 0
        %something wrong with this dataset
        % Intrinsic parameters of Camera1_2Aug2002 (short range)
        f_c1=8.08668;              
        xh_c1=0.0442334;              
        yh_c1=-0.0923112;                
        sx_c1=1.00227;               
        theta_c1=0.00102762;                     
        k1_c1=-0.00160339;             
        k2_c1=-2.14079e-005;             
        p1_c1=9.54823e-005;           
        p2_c1=0.00011293;             
        % Intrinsic parameters of Camera1_1Aug2002 (short range)
        f_c2=8.39017;                 
        xh_c2=-0.340186;              
        yh_c2=-0.0384037;             
        sx_c2=1.00029;               
        theta_c2=0.00129106;                     
        k1_c2=-0.00197283;            
        k2_c2=3.30774e-005;            
        p1_c2=-0.000624347;           
        p2_c2=-4.98973e-005;                 
    else
        %Parameters from Delta3_Juni2
        f_c1=8.32939;
        xh_c1=-0.0402767;
        yh_c1=-0.100339;
        sx_c1=1.00161;
        theta_c1=0.00113861;
        k1_c1=-0.00172895;
        k2_c1=2.25482e-005;
        p1_c1=-0.000213429;
        p2_c1=-4.89476e-005;

        f_c2=8.38127;
        xh_c2=-0.200838;
        yh_c2=0.0167237;
        sx_c2=1.0011;
        theta_c2=-0.0019483;
        k1_c2=-0.00199987;
        k2_c2=3.87158e-005;
        p1_c2=-5.29845e-005;
        p2_c2=3.4849e-005;
	end

    %Pfad bestimmen
    [fname,pname] = uigetfile('*.bmp','Pfad der Bilder angeben');
    start_n=str2num(fname((size(fname,2)-7):(size(fname,2)-4)));         % Start Laufvariable bestimmen
    fname=fname(1:(size(fname,2)-9));
    name=[pname,fname]; 
    img_name=name;
    
    %Anzahl Bilder festlegen
    number_pictures=50;             % Standart 50 Bilder
    prompt  = {'Number of images:'};
    title   = 'Number of images';
    lines= 1;
    def     = {num2str(number_pictures)};
    number_pictures  = str2double(inputdlg(prompt,title,lines,def));
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Ende Initalisierung
    
    
    % Hauptprogramm
    
    % Datenerkennung zur Berechnung der Essentiellen Matrix
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %get the image shift between camera 1 and 2
    if 0
        n_c=10000+5+start_n-1;     
        pic1=imread([name,num2str(n_c),'.bmp']);                 % Bild einlesen
        pic2=imread([name,num2str(n_c),'.bmp']);                 % Bild einlesen 
        figure
        imshow(pic1);
        msgbox('Klick on a point in img 1 and then return');    
        [x1,y1] = ginput(1);  
        figure
        imshow(pic2);      
        msgbox('Klick on the correspondig point in img2 and then return');    
        [x2,y2] = ginput(1);      
    end
    
  % Checkoption to use averaged background for subtraction
    button = questdlg('Do averaged background subtraction?','AvBgSub','Yes','No','No');
    if strcmp(button,'Yes')
        if number_pictures > 50
            disp(['Build averaged background']) 
            %make a average of all images
            pic1_av=zeros(480,640);
            pic2_av=zeros(480,640);
            pic_nr=0;
            for n=1:number_pictures
                n_c1=10000+n+start_n-1;                                                  % Laufvariable Kamera 1
                n_c2=20000+n+start_n-1;                                                  % Laufvariable Kamera 2
                if exist([name,num2str(n_c1),'.bmp'])&exist([name,num2str(n_c2),'.bmp'])
                     pic1=imread([name,num2str(n_c1),'.bmp']);                 % Bild einlesen
                     pic2=imread([name,num2str(n_c2),'.bmp']);                 % Bild einlesen            
                     pic1_av=pic1_av+im2double(pic1);
                     pic2_av=pic2_av+im2double(pic2);  
                     pic_nr=pic_nr+1;
                end
            end
            pic1_av=pic1_av/pic_nr;  
            pic2_av=pic2_av/pic_nr;
            figure
            imshow(pic1_av)
            %title(['Background approximation of image 1'])    
            figure
            imshow(pic2_av)
            %title(['Background approximation of image 2']) 
        else
             disp(['Too few images, no averaged background is used']) 
             pic1_av=zeros(480,640);
             pic2_av=zeros(480,640);   
        end
	elseif strcmp(button,'No') 
       disp(['No averaged background subtraction'])      
       pic1_av=zeros(480,640);
       pic2_av=zeros(480,640);   
    end
    
    
    disp(['Markers identification and coordinate normalization'])    
    for n=1:number_pictures
        
        n_c1=10000+n+start_n-1;                                                  % Laufvariable Kamera 1
        n_c2=20000+n+start_n-1;                                                  % Laufvariable Kamera 2
        if exist([name,num2str(n_c1),'.bmp'])&exist([name,num2str(n_c2),'.bmp'])
            
            ncorr=n-del;                                                % Korrektur  der gelöschten Bilder
            
            % Erkennung der Lichtpunkte
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            
            % //Kamera 1//
            disp([name,num2str(n_c1),'.bmp'])
            pic_c1=imread([name,num2str(n_c1),'.bmp']);                 % Bild einlesen
            [h_c1,b_c1,z_c1]=size(pic_c1);                              % Grösse Bild
            pic_c1=pic_c1(1:h_c1,1:b_c1);                               % Grösse Transformation
            
            % Bestimmung der Koordinaten  der Lichtpunkte, Ausgabe ob die Punkte den Erkennungskritieren entprechen  
            [list1_coord,cpara1]=identify(pic_c1,pic1_av,treshold);   
            %structure of the variable list1_coord
            %list_coord(1,2) : x-coordinate of the first point 
            %list_coord(1,1) : y-coordinate of the first point  
            %list_coord(2,2) : x-coordinate of the second point  
            %list_coord(2,1) : y-coordinate of the second point 
            
            %normalization
            %the cam structure is the same as list_coord: [y1, x1; y2,x2]
            cam1=normalize(list1_coord,f_c1,sx_c1,theta_c1,xh_c1,yh_c1,k1_c1,k2_c1,p1_c1,p2_c1,pixel,h_c1,b_c1);
            
            % //Kamera 2//
            disp([name,num2str(n_c2),'.bmp']) 
            pic_c2=imread([name,num2str(n_c2),'.bmp']);                 % Bild einlesen
            [h_c2,b_c2,z_c2]=size(pic_c2);                              % Grösse Bild
            pic_c2=pic_c2(1:h_c2,1:b_c2);                               % Grösse Transformation
            
            % Bestimmung der Koordinaten  der Lichtpunkte, Ausgabe ob die Punkte den Erkennungskritieren entprechen        .
            [list2_coord,cpara2]=identify(pic_c2,pic2_av,treshold);  
            %normalization
            cam2=normalize(list2_coord,f_c2,sx_c2,theta_c2,xh_c2,yh_c2,k1_c2,k2_c2,p1_c2,p2_c2,pixel,h_c2,b_c2);
            
            % Test ob die Punkte den Erkennungskriterien entsprechen
            if ((cpara1==1) | (cpara2==1))
                del=del+1;
                disp('Image pair rejected:')
            else
                % Bildpunkte Zuordnung, Vergleich der Distanzen zwischen Bildpunkten
                % Die zugehörigen Bildpunkte werden gemeinsam in einer 2*2*Bildpaaranzahl Matrix gespeichert. 
                % Realteil: Koordinaten in der Kamera 1, Imaginäranteil: Kamera 2
                coord(:,:,ncorr)=compare(cam1,cam2);   
                piclist(ncorr)=n+start_n-1;
            end
            
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            % Ende Aufbereitung der Daten
        else
            disp('Image not found')
            del=del+1;
        end
    end
    % Checkoption to review the marker identification
    button = questdlg('Review the marker identification?',...
        'Review','Yes','No','No');
    if strcmp(button,'Yes')
        crtlpic(coord,name,fname,piclist,f_c1,xh_c1,yh_c1,sx_c1,theta_c1,k1_c1,k2_c1,p1_c1,p2_c1,f_c2,xh_c2,yh_c2,sx_c2,theta_c2,k1_c2,k2_c2,p1_c2,p2_c2,pixel)
        [coord,piclist]=delpic(coord,piclist,name);
    elseif strcmp(button,'No') 
    end
    
else
    coord=coord2;
    piclist=piclist2;
end

pltallpts(coord,name,fname,f_c1,xh_c1,yh_c1,sx_c1,theta_c1,k1_c1,k2_c1,p1_c1,p2_c1,f_c2,xh_c2,yh_c2,sx_c2,theta_c2,k1_c2,k2_c2,p1_c2,p2_c2,pixel)  
disp(['Image pairs used:',num2str(number_pictures-del)])
disp(['Rejected image pairs:',num2str(del)])
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Ende Datenerkennung zur Berechnung der Essentiellen Matrix
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Essential matrix calcualation
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
length=size(coord,3);
%if length >=8 
if 1     
	B1=reshape(real(coord),2,2*length,1);                   %Umschreiben für das Programm FundMat.m
	Q1=reshape(B1',2*length,2);
	B2=reshape(imag(coord),2,2*length,1);                   %Umschreiben für das Programm FundMat.m
	Q2=reshape(B2',2*length,2);
	disp('Calculate the essential matrix with the norm. 8-point alg.')
	[E0,A] = fundmat(Q1,Q2);                                % Essentielle Matrix Berechnung
	[av_res,av_dist]=resplot(E0,coord,'(norm. 8 point alg.)');        % Residualplot der Essentiellen Matrix
	disp('Recalculate the essential matrix with a alg. error alg.')
	E1=optess(E0,A);                                        % Optimiert die Essentielle Matrix
	[av_res_opt,av_dist_opt]=resplot(E1,coord,'(alg. minim. alg.)');      % Residualplot der optimierten Essentiellen Matrix
	E=E1;
	
	%calculate the av. point epiplar line distance in pixel
    av_dist_p=av_dist*f_c2/pixel;
    av_dist_opt_p=av_dist_opt*f_c2/pixel;    
	%Speichert Q1,Q2, die optimierte Essentielle Matrix E und die Koordinaten coord 
	save([pname,'savedata.mat'],'Q1','Q2','coord','piclist','E0','E1','-mat');
	
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	% End of the essential matrix calculation%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	
	
	
	
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	% Calculation of the rotation and translation of camera 2 with reference to camera 1  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	
	% Bestimmt den skalaren Faktor (Distanz zwischen den Kameras) und die Projektionsmatrix der Kamera 2
	[P,landa,delta_bar_list]=findsl(E,coord,distant);
    
    %calculate the three dimensional positions of the markers and plot them
    %and plot the difference between true bar lenght and the calcualated
    %and plot the difference of the camera distance
	[p1,p2,delta_bar]=get_xyz(landa,coord,P,distant,delta_bar_list); 
    delta_bar_av=mean(delta_bar);
    delta_bar_std=std(delta_bar);
    
	% Bestimmung der Rotationsmatrix, des Translationsvektors und der Winkel
	R=P(1:3,1:3);
	T=-R'*landa*P(:,4);
	[omega,phi,kappa,residual1]=defwink(R);
	winkel=[omega;phi;kappa];
	
	% Ausgabe der Resulate
	prompt  = {'Winkel omega','Winkel phi', 'Winkel kappa','x-Translation','y-Translation','z-Translation','Distanz'};
	title   = ['Result of the calibration'];
	lines= [1,60];
	def     = {num2str(winkel(1)),num2str(winkel(2)),num2str(winkel(3)),num2str(T(1)),num2str(T(2)),num2str(T(3)),num2str(landa)};
	resultat  = str2double(inputdlg(prompt,title,lines,def))
	
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	% End of Calculation of the rotation and translation of camera 2 with reference to camera 1  %%%%%%%%%%%%%%%%%%%%%%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	
	
	
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	% Analysis of the image data and recalculation with a modified coordinate list               %%%%%%%%%%%%%%%%%%%%%%
	% the markers are rejected if the relative difference between the average length             %%%%%%%%%%%%%%%%%%%%%%
	% and the corresponding length is larger than 0.02                                           %%%%%%%%%%%%%%%%%%%%%%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	button = questdlg('Sort out bad marker pairs?',...
        'Sort out','Yes','No','No');
	if strcmp(button,'Yes')
        [Q1,Q2,coord,piclist,n]=optim(delta_landa_list,landa,piclist,coord);     %Filter the data based on the estimated lenght of the cal bar 
        disp(['Number filtered marker pairs',num2str(n)]) 
        if size(Q1,1)>4
            [E2,A] = fundmat(Q1,Q2);                                     %Bildet den Essentiellen Vektor      
            %E2=optess(E0,A);                                            %Optimiert die Essentielle Matrix
            %figure
            av_res_filter=resplot(E2,coord,'(filterd)');        %Residual plot of the  filtered essential matrix
            E=E2;
            
            % Bestimmt den skalaren Faktor (Distanz zwischen den Kameras) und die Projektionsmatrix der Kamera 2
            [P,landa,delta_landa_list]=findsl(E,coord,distant);
            
            % Bestimmung der Rotatinsmatrix, des Translationsvektors und der Winkel
            R=P(1:3,1:3);
            T=-R'*landa*P(:,4);
            [omega,phi,kappa,residual1]=defwink(R);
            winkel=[omega;phi;kappa];
            resultat=[winkel;T;norm(T)];
            
            % Plot the distance between the camera 1&2
			plot(delta_landa_list)
            a= size(delta_landa_list);  
            text(5,landa,'Estimated camera distance')       
            line([1 a(2)],[landa landa]);
			%title('Estimated camera distance (filterd data)')
			ylabel('Length [mm]')
			xlabel('Pair number')
			drawnow;
        else
            disp('Error, use more image pairs')
            return
        end
	elseif strcmp(button,'No')
        disp('Calibrated')
	end
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	% Analysis of the image data and recalculation with a modified coordinate list               %%%%%%%%%%%%%%%%%%%%%%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	
	
	
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	% Save the data %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
    %modify the P matrix to the right scale
	t2=landa*R'*P(:,4);
    P=[R,-R*T;];
    
    E_used=-trans(T)*R';
    E_used=E_used';
	%Speichern von Daten
	datum=datestr(date,1);
	filename=['result.mat'];
	[filename,pname] = uiputfile([pname,filename],'Save M-file name');
	if filename ~= 0 
        save([pname,filename],'winkel','T','E_used','P','landa');
	end
	
	filename=['stick_cal_res.dat'];
	[filename,pname] = uiputfile([pname,filename],'Save dat. file name');
	if filename ~= 0 
        pname=strcat(pname,filename);
		fid = fopen(pname,'w');
		fprintf(fid,'The camera parameters\n');
  	    fprintf(fid,'The rotation matrix is transposed defined compared to Stereo2!!!\n');      
		fprintf(fid,'Camera position X, Y, Z[mm]\n');
		fprintf(fid,'%10.4f\n%10.4f\n%10.4f\n',T(1),T(2),T(3));
 		fprintf(fid,'Camera orientation omega, phi ,kappa [grad]\n');
		fprintf(fid,'%8.4f\n%8.4f\n%8.4f\n',winkel(1),winkel(2),winkel(3));        
		fprintf(fid,'The rotation matrix\n');
		fprintf(fid,'%10.4f %10.4f %10.4f\n%10.4f %10.4f %10.4f\n%10.4f %10.4f %10.4f\n',R(1,1),R(1,2),R(1,3),R(2,1),R(2,2),R(2,3),R(3,1),R(3,2),R(3,3));    
		fprintf(fid,'The essential matrix\n');
		fprintf(fid,'%10.4f %10.4f %10.4f\n %10.4f %10.4f %10.4f\n %10.4f %10.4f %10.4f\n',E_used(1,1),E_used(1,2),E_used(1,3),E_used(2,1),E_used(2,2),E_used(2,3),E_used(3,1),E_used(3,2),E_used(3,3));
		fprintf(fid,'The camera matrix P\n');  
		fprintf(fid,'%10.4f %10.4f %10.4f %10.4f\n%10.4f %10.4f %10.4f %10.4f\n%10.4f %10.4f %10.4f %10.4f\n',P(1,1),P(1,2),P(1,3),P(1,4),P(2,1),P(2,2),P(2,3),P(2,4),P(3,1),P(3,2),P(3,3),P(3,4));
		fprintf(fid,'Distance between the cameras\n'); 
		fprintf(fid,'%7.3f\n',landa); 
       	fprintf(fid,'Mean cal. bar length deviation and standart deviation of the data set\n');  
        fprintf(fid,'%10.7f  %10.7f \n',delta_bar_av,delta_bar_std);
        fprintf(fid,'The residual of the essential matrix (norm. 8 point alg.)\n'); 
        fprintf(fid,'%10.7f\n',av_res); 
        fprintf(fid,'The residual of the essential matrix (alg. minim. alg.)\n'); 
        fprintf(fid,'%10.7f\n',av_res_opt);  
        fprintf(fid,'The average point epi-line distance in normalized and pixel units (norm. 8 point alg.)\n'); 
        fprintf(fid,'%10.7f [-] %10.7f [pixel]\n',av_dist, av_dist_p); 
        fprintf(fid,'The average point epi-line distance in normalized and pixel units (alg. minim. alg.)\n'); 
        fprintf(fid,'%10.7f [-] %10.7f [pixel]\n',av_dist_opt, av_dist_opt_p);              
        if(av_res_filter ~= 0)
            fprintf(fid,'The residual of the essential matrix (filered)\n'); 
            fprintf(fid,'%10.7f\n',av_res_filter);  
        else
            fprintf(fid,'Data not filtered\n');  
        end
        fprintf(fid,'\nDate: %s\n',datum); 
        fprintf(fid,'Used images: %s\n',img_name); 
        fprintf(fid,'Number of pairs used: %s',num2str(number_pictures-del));
		fclose(fid);
        disp('Result saved')
    else
        disp('Not saved')   
	end
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	% End Save the data %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
else
    disp('Eight or more markers needed!!! (moron)')
end

% Ende Hauptpgrogramm