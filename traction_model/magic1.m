
% F = N*D*sin(C*atan(B*r-E*(B*r−atan(B*r))))

% N = Fn -- normal force
% r = S -- slip ratio

% coefficient table from
% https://www.mathworks.com/help/physmod/sdl/ref/tireroadinteractionmagicformula.html
%
% Surface	B		C		D		E
% Dry		10		1.9		1		0.97
% Wet		12		2.3		0.82	1
% Snow		5		2		0.3		1
% Ice		4		2		0.1		1

clear;

x = 0:0.01:2;

n = max(size(x));

% dry road
B=10; C=1.9; D=1; E=0.97;
for k = 1:n
	r = x(k);
	Fd(k) = D*sin(C*atan(B*r-E*(B*r-atan(B*r))));
end

% wet road		
B=12; C=2.3; D=0.82; E=1;
for k = 1:n
	r = x(k);
	Fw(k) = D*sin(C*atan(B*r-E*(B*r-atan(B*r))));
end

% snow		
B=5;C=2;D=0.3;E=1;
for k = 1:n
	r = x(k);
	Fs(k) = D*sin(C*atan(B*r-E*(B*r-atan(B*r))));
end

% ice
B=4; C=2; D=0.1; E=1;
for k = 1:n
	r = x(k);
	Fi(k) = D*sin(C*atan(B*r-E*(B*r-atan(B*r))));
end

% BW model
% similar to Burckhardt model but with added term a2 for dip in curve

% these parameters can be readily estimated off-line or on-line
a1 = 1.63;
a2 = -0.9; % mainly for wet road dip in curve
a3 = -0.1;

% normally c1 and c2 should be constant for different road conditions
c1 = -27;
c2 = -15; 

rmax = 1.0;

for k = 1:n

	r = x(k);
	
	% limit r
	r = min(r,rmax);
	r = max(r,-rmax);
	
	if r < 0
		r = abs(r);
		Fbw(k) = -( a1 * (1-exp(c1*r)) + a2 * (1-exp(c2*r)) + a3 * r );
	else
		Fbw(k) = a1 * (1-exp(c1*r)) + a2 * (1-exp(c2*r)) + a3 * r;	
	end

end

plot(x,Fd,'b',x,Fw,'r',x,Fs,'g',x,Fi,'m',x,Fbw,'b--');

xlabel('S, slip ratio');
ylabel('mu, tire friction coefficient');
legend('dry','wet','snow','ice','BW approx');


