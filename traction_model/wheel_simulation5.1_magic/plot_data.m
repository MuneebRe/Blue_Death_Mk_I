
% delete all variables
clear;

% close existing figure windows
close ALL;

% load array from file into matlab
% note that columns can be separated by space / tab or commas in matlab
A = load('sim.txt');

t  = A(:,1);
x1 = A(:,2); % w
x2 = A(:,3); % v
u = A(:,4); % tau
S = A(:,5); % S
mu = A(:,6); % mu

figure(1);
plot(t,x1);
ylabel('x1, wheel angular velocity, w (rad/s)');
xlabel('time (s)');

figure(2);
plot(t,x2);
ylabel('x2, wheel forward velocity, v (m/s)');
xlabel('time (s)');

figure(3);
plot(t,u);
ylabel('u, input, wheel torque (Nm)');
xlabel('time (s)');

figure(4);
plot(t,S);
ylabel('S, slip ratio');
xlabel('time (s)');

figure(5);
plot(t,mu);
ylabel('mu, tire friction coefficient');
xlabel('time (s)');
