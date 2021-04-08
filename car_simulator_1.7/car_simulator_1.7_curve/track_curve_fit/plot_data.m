
% delete all variables
clear;

% close existing figure windows
close ALL;

% load array from file into matlab
% note that columns can be separated by space / tab or commas in matlab
A = load('output.txt');

k = 1;
t  = A(:,k); k = k + 1;
x = A(:,k); k = k + 1;
y = A(:,k); k = k + 1;

k = 1;

figure(k); k = k + 1;
plot(t,x);
xlabel('curve length t (pixels)');
ylabel('track center line, x position (pixels)');

figure(k); k = k + 1;
plot(t,y);
xlabel('curve length t (pixels)');
ylabel('track center line, y position (pixels)');

figure(k); k = k + 1;
plot(x,y);
xlabel('track center line, x position (pixels)');
ylabel('track center line, y position (pixels)');
