
% delete all variables
clear;

% close existing figure windows
close ALL;

% load array from file into matlab
% note that columns can be separated by space / tab or commas in matlab
A = load('output.txt');

% file format
% time(s),xc,yc,us,u_phi

k = 1;
t  = A(:,k); k = k + 1;

us = A(:,k); k = k + 1;
xc = A(:,k); k = k + 1;
yc = A(:,k); k = k + 1;
wb = A(:,k); k = k + 1;
wf = A(:,k); k = k + 1;
r = A(:,k); k = k + 1;
v = A(:,k); k = k + 1;
mu = A(:,k); k = k + 1;
target_vf = A(:,k); k = k + 1;
target_r = A(:,k); k = k + 1;
target_VFF = A(:,k); k = k + 1;
current = A(:,k); k = k + 1;

k = 1;

figure(k); k = k + 1;
plot(t,us);
ylabel('u_s - Voltage [V]');
xlabel('time [s]');

figure(k); k = k + 1;
plot(xc,yc);
ylabel('yc [m]');
xlabel('xc [m]');

figure(k); k = k + 1;
plot(t,wb,t,wf);
ylabel('wheel speed [rad/s]');
xlabel('time [s]');
legend('back wheel velocity wb','forward wheel velocity wf');

figure(k); k = k + 1;
plot(t,r, t, target_r);
ylabel('Slip Ratio');
xlabel('time [s]');
legend('Current Slip Ratio','Target Slip Ratio');

figure(k); k = k + 1;
plot(t,v,t,target_vf);
ylabel('Forward Velocity [m/s]');
xlabel('time [s]');
legend('Forward Velocity','Target Velocity');

figure(k); k = k + 1;
plot(t,mu);
ylabel('mu');
xlabel('time [s]');

figure(k); k = k + 1;
plot(t,target_VFF);
ylabel('Steer Error [rad]');
xlabel('time [s]');

figure(k); k = k + 1;
plot(t,current);
ylabel('Motor Current [Amps]');
xlabel('time [s]');
