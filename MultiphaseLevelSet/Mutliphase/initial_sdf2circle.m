function f = initial_sdf2circle(nrow,ncol, ic,jc,r,fun_n)
%   ������ž���Բ
%   input: 
%       nrow: number of rows
%       ncol: number of columns
%       (ic,jc): center of the circle
%       r: radius of the circle
%   output: 
%       f: signed distance to the circle
[X,Y] = meshgrid(1:ncol, 1:nrow);
a=5;b=6;
f=ones(nrow,ncol,fun_n);
for k=1:fun_n
    f(:,:,k) = -sqrt((X-k*jc/fun_n).^2+(Y-k*ic/fun_n).^2)+r/2;
end

%f=sdf2circle(100,50,51,25,10);figure;imagesc(f)

