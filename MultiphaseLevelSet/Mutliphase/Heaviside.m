function H = Heaviside(phi,epsilon) 
% ����⻬�汾��Heaviside ����
H = 0.5*(1+ (2/pi)*atan(phi./epsilon));
