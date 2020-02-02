function M = getMomentOnHandle(L, k)

% L = length between base of spring and handle
% d = distance between base of hadle and base of spring
% k = spring constant

% Distance between the base of the handle and its middle in meters
Lh = 0.0543;  

% Max turning angle for the handle
maxAngle = 70;  

d = sqrt(L^2+Lh^2);

x = sqrt(Lh^2+d^2-2*d*Lh*cosd(atand(L/Lh)+maxAngle)) - L;
F = k * x;
th2 = acosd((((x+L)^2)-d^2+Lh^2)/(2*(x+L)*Lh));
M = F * sind(th2) * Lh;
