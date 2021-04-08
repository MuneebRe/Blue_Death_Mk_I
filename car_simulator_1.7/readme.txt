
I added a new 3D car simulator (in car_simulator_1.7) based on 
the 3D graphics library from the previous lectures / assignment.
The car simulator includes a traction model, steering model,
and 3D graphics.

The included podcast lecture explains the model / simulation
and gives a demonstration.

The simulator is a good example of how to use the 3D graphics
library and simulation methods in order to simulate a car 
with various controllers.  This might help you develop
your own simulator (perhaps with additional capabilities, features,
and scenarios) as an optional topic for the project.
In that case you should try to write your own version as 
much as possible rather than modify my existing program.

The car simulator will also allow implementation and testing
of control software for the project that involves steering control,
autonomous driving, etc.  It can also be used for preliminary
implemention / testing of speed, launch, and braking control 
algorithms albeit without Arduino / register level programming.

Note that this is the last major update to the simulators
for the project.  Therefore, there is a HIL simulator to 
implement / test register level control software for 
speed / launch / braking control and a 3D graphics car 
simulator to implement / test other types of control software (you 
don't need to use register level programming for these other types).

It's possible to combine the HIL simulator with the 3D graphics
simulator as an optional part of the project.  This would be 
a fairly interesting / useful contribution.

I've also included an example (in car_simulator_1.7_curve)
that constructs a curve fit of the center line of the race track
(in track_curve_fit) and gives a demonstration of how to use 
the curve equation for the track (in car_simulator_curve).  
This equation could be useful for developing a steering 
controller for the project to guide the car around the track 
(as an optional topic).

The curve is based on a cubic spline curve fit of the center 
line of the track (obtained from a skeleton image processing 
function of the road using the "Fiji / ImageJ" discused earlier 
in the term). The cubic spline is continuous up to 
second derivative / acceleration which makes it useful for a 
wide variety of control methods that require continuity.

Anyhow, you could proceed by calculating whether the car is 
to the left or right of the curve (cross product, etc.)
and then turning the car the opposite direction.
Note that the curve function is very fast to compute so you 
can quickly compute all the points in the track in a for loop 
to calculate the minimum distance between the car and the track 
for any position of the car.
