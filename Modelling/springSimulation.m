k = 200;
% 
% hold on
% axis equal
% axis([0 0.1 -0.1 0.1])

% stationary

% plot([0 Lh], [0 0]) % handle
% plot([0 Lh], [0 -L]) % d
% plot([Lh Lh], [0 -L]) % Spring

% rotated

th2 = acosd((((x+L)^2)-d^2+Lh^2)/(2*(x+L)*Lh));
spring = [0 L 0 1]';
i = 0;

while (i <= 70)
    
    i = i + 0.5;
    
    handle = [cosd(i)*Lh, sind(i)*Lh];
    tf = make_tfMatrix(d, 0, -th2, 0);
    tspring = tf * spring;

    quiver([0 Lh], [0 -L], [handle(1) tspring(1)], [handle(2) tspring(2)]);
    axis equal
    axis([0 0.1 -0.1 0.1])

    p1 = [cosd(i)*Lh sind(i)*Lh]';
    p2 = [Lh -L]';

    x = norm(p1 - p2) - L;

    F = k * x;
    th2 = acosd((((x+L)^2)-d^2+Lh^2)/(2*(x+L)*Lh));
    
    drawnow
    pause(0.01)
    
end
