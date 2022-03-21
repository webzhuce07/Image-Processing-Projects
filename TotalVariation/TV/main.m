clear all;
close all;
IterMax=10;lambda=0.03; 
%u0=imread('noisy.jpg');
pathForImages ='';
imageName = 'noisy.jpg';
[IMin0,pp]=imread(strcat([pathForImages,imageName]));
IMin0=im2double(IMin0);
if (length(size(IMin0))>2)
    IMin0 = rgb2gray(IMin0);
end
if (max(IMin0(:))<2)
    IMin0 = IMin0*255;
end

image=hpROF(IMin0,IterMax,lambda);

figure;
subplot(1,2,1);
imagesc(IMin0); axis image; axis off; colormap(gray);
title('原图像');   
subplot(1,2,2);
imagesc(image); axis image; axis off; colormap(gray);
title('全变法去噪图像'); 
PSNR=psnr(IMin0,image)
SSIM=ssim(IMin0,image)

tv(IMin0, 10)