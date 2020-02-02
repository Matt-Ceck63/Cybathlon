% 20 kg /cm
% 20 * 9.8 N / cm
Mass = 20;
Torque = Mass*9.8*0.01; % N/m

distance = 0:0.01:1;

Force = Torque./distance;  % distance in meters

plot(distance, Force);

% The force is the amount of force exerted by the motor at that distance, 
% use that as a starting point to calculate moments in the system.
% Point at which the motor generates the most moment is closest to the
% shaft. A belt mechanism would better suit the application because the 
% pulley has a short radius (0.1 m) meaning that the motor can apply more
% torque to the belt. The belt then transfers that power away to the joint
% at any given distance without loss.
% A stick (1 m) will reduce the force to its minimum losing the advantage
% of getting power close from the motor.