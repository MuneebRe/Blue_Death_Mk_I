The PID tests are set up for
Speed PID
Speed PID + Brake PID
Speed PID + Brake PID + Traction PID

It is not possible to test one at a time anymore since the brake PID can only run before the traction PID + the brake PID is triggered depending on the target velocity and the current velocity that was taken care of from the speed PID + the brake PID and traction PID need each other to know which one of those is triggered.

timer1 is set to 20 ms period with 1500-2500 microseconds square signals.
ADC timer is set to 60 seconds reading without polling per analog pin.
the PIDs are run at 0.003 seconds interval when dt = 0.001 seconds.

The test runs for each case scenario are recorded in their own text file, to open them, change them into "output.txt" or enter their name inside the matlab file.

The final PID results are also shown inside the final report.

Have fun!