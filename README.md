# arduino_servo
Simple DC Motor Servo using Arduino



[![particle_filter_cuda](https://img.youtube.com/vi/j1zosktqNGo/0.jpg)](https://www.youtube.com/watch?v=j1zosktqNGo)

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
Arduino sends over port control setpoint and measurment. Thus it can be plotted.
### Step Response
![alt text](https://raw.githubusercontent.com/michalpelka/arduino_servo/master/plots/step.png)
### Sinusoidal Response
![alt text](https://raw.githubusercontent.com/michalpelka/arduino_servo/master/plots/sin.png)

# Issues
If you experience constant movement, please check if feedback (rotary potentiometer is connected in right direction).

# Road Map
- Support for multiple RC channels (max 2), maybe SBUS
- Digital outputs


