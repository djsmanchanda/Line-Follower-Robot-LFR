# Line-Follower-Robot
Code for a line follower robot based on Arduino Nano, 8 Channel QTR-8RA Analog Reflectance Sensor, MP1584 Buck Module, TB 6612 motor driver and N20 600 rpm 6V motors with 2 Li ion Cells for power

## Requirements 
Install SparkFun_TB6612.h library in your Arduino IDE

## Special Cases for Right angles and Obtuse Angles
![image](https://github.com/djsmanchanda/Line-Follower-Robot/assets/68808848/8c97ac30-4f76-47ce-afe9-9000ec3fb803)
Cells represent values given by QTR sensor, blank(blue) cells means that those values aren't considered in the test case, the cases are if - elseif cases.

##Advice
Please Calibrate the Kp Kd values thoroughly for optimal results, it took us over 5 hours of continuous changes for us to find the optimal values for our design, hopefully you will require less time.
Use a buck module in between the battery and motordriver, otherwise drop in battery will lead to variance between consecutive runs.
Print a customised track on a large poster, with 90 degree turns, obtuse angles, straight lines and curved semi circles(don't make it too tight) and dashes, and keep cleaning the track as dust can influence the sensor readings 

## Best of Luck
