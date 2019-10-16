function Delta_h = Delta(phi, epsilon)
% 计算光滑版本的Dirac函数
Delta_h=(epsilon/pi)./(epsilon^2+ phi.^2);

