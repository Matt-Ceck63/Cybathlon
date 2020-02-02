% Spring rate  2 x 9.8 / 0.015
k = 1306.6;
% Distance between the base of the handle and its middle in meters
Lh = 0.05455;  
% Max turning angle for the handle
maxAngle = 70;  
% Distance between base of spring and handle
L = 0.1;

% Calculating values
d = sqrt(L^2+Lh^2);
x = sqrt(Lh^2+d^2-2*d*Lh*cosd(atand(L/Lh)+maxAngle)) - L;
F = k * x;
th2 = acosd((((x+L)^2)-d^2+Lh^2)/(2*(x+L)*Lh));
M = F * sind(th2) * Lh;

% Origin

P1 = [0 0];
P3 = [Lh -L];
line([P1(1) P3(1)], [P1(2) P3(2)]);

% Parameters of the plot
axis(gca,'equal');
axis([0 0.2 -0.1 0.1]);

% angular speed of the handle
th_dot = 1;

for t=1:0.1:maxAngle
    
    theta = th_dot * (t);
    
    P2 = Lh*[cosd(theta) sind(theta)];
    handle = line([P1(1) P2(1)], [P1(2) P2(2)]);
    
    % Extension of spring
    x = norm(P2 - P3) - L;
    
    % P4 with rotation matrix
    % Spring same length change direction
    th2 = acosd((((x+L)^2)-d^2+Lh^2)/(2*(x+L)*Lh));
    c = 90 - 45 - (180 - (theta + 45) - th2);
    
    % Composite translation*rotation*rotation, define point, translate to
    % origin, rotate around origin, translate back
    P4 = [1 0 Lh; 0 1 -L; 0 0 1]*[cosd(c) -sind(c) 0;sind(c) cosd(c) 0; 0 0 1]*[1 0 -Lh; 0 1 L; 0 0 1]*[Lh 0 1]';
    
    spring = line([P3(1) P4(1)], [P3(2) P4(2)]);
    extension = line([P4(1) P2(1)], [P4(2) P2(2)], 'color', 'red');
    
    
    % Normal force vector to the handle
    F = k * x * [sind(theta) -cosd(theta)];
    X = (P2 + F);
    
    force = line([P2(1) X(1)], [P2(2) X(2)]);
    
    pause(0.01);
    
    % Delete the previous line
    if t < 70
        delete(handle);
        delete(spring);
        delete(force);
        delete(extension);
    end
    
end

Moment = norm(F) * Lh;






