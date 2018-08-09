# arduino_servo
Simple DC Motor Servo using Arduino

## Motivation

Building very, very, very large RC Servo

## Input signal
Standard PPM signal (like classic RC Servo)
- D1 Output of servo tester or RC reciever

## Output
Driving signal for H-Bridge
D9 - PWM output
D6 - direction Left
D7 - direction Right

## Feedback
Rotary Potentiometr
A1 - signal from rotary potentiometer

## Regulation
ch1_interrupt is called when state of reciever input is changed. Than pulse width is measured. That value is mapped to input value of P controller. That P controller works output mapped to PWM generator and two direction output. Those outputs are controlling motor H-Bridge IC (like VNH 5019)

## Plots
Arduino sends over port controll setpoint and measurment. Thus it can be plotted.

# Issues
If you experience contant movement, please check if feedback (rotary encoder is connected in right direction).


