function K = CURVATURE_CV(f,diff_scheme)
%º∆À„«˙¬ 
epsilon=1e-10;
if diff_scheme == 0  % the scheme in Chan and Vese's paper
    [fx,fy]=gradient(f);  % central difference
    %fx_f = Dx_forward(f);
    [nr, nc] = size(f);
    fx_f = zeros([nr,nc]);
    fx_f(1:nr-1,:) = f(2:nr,:)-f(1:nr-1,:);
    %%%
    ax = fx_f./sqrt(fx_f.^2+ fy.^2+epsilon);
    %axx = Dx_backward(ax);
    axx = zeros([nr,nc]);
    axx(2:nr,:) = ax(2:nr,:)-ax(1:nr-1,:);
    %%%
    %fy_f = Dy_forward(f);
    fy_f = zeros([nr,nc]);
    fy_f(:,1:nc-1) = f(:,2:nc)-f(:,1:nc-1);
    ay = fy_f./sqrt(fx.^2 + fy_f.^2 + epsilon);
    ayy = zeros([nr,nc]);
    ayy(:,2:nc) = ay(:,2:nc)-ay(:,1:nc-1);
    %ayy = Dy_backward(ay);  
    
    K = axx + ayy;
    
elseif diff_scheme == 1   % forward difference followed by a backward difference
    [nr, nc] = size(f);
    fx_f = zeros([nr,nc]);
    fx_f(1:nr-1,:) = f(2:nr,:)-f(1:nr-1,:);
    fy_f = zeros([nr,nc]);
    fy_f(:,1:nc-1) = f(:,2:nc)-f(:,1:nc-1);
    ax = fx_f./sqrt(fx_f.^2+ fy_f.^2+epsilon);
    ay = fy_f./sqrt(fx_f.^2 + fy_f.^2 + epsilon);  
    axx = zeros([nr,nc]);
    axx(2:nr,:) = ax(2:nr,:)-ax(1:nr-1,:);
    ayy = zeros([nr,nc]);
    ayy(:,2:nc) = ay(:,2:nc)-ay(:,1:nc-1);  
    K = axx + ayy;
elseif diff_scheme == 2   % central difference followed by a central difference    
    [fx, fy]= gradient(f); % central difference
    ax = fx./sqrt(fx.^2+ fy.^2+epsilon);
    ay = fy./sqrt(fx.^2 + fy.^2 + epsilon);    
    [axx, axy] = gradient(ax); % central difference
    [ayx, ayy] = gradient(ay);    
    K = axx + ayy;    
else
    disp('Wrong difference scheme: CURVATURE_CV.m');
    return;    
end
