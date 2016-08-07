 /*
Created by Geoff Spielman on 2016-06-25.
Copyright (c) 2016 Geoff Spielman. All rights reserved.
*/
const int btn0 = 2;
const int btn1 = 3;
const int btn2 = 4;
const int btn3 = 5;
const int led0 = 8;
const int led1 = 9;
const int led2 = 10;
const int led3 = 11;

int btn[] = {btn0, btn1, btn2, btn3};
int led[] = {led0, led1, led2, led3};

//debouncing purposes
unsigned long prevChangeTime[] = {0, 0, 0, 0};
int prevButtonState[] = {0,0,0,0};
int buttonState[] = {0,0,0,0};
unsigned long debounceTime = 50;

int seqIndex;
int seqSize;


boolean displayingSequence = false;
boolean repeatingSequence = false;
boolean creatingSequence = false;


/*
 String recString = "";
 boolean arduinoTurn = false;
 String recMessage = "";
 boolean LEDon = false;
 
 unsigned long timeOfLastChange;
 int seqArr[100];
 int repeatArr[100];

 boolean playerRepeating = false;
 boolean playerCreating = false;
 boolean prevBtns[] = {false, false, false, false};
*/

void setup() {
 
 Serial.begin(9600);
 pinMode(btn0, INPUT);
 pinMode(btn1, INPUT);
 pinMode(btn2, INPUT);
 pinMode(btn3, INPUT);
 pinMode(led0, OUTPUT);
 pinMode(led1, OUTPUT);
 pinMode(led2, OUTPUT);
 pinMode(led3, OUTPUT);
 
 //timeOfLastChange = millis();
 //recString.reserve(200);
 //seqIndex = 0;
}

