
% ResPlot.m Plotet die Residuals der Essentiellen Matrixberechnung
% resplot(E,coord,titel)
% coord (2x2xN) Complexe Matrix mit den Koordinaten, E (3x3) Essentielle Matrix, titel string 
% 
% Semesterarbeit Kalibrierung zweier Kameras im Windkanal mittels 2-Punkte-Referenzobjekts 2001/2002
% Michael Sauter sauterm@student.ethz.ch
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


function [av_res,av_dist]=resplot(E,coord,titel)
N=size(coord,3);
av_res=0;
av_dist=0;
for n=1:N
    %first point
    res1(n)=abs([imag(coord(:,1,n));1]'*E*[real(coord(:,1,n));1]);
    %second point
    res2(n)  =abs([imag(coord(:,2,n));1]'*E*[real(coord(:,2,n));1]);
    av_res=av_res+res1(n)+res2(n);
end
av_res=av_res/(2*N);
figure,
plot(res1,'-')
hold on
plot(res2,':')
title(['Residual of the essential matrix   R=x_2^T E x_1'  ,titel])
xlabel('Point number');
ylabel('Residual');
text(N,res1(N),['R=',num2str(av_res)]);
drawnow;
%calculate the distance between the epipolar line and the corresponding points
%as a second quality estimation of the essential matrix
%the epipolar line is defined as l=E x_1 
for n=1:N
    %first point
    epi = E*[real(coord(:,1,n));1];
    distance  =abs(epi(1) * imag(coord(1,1,n)) + epi(2) * imag(coord(2,1,n)) +  epi(3)) / (sqrt(epi(1) * epi(1) + epi(2) * epi(2)));
    av_dist=av_dist+distance;
end
%this is the distance in normalized coordinates
av_dist=av_dist/N;
