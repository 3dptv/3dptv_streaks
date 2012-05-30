
% Ordnet die Bildpunkte beider Bilder einander zu. Die zugehörigenden Koordinaten werden gemeinsam in einer Matrix gespeichert. 
% coord=compare(cam1,cam2)
% cam1 (2x2) Koordinaten in der Kamera 1, cam2 (2x2) Koordinaten in der Kamera 2
% coord (2x2) Complex, Realteil: Koordinaten in der Kamera 1, Imaginäranteil: Kamera 2
%
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%
%cam1 [x1,y1 ; x2,y2]
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function coord=compare(cam1,cam2)   

comp1=sum((sum(((cam1-cam2).^2)')));
comp2=sum((sum(((cam1-([cam2(:,2),cam2(:,1)])).^2)')));
    
    
if comp1<comp2
    coord=[cam1(1,1)+i*cam2(1,1),cam1(1,2)+i*cam2(1,2);  cam1(2,1)+i*cam2(2,1),cam1(2,2)+i*cam2(2,2)];  
else
    coord=[cam1(1,1)+i*cam2(1,2),cam1(1,2)+i*cam2(1,1);  cam1(2,1)+i*cam2(2,2),cam1(2,2)+i*cam2(2,1)];  
end