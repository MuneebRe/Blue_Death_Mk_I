
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


k = 1;

figure(k); k = k + 1;
plot(t,us);
ylabel('u_s, Voltage, V');
xlabel('time (s)');

figure(k); k = k + 1;
plot(xc,yc);
ylabel('yc');
xlabel('xc');

figure(k); k = k + 1;
plot(t,wb,t,wf);
ylabel('wheel speed');
xlabel('time');

figure(k); k = k + 1;
plot(t,r);
ylabel('Slip Ratio');
xlabel('time');

figure(k); k = k + 1;
plot(t,v);
ylabel('Forward Velocity');
xlabel('time');