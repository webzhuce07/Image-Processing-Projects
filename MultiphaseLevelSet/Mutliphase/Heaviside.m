function H = Heaviside(phi,epsilon) 
% 计算光滑版本的Heaviside 函数
H = 0.5*(1+ (2/pi)*atan(phi./epsilon));
