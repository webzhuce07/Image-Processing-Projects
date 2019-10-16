function [C,mult]= quadrifit(phi,U,epsilon,fun_n) 
% 计算最优二值拟合C1, C2, C3, C4 

H = Heaviside(phi,epsilon); %compute the Heaveside function values 
s=size(H);
M=2^fun_n;
C=zeros(2^fun_n,1);
a=zeros(size(H));
mult=ones(s(1),s(2),fun_n,M/2*(fun_n-1));%mult 

mult(:,:,1,1)=1-H(:,:,2);%1-H(phi(2)):c0
mult(:,:,1,2)=H(:,:,2);%H(phi(2)):c1
mult(:,:,2,1)=1-H(:,:,1);%1-H(phi(1)):c2
mult(:,:,2,2)=H(:,:,1);%H(phi(1)):c3

mult2=zeros(s(1),s(2),4);%numerator
%%%phi-1,2,...n, c1...c2^n
mult2(:,:,1)=(1-H(:,:,1)).*(1-H(:,:,2));%00
mult2(:,:,2)=H(:,:,1).*(1-H(:,:,2));%01
mult2(:,:,3)=(1-H(:,:,1)).*(H(:,:,2));%10
mult2(:,:,4)=H(:,:,1).*H(:,:,2);%11
mult3=mult2;
for k=1:M
    mult2(:,:,k)= mult2(:,:,k).*U;
end
for k=1:M
    tmp1=mult3(:,:,k);
    tmp2=mult2(:,:,k);
    denum=sum(tmp1(:));
    num=sum(tmp2(:));
    C(k) = num/denum;
end


