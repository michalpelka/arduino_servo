    #define enA 9
    #define in1 6
    #define in2 7
    #define button 4
    #define RELAY 12
    #define CH1 3
    #define CH2 2    
    #define RANGE 1000
    
    void setup() {
      pinMode(enA,    OUTPUT);
      pinMode(in1,    OUTPUT);
      pinMode(in2,    OUTPUT);
      pinMode(RELAY,  OUTPUT);
      pinMode(button, INPUT);
      pinMode(CH1,    INPUT);
      pinMode(CH2,    INPUT);
      TCCR1B = TCCR1B & B11111000 | B00000001;
      // Set initial rotation direction
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      attachInterrupt(digitalPinToInterrupt(CH1), ch1_interrupt, CHANGE);
      attachInterrupt(digitalPinToInterrupt(CH2), ch2_interrupt, CHANGE);
      
      Serial.begin(9600);
    }
    
    int counter =0;
    int value =0;
    int oldCh1;
    
    
    volatile unsigned int oldCh[2];
    volatile unsigned int microsStart[2]={0,0};
    volatile unsigned int readout[2]={0,0}; 
    volatile bool isPPMOk = false;
    volatile unsigned int valid_frames = 0;
    
    void ch1_interrupt() {
      volatile int ch = digitalRead(CH1);
      if (ch==1 && oldCh[0]==0)
      {
        microsStart[0] = micros(); 
      }
      if (ch==0 && oldCh[0]==1)
      {
        //Faling
        readout[0] = micros()- microsStart[0];
        valid_frames ++;

      }
      if (readout[0] > 2500)
      {
        valid_frames = 0;
        isPPMOk = false;
      }
      else
      {
        if (valid_frames > 50)
        {
          isPPMOk = true;
        }
      }
      oldCh[0] = ch;
    }
    void ch2_interrupt()
    {
      volatile int ch = digitalRead(CH2);
      if (ch==1 && oldCh[1]==0)
      {
        microsStart[1] = micros(); 
      }
      if (ch==0 && oldCh[1]==1)
      {
        //Faling
        readout[1] = micros()- microsStart[1];
      }

      oldCh[1] = ch;
      
    }

    int lastSetpoint = 0;
    
    char buffer [50];
    void loop() {
      // input PPM smoothing
      int setPoint = map(readout[0], 1000, 2000, -RANGE, RANGE);
      if (setPoint > RANGE) setPoint = RANGE;
      if (setPoint < -RANGE) setPoint = -RANGE;
      
      int setPointSmooth = lastSetpoint + (-lastSetpoint+setPoint)/10;
      lastSetpoint = setPointSmooth;



      // potientometer
      int potValue = analogRead(A1); // Read potentiometer value    
      int input = map(potValue, 100, 900, -RANGE , RANGE);     
      
      // error and P controller
      int error = -input + setPointSmooth;
      int output = 0;
      float kp  = 2;
      if (abs(error) < RANGE/kp)
      {
        output = error *kp;
      }
      else
      {
        if (error > 0) output = RANGE;
        if (error < 0) output = -RANGE;
        
      }
      bool is_ok = false;
      
      
      if (isPPMOk)
      {
        int pwmOutput = map(abs(output), 0, RANGE, 0 , 255);
        //Serial.println(pwmOutput);
        analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin
        // Read button - Debounce
    
        //int i=sprintf (buffer, "%d %d \n", setPoint,input );
        //for(int l= 0; l<=i; l++)
        //  Serial.print(buffer[l]);
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
      else
      {
        digitalWrite(in2, LOW);
        digitalWrite(in1, LOW);
      }

      // Relay
      if(readout[1]>1850 and readout[1] < 3000)
      {
         digitalWrite(RELAY, LOW);
      }else
      {
         digitalWrite(RELAY, HIGH);
      }
        
      // logging
      
      int i=sprintf (buffer, "%d %d %d %d %d %d, %d \n",readout[0],readout[1], setPoint,setPointSmooth, output,  input, isPPMOk);
      for(int l= 0; l<=i; l++)
        Serial.print(buffer[l]);

      
    }
