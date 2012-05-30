% FundMat - compute fundamental matrix using SVD
% F = fundmat(Q1,Q2)
% Q1, Q2 (N x 3) matrices of homogenous coordinates 
% coordinates
% R. Hartley normalization of correspondences is incorporated
%			 In Defence of thew 8-point Algorithm, ICCV95
%
% See: FUNDMAT2
%
% T. Svoboda, CMP Prague, 1/1997, svoboda@cmp.felk.cvut.cz

function [F,A] = fundmat(Q1,Q2)
NumPoints = size(Q1,1);

if size(Q1,2)==2,		% not homogenous
   Q1 = [Q1, ones(size(Q1(:,1)))];
   Q2 = [Q2, ones(size(Q2(:,1)))];
end;

%%%%%%%%% Normalization %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
% Normalization: The coordinates should be in average (1,1,1)
% Isotrophic normalization is used the same ratios is used
% for x-coor either for y-coordinate
% normalization by R. Hartley: In Defence of the 8-point Algorithm, ICCV95
%%
Xmean1 = mean(Q1(:,1));
Ymean1 = mean(Q1(:,2));
Xmean2 = mean(Q2(:,1));
Ymean2 = mean(Q2(:,2));

%%%
% Translation to the origin
%%%
Q1(:,1:2) = [Q1(:,1)-Xmean1 Q1(:,2)-Ymean1];
Q2(:,1:2) = [Q2(:,1)-Xmean2 Q2(:,2)-Ymean2];

Distances1 = sqrt( (Q1(:,1).^2 + Q1(:,2).^2) );
Distances2 = sqrt( (Q2(:,1).^2 + Q2(:,2).^2) );

Ratio1 = mean(Distances1)/sqrt(2);
Ratio2 = mean(Distances2)/sqrt(2);

%%
% compute transformation matrices Po'=T*Po
%%
T1      = diag([1/Ratio1,1/Ratio1,1]);
T2      = diag([1/Ratio2,1/Ratio2,1]);
T1(1,3) = -T1(1,1)*Xmean1;
T1(2,3) = -T1(2,2)*Ymean1;
T2(1,3) = -T2(1,1)*Xmean2;
T2(2,3) = -T2(2,2)*Ymean2;

%%%
% Isotrophic scaling
%%%
Q1(:,1:2) = Q1(:,1:2)/Ratio1;
Q2(:,1:2) = Q2(:,1:2)/Ratio2;

%%%%%%%% End of normalization %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

A = zeros(NumPoints,9);

for i=1:NumPoints                             % create equations
 for j=1:3
  for k=1:3
   A(i,(j-1)*3+k)=Q2(i,j)*Q1(i,k);   
  end
 end
end

[U,S,V]  = svd(A);
f        = V(:,size(V,2));
F        = reshape(f,3,3)';
F		 = T2' * F * T1;		% undo normalization
return



