function [c,h]=plotLevelSet(u,zLevel, style)
%   ���ƺ���u��zLevel������
% hold on;
[c,h] = contour(u,[zLevel zLevel],style); 
% hold off;

