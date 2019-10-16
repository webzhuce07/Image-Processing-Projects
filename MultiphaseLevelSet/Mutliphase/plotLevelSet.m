function [c,h]=plotLevelSet(u,zLevel, style)
%   绘制函数u在zLevel的轮廓
% hold on;
[c,h] = contour(u,[zLevel zLevel],style); 
% hold off;

