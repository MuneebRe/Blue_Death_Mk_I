NOTE: The program should only be run in Release Mode!

1.Mandatory Requirements
For the mandatory project requirement, the three PID cases for the Arduino are located inside HIL3_PID_cases. However, you need an Arduino Mega 2560 for the controller to work (already had a Mega, Nano Every, 2x esp8266, and Teensy 3.5, but an old Arduino Uno and Arduino Nano were broken months ago). If a Mega is not available for testing, you can use the matlab to plot the output text files already prepared.

For the Car_Simulator, there are four PIDs you can test, located under controller.cpp:
VFF_Control (Steering)
speed_PID
traction_PID
brake_PID

Unlike the Arduino PID cases, the car simulator can run the speed controller with traction, brake, traction+brake. To enable those PIDs, change the following settings inside controller.cpp:

VFF_Feature = 1;
speed_PID_switch = 1;
traction_PID_switch = 1;
brake_PID_switch = 1;

Note that if the VFF_Feature is on, then the program will run slower, since the Vision part of the program is running on a seperate thread that takes longer than 0.001 to complete its image processing before merging to the main thread. However, you can add obstacles around the course like the white box and it'll avoid it perfectly. Use Image_View to view how the image is being processed.

To plot the data, use Plot2.m

2. Optional Requirements - Autonomous Steering with Virtual Force Field (VFF)
Check video BDMKI_VFF_DEMO.mp4

3. Optional Requirements - Building the Real Car
Check video BDMKI_REAL_CAR.mp4
Check full program in Blue_Death-Mk-I (Teesny 3.5 Code)

4. Optional Requirements - Serial Buffer
Using the progarm inside the Serial folder, it's possible to
-Make the car_simulator the controller for the HIL or sim_step [Arduino]
-Use the wb and wf from the HIL or sim_step [Arduino] and run the 3D simulator using those values.
-Combine both so it would look like the HIL is running in real time while also showing the simulation on the 3D graphics.
Basically, the Serial program is converting the character readings from the Serial Monitor into binary buffers that the car_simulator can read, since the car_sim does not have a console of it own (that's what the car_simulator_console_testing) is there for.

That's it.

Have fun!