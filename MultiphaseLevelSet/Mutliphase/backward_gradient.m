function [bdy,bdx]=backward_gradient(f)
%Ïòºó²î·Ö
[nr,nc]=size(f);
bdx=zeros(nr,nc);
bdy=zeros(nr,nc);

bdx(2:nr,:)=f(2:nr,:)-f(1:nr-1,:);
bdy(:,2:nc)=f(:,2:nc)-f(:,1:nc-1);


