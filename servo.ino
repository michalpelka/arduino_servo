    #define enA 9
    #define in1 6
    #define in2 7
    #define button 4
    #define CH1 3
    #define RANGE 10000
    const byte ledPin = 13;
    volatile byte state = LOW;
    int rotDirection = 0;
    int pressed = false;
    void setup() {
      pinMode(enA, OUTPUT);
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
      pinMode(button, INPUT);
      pinMode(CH1, INPUT);
      TCCR1B = TCCR1B & B11111000 | B00000001;
      // Set initial rotation direction
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      attachInterrupt(digitalPinToInterrupt(CH1), blink, CHANGE);
      
      Serial.begin(9600);
    }
    
    int counter =0;
    int value =0;
    int oldCh1;
    
    
    volatile int oldCh;
    volatile int microsStart=0;
    volatile int readout =0; 
    void blink() {
      volatile int ch = digitalRead(CH1);
      if (ch==1 && oldCh==0)
      {
        microsStart = micros();
      }
      if (ch==0 && oldCh==1)
      {
        //Faling
        readout = micros()- microsStart;
        
      }
      oldCh = ch;
      state = !state;
    }


    
    void loop() {
      
      int setPoint = map(readout, 1200, 1700, -RANGE, RANGE);
      //Serial.println(setPoint);
     
      int potValue = analogRead(A0); // Read potentiometer value    
      int input = map(potValue, 100, 900, -RANGE , RANGE);     
      //Serial.println(input);
     
      int error = input - setPoint;
   
      int output = 0;
      float kp  = 5;
      if (abs(error) < RANGE/kp)
      {
        output = error *kp;
      }
      else
      {
        if (error > 0) output = RANGE;
        if (error < 0) output = -RANGE;
        
      }
     
      //Serial.println(output);
     
      int pwmOutput = map(abs(output), 0, RANGE, 0 , 255); // Map the potentiometer value from 0 to 255
      //Serial.println(pwmOutput);
      analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin
      // Read button - Debounce

    
      char buffer [50];
      int i=sprintf (buffer, "%d %d \n", setPoint,input );
      for(int l= 0; l<=i; l++)
      Serial.print(buffer[l]);
      if (output > 50)
      {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
      }
      else if (output < -50)

      {
         digitalWrite(in1,HIGH );
        digitalWrite(in2, LOW);
      }
     
    }
