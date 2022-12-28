 %% Restore an degraded Image (noising and blurring) by TV method. 
 %% The numerical implementation is based on Roe upwind schema
 
clear all;
close all;
clc;

Img = imread('lenna.bmp');
Img = double(Img);
figure(1); imshow(uint8(Img)); title('Origin');
[nrow, ncol] = size(Img);
% blurring image
Img = gauss(Img,7,3);
% add noise to the blurred image
I0=Img+10*randn([nrow, ncol]);
figure(2); imshow(uint8(I0));title('Noise');


lamda=0.2;        % This is a important parameter to get a better result
timestep=0.01;

I_temp=I0;        % Initialization

%Iteration begin
for n=1:600
   
       Ix = 0.5*(I_temp(:,[2:ncol,ncol])-I_temp(:,[1,1:ncol-1]));
       Iy = 0.5*(I_temp([2:nrow,nrow],:)-I_temp([1,1:nrow-1],:));
       Ix_back = I_temp-I_temp(:,[1,1:ncol-1]);
       Ix_forw = I_temp(:,[2:ncol,ncol])-I_temp;
       Iy_back = I_temp-I_temp([1,1:nrow-1],:);
       Iy_forw = I_temp([2:nrow,nrow],:)-I_temp;
       grad = Ix.^2+Iy.^2+eps;
       
       Ixx = I_temp(:,[2:ncol,ncol])+I_temp(:,[1,1:ncol-1])-2*I_temp;
       Iyy = I_temp([2:nrow,nrow],:)+I_temp([1,1:nrow-1],:)-2*I_temp;
       Ixy = 0.25*(I_temp([2:nrow,nrow],[2:ncol,ncol])+I_temp([1,1:nrow-1],[1,1:ncol-1])-...
           I_temp([2:nrow,nrow],[1,1:ncol-1])-I_temp([1,1:nrow-1],[2:ncol,ncol]));
       term1 = (Ixx.*Iy.^2-2*Ix.*Iy.*Ixy+Iyy.*Ix.^2)./grad;
       
       D=gauss(I_temp,7,3)-I0;
       DD = gauss(D,7,3);
       upwind_x=Ix_back;
       upwind_x(Ix.*DD<0)=Ix_forw(Ix.*DD<0);
       upwind_y=Iy_back;
       upwind_y(Iy.*DD<0)=Iy_forw(Iy.*DD<0);
       term2 = lamda*sqrt(upwind_x.^2+upwind_y.^2).*DD;
       I_t = term1-term2;

       I_temp=I_temp+timestep*I_t;
end
figure(3);imshow(uint8(I_temp));title('TV');