void loop() {

for (int i = 0; i < 4; i ++)
{  
  //just incase something changes while executing (very unlikely, but I just read it once
  int cur = digitalRead(btn[i]);
  /*
  Serial.print("\tBtn");
  Serial.print(i);
  Serial.print(": ");
  Serial.print(cur);
  Serial.print(" (");
  Serial.print(prevButtonState[i]);
  Serial.print(")");
  Serial.print(prevChangeTime[i]);
  Serial.print("--");
  Serial.print(millis() - prevChangeTime[i]);
  */
  
  //just saves the time of last change
  if (cur != prevButtonState[i])
  {
    prevChangeTime[i] = millis();
  }

  //if enough time has passed that the button is being held down or has been released (intended state)
  if ((millis() - prevChangeTime[i]) > debounceTime)
  {
    if (cur != buttonState[i])
    {
      buttonState[i] = cur;
     
      //now that the new state has been officially changed, make appropriate change
      if (cur)
      {
        //button pressed down
        digitalWrite(led[0], HIGH);
        digitalWrite(led[1], LOW);
      }
      else {
        //button released
        digitalWrite(led[0], LOW);
        digitalWrite(led[1], HIGH);
      }
    }
  }
  prevButtonState[i] = cur;
}

//expect morse code to start with . or - and sequences are in the form 12031021, BOTH END WITH |
void serialEvent()
{
  while (Serial.available())
  {  
    char inChar = (char)Serial.read();
    recString += inChar;
    
    if (inChar == '|')
    {
      int endIndex = recString.indexOf("|");
      /*
      if (endIndex == -1)
      {  
        endIndex = recString.indexOf("/n");
      }
      */
      
      //PROCESS MESSAGE
      if (recString.charAt(0) == '.' || recString.charAt(0) == '-' {
        recMessage= recString.substring(2, endIndex);
        Serial.print("Received Message:");
        Serial.println(recMessage);
        
      //PROCESS SEQUENCE  
      }
      if (recString.charAt(0) == 's') {
        seqSize = (recString.length()-1)/2;
        /*
        int* seq = new int[(recString.length()-1)/2];
        Serial.println(sizeof(*seq));
        for (int i = 2; i < recString.length(); i++)
        {
        }
        */
        /*
        for (int i = 2; i <= (seqSize*2); i += 2)
        {
          seqArr[i/2 -1] = recString.charAt(i) - '0';
        }
        /*
        Verify that you can actually create arrays of variable size
        Serial.print("Created array size: ");
        Serial.println(sizeof(seqArr) / sizeof(int));
        
        //Test Sequence Array
        for (int i = 0; i < digits; i ++)
        {
          Serial.println(seqArr[i]);  
        }
        */
        /*
        seqIndex = 0;
        LEDon = false;
        for (int i = 0; i < 4; i ++)
          prevBtns[i] = false;
        arduinoTurn = true;
      }
      
      
      //Output sequence to user
      

      
      recString = "";
    }
  }
}






/*
  'all this does is read all the buttons and light them up when pressed
  if (playerCreating || playerRepeating)
  {
    'if button is pressed, turn on LED
    for (int i = 0; i < 4; i ++) {
      if (digitalRead(btn[i]) == 1) {
        digitalWrite(led[i], HIGH);
      }
      else {
        digitalWrite(led[i], LOW);
      }    
    }
  }
  
  if (arduinoTurn) {
    /*
     Serial.println("ITS MY TURN!!");
     Serial.print("seqIndex: ");
     Serial.println(seqIndex);
     Serial.print("seqSize: ");
     Serial.println(seqSize);
     */
     /*
     
     
    //If sequence index is less than the length, we are still outputting the sequence
    if((seqIndex < seqSize) && playerRepeating == false && playerCreating == false)
    {
      //If an LED was on and change must be made
      if (LEDon && (millis() - timeOfLastChange >= 1000))
      {
        digitalWrite(led[seqArr[seqIndex]], LOW);
        LEDon = false;
        seqIndex += 1;
        if (seqIndex == seqSize)
        {
          playerRepeating = true;
          for (int i = 0; i < 4; i ++)
            prevBtns[i] = false;
          seqIndex = 0;
        }
        timeOfLastChange = millis();        
        
      }
      //LED is off and change must be made
      else if (!LEDon && (millis() - timeOfLastChange >= 500))
      {          
          digitalWrite(led[seqArr[seqIndex]], HIGH);
          LEDon = true;
          timeOfLastChange = millis();
      }
      
    }
    else if (playerRepeating && (seqIndex < seqSize)) 
    {
      //Check each button for change
      for (int i = 0; i < 4; i ++)
      {
        'if the button wasn't pressed previously and is now
        if (!prevBtns[i] && digitalRead(btn[i]))
        {
          prevBtns[i] = true;
          delay(150);
        }
        else if(prevBtns[i] && !digitalRead(btn[i]))
        {
          prevBtns[i] = false;
          repeatArr[seqIndex] = i;
          seqIndex += 1;
          //Check user for correctness once the entire sequence has been entered
          if (seqIndex == seqSize)
          {
            boolean correct = true;
            for(int i = 0; i < 4; i ++)
            {
              if(seqArr[i] != repeatArr[i])
                correct = false;
            }
            if(correct)
            {
              //PLAYER CORRECT,prepare for create phase
              //Simple display
              for (int o = 0; o < 3; o ++)
              {
                for (int i = 0; i < 4; i ++)
                {
                  digitalWrite(led[i], HIGH);
                  delay(150);
                  digitalWrite(led[i], LOW);
                }
              }
              playerRepeating = false;
              playerCreating = true;
              seqIndex = 0;
              for (int i = 0; i < 4; i ++)
                prevBtns[i] = false;
            }
            else {
              for (int i = 0; i < 5; i ++)
              {
                digitalWrite(led[0], HIGH);
                delay(180);
                digitalWrite(led[0], LOW);
                delay(120);
              }

              //REMOVE THIS FOR MESSAGES, JUST FOR TESTING SEQUENCES
              playerRepeating = false;
              playerCreating = true;
              seqIndex = 0;
              for (int i = 0; i < 4; i ++)
                prevBtns[i] = false;
            }
            
          }
         }
      }
    }

    
    else if (playerCreating && (seqIndex < seqSize))
    {
      //Check each button for change
      for (int i = 0; i < 4; i ++)
      {
        if (!prevBtns[i] && digitalRead(btn[i]))
        {
          prevBtns[i] = true;
        }
        else if(prevBtns[i] && (digitalRead(btn[i]) == false))
        {
          prevBtns[i] = false;
          seqArr[seqIndex] = i;
          seqIndex += 1;
          if (seqIndex == seqSize)
          {
            //Convert int array to string
            String oString;
            for (int i = 0; i < seqSize; i++)
            {
              oString = String(oString + String(seqArr[i]));
            }
            Serial.println(oString);
            for (int i = 0; i < 4; i ++)
              digitalWrite(led[i], LOW);
            playerCreating = false;
            //TAKE THIS OUT LATER
            arduinoTurn = false;
          }
        }
      }
      
    }
    
    
     
    
    
    
    //arduinoTurn = false;
  }
  /*
  Serial.println(" ");
  Serial.print("Button0: ");
  Serial.print(digitalRead(btn0));
  Serial.print("   Button1: ");
  Serial.print(digitalRead(btn1));
  Serial.print("   Button2: ");
  Serial.print(digitalRead(btn2));
  Serial.print("   Button3: ");
  Serial.print(digitalRead(btn3));
  Serial.println(" ");
  */

  

}

/*
void serialEvent()
{
  while (Serial.available())
  {  
    char inChar = (char)Serial.read();
    recString += inChar;
    /*
    Serial.print("inChar: ");
    Serial.println(inChar);
    */
    /*
    if (inChar == '|')
    {
      int endIndex = recString.indexOf("|");
      if (endIndex == -1)
      {  
        endIndex = recString.indexOf("/n");
      }
      //PROCESS MESSAGE
      if (recString.charAt(0) == 'm') {
        recMessage= recString.substring(2, endIndex);
        Serial.print("Received Message:");
        Serial.println(recMessage);
        
      //PROCESS SEQUENCE  
      }
      if (recString.charAt(0) == 's') {
        seqSize = (recString.length()-1)/2;
        /*
        int* seq = new int[(recString.length()-1)/2];
        Serial.println(sizeof(*seq));
        for (int i = 2; i < recString.length(); i++)
        {
        }
        */
        /*
        for (int i = 2; i <= (seqSize*2); i += 2)
        {
          seqArr[i/2 -1] = recString.charAt(i) - '0';
        }
        /*
        Verify that you can actually create arrays of variable size
        Serial.print("Created array size: ");
        Serial.println(sizeof(seqArr) / sizeof(int));
        
        //Test Sequence Array
        for (int i = 0; i < digits; i ++)
        {
          Serial.println(seqArr[i]);  
        }
        */
        /*
        seqIndex = 0;
        LEDon = false;
        for (int i = 0; i < 4; i ++)
          prevBtns[i] = false;
        arduinoTurn = true;
      }
      
      
      //Output sequence to user
      

      
      recString = "";
    }
  }
}

/*
const int led = 13;
const int mtrL = 6;
const int mtrR = 5;

int mL = 0, mR = 0;

String inputString = "";
boolean needChange = false;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  inputString.reserve(200);
  Serial.println("-----Start-----");
}

void loop() {

  if (needChange)
  {
    //Serial.print("Arduino rec: ");
    //Serial.println(inputString);
    Serial.print("Left Motor: ");
    Serial.print(mL);
    Serial.print("   Right Motor: ");
    Serial.println(mR);

    analogWrite(mtrL, mL);
    analogWrite(mtrR, mR);
    inputString = "";
    needChange = false;
  }
}

void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n' || inChar == '!') //if 'end with newline' is not enabled on serial monitor, end with !
    {
      int commaIndex = inputString.indexOf(",");
      int endIndex = inputString.indexOf("!");
      if (endIndex == -1)
      {  
        endIndex = inputString.indexOf("/n");
      }
      mL = inputString.substring(0, commaIndex).toInt();
      mR = inputString.substring(commaIndex + 1, endIndex).toInt();
      needChange = true;
    } 
  }
}
*/

