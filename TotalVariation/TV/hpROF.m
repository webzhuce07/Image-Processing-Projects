function u=hpROF(u0,IterMax,lambda)
u0=double(u0);  %��ԭͼ��ת��Ϊ˫���ȸ�����
[M N]=size(u0); 
u=u0;
[M,N]=size(u);  %���������߶ȣ�,��������ȣ���
h=1;%  �ռ���ɢ
for Iter=1:IterMax,    %����
    
    for i=2:M-1,       
        for j=2:N-1,     
            %ϵ������
            ux=(u(i+1,j)-u(i,j))/h;
            uy=(u(i,j+1)-u(i,j-1))/2*h;
            Gradu=sqrt(ux*ux+uy*uy);
            co1=1./Gradu;
            
            ux=(u(i,j)-u(i-1,j))/h;
            uy=(u(i-1,j+1)-u(i-1,j-1))/2*h;
            Gradu=sqrt(ux*ux+uy*uy);
            co2=1./Gradu;
            
            ux=(u(i+1,j)-u(i-1,j))/2*h;
            uy=(u(i,j+1)-u(i,j))/h;
            Gradu=sqrt(ux*ux+uy*uy);
            co3=1./Gradu;
            
            ux=(u(i+1,j-1)-u(i-1,j-1))/2*h;
            uy=(u(i,j)-u(i,j-1))/h;
            Gradu=sqrt(ux*ux+uy*uy);
            co4=1./Gradu;
            u(i,j)=(u0(i,j)+(1/lambda*h*h)*(co1*u(i+1,j)+co2*u(i-1,j)+co3*u(i,j+1)+co4*u(i,j-1)))*(1/(1+(1/(lambda*h*h)*(co1+co2+co3+co4))));
%             co=1./(1.+(1/lambda*h*h)*(co1+co2+co3+co4));
%             div=co1*u(i+1,j)+co2*u(i-1,j)+co3*u(i,j+1)+co4*u(i,j-1);
%             u(i,j)=(u0(i,j)+(1/lambda*h*h)*div)*co;
           
        end
    end
    %��Ե����
    for i=2:M-1,   
        u(i,1)=u(i,2);    %��һ�еļ�����ڵڶ���
        u(i,N)=u(i,N-1);  %���һ�еļ�����ڵ����ڶ���
    end
    
    for j=2:N-1,   
        u(1,j)=u(2,j);
        u(M,j)=u(M-1,j);  %ͬ����һ�к����һ�еļ�����ں����������
    end
    
    u(1,1)=u(2,2);
    u(1,N)=u(2,N-1);
    u(M,1)=u(M-1,2);
    u(M,N)=u(M-1,N-1);    %�ĸ��ǵ�ֵҲ������������ĵ�
    %����ÿ�ε�������ɢ����
    en=0.0;   %��������ʼֵΪ0
    for i=2:M-1,
        for j=2:N-1,
            ux=(u(i+1,j)-u(i,j))/h;
            uy=(u(i,j+1)-u(i,j))/h;
            fid=(u0(i,j)-u(i,j))*(u0(i,j)-u(i,j));
            en=en+sqrt(ux*ux+uy*uy)+lambda*fid;
        end
    end
    %����ÿ�ε���������
    Energy(Iter)=en;
end
%����ÿ�ε�����������ͼ
figure
plot(Energy);legend('Energy/Iterations');
