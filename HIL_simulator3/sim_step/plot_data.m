
% delete all variables
clear;

% close existing figure windows
close ALL;

% load array from file into matlab
% note that columns can be separated by space / tab or commas in matlab
temp = importdata('output.txt', ',', 1);
A = temp.data;
delete temp;

% file format
% time(s),x1,x2,x3,x4,x5

k = 1;
t  = A(:,k); k = k + 1;

x1 = A(:,k); k = k + 1;
x2 = A(:,k); k = k + 1;
x3 = A(:,k); k = k + 1;
x4 = A(:,k); k = k + 1;
x5 = A(:,k); k = k + 1;

k = 1;

figure(k); k = k + 1;
plot(t,x1);
ylabel('x1, current, i (A)');
xlabel('time (s)');

figure(k); k = k + 1;
plot(t,x2);
ylabel('x2, motor velocity, wm (rad/s)');
xlabel('time (s)');

figure(k); k = k + 1;
plot(t,x3);
ylabel('x3, motor theta (rad)');
xlabel('time (s)');

figure(k); k = k + 1;
plot(t,x4);
ylabel('x4, forward velocity v (m/s)');
xlabel('time (s)');

figure(k); k = k + 1;
plot(t,x5);
ylabel('x5, forward position x (m)');
xlabel('time (s)');
