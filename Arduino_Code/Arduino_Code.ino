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

int seqIndex = 0;
int seqLength = 0;
String recString = "";
//since dynamic arrays are not advised in Arduino, it is unlikely the user will get past 50
int seqArr[50];
int btnLog[50];
int btnLogIndex = 0;
String sendSequence = "";
String sendMorse = "";

//Status trackers
boolean displayingSequence = false;
boolean repeatingSequence = false;
boolean displayResultAnimation = false;
boolean creatingSequence = false;
boolean listeningToButtons = false;
boolean playerWasCorrect = true;

//customizable display, just set indexes appropriately in displayResultAnimation
int animationArr[] = {0,1,2,3,0,1,2,3,0,1,2,3,0,-1,0,-1,0,-1,0};
int animationIndex = -1;
int endI;
const int animationLEDonDuration = 100;
const int animationLEDoffDuration = 20;

//for tweaking how sequence it output to user
const int LEDonDuration = 800;
const int LEDoffDuration = 400;
unsigned long timeLEDlastChanged = 0;
boolean LEDon = false;


void setup() {
 
 Serial.begin(9600);

 for (int i = 0; i < 4; i++)
 {
  pinMode(btn[i], INPUT);
  pinMode(led[i], OUTPUT);
 }
 recString.reserve(400);
}


void loop() {

  if(displayingSequence)
  {
    //if an LED has been on and it's time to turn it off
    if(LEDon && (millis() - timeLEDlastChanged >= LEDonDuration))
    {
      digitalWrite(led[seqArr[seqIndex]], LOW);
      LEDon = false;
      seqIndex ++;

      //if the entire sequence has been output
      if(seqIndex == seqLength)
      {
        displayingSequence = false;
        repeatingSequence = true;
        listeningToButtons = true;
        for (int i = 0; i < seqLength; i ++)
        {
          btnLog[i] = -1;
        }
        btnLogIndex = 0;
        seqIndex = 0;
      }
      timeLEDlastChanged = millis();
    }

    //if the last LED was off and it's time to turn the next one on
    else if (!LEDon && (millis() - timeLEDlastChanged >= LEDoffDuration))
    {
      digitalWrite(led[seqArr[seqIndex]], HIGH);
      LEDon = true;
      timeLEDlastChanged = millis();
    }
    
  }
  
  else if(repeatingSequence)
  {
    //see if the user is done pressing buttons
    if(btnLogIndex == seqLength)
    {      
      //now check if correct answer was written
      //I am an optimist
      playerWasCorrect = true;
      for (int i  = 0; i < seqLength; i ++)
      {
        if(btnLog[i] != seqArr[i])
        {
          playerWasCorrect = false;
          break;
        }
      }
      displayResultAnimation = true;
      listeningToButtons = false;
      repeatingSequence = false;
      //clear button log
      for (int i = 0; i < seqLength; i ++)
          btnLog[i] = -1;
      btnLogIndex = 0;
      LEDon = false;
      animationIndex = -1;
    }
    
  }

  else if(displayResultAnimation)
  {
    //only set indices the first time
    if (animationIndex == -1)
    {
      if (playerWasCorrect)
      {
        animationIndex = 0;
        endI = 11;
      }
      else {
        animationIndex = 12;
        endI = 18;
      }
    }
    //if an LED has been on and it's time to turn it off
    if(LEDon && (millis() - timeLEDlastChanged >= animationLEDonDuration))
    {
      digitalWrite(led[animationArr[animationIndex]], LOW);
      LEDon = false;
      animationIndex ++;

      //if the entire sequence has been output
      if(animationIndex == (endI + 1))
      {
        displayResultAnimation = false;
        creatingSequence = true;
        listeningToButtons = true;
      }
      timeLEDlastChanged = millis();
    }

    //if the last LED was off and it's time to turn the next one on
    else if (!LEDon && (millis() - timeLEDlastChanged >= animationLEDoffDuration))
    {
      digitalWrite(led[animationArr[animationIndex]], HIGH);
      LEDon = true;
      timeLEDlastChanged = millis();
    }
  }
  
  else if(creatingSequence)
  {
    //since the index is incremented after the last button was logged
    if (btnLogIndex == seqLength)
    {
      for (int i = 0; i < seqLength; i ++)
      {
        sendSequence += String(btnLog[i]);
      }
      Serial.println(sendSequence);
      creatingSequence = false;
    }
  }


  //seperate from status blocks of code -------------------------------------
  if (listeningToButtons)
  {
    for (int i = 0; i < 4; i ++)
    {
      int cur = digitalRead(btn[i]);
    
      //just saves the time of last change
      if (cur != prevButtonState[i])
        prevChangeTime[i] = millis();
    
      //if enough time has passed that the button is being held down or has been released (intended state)
      if ((millis() - prevChangeTime[i]) > debounceTime)
      {
        if (cur != buttonState[i])
        {
          buttonState[i] = cur;
         
          //now that the new state has been officially changed, make appropriate change
          if (cur)
          {
            digitalWrite(led[i], HIGH);
          }
          else {
            //button released
            digitalWrite(led[i], LOW);
            btnLog[btnLogIndex] = i;
            btnLogIndex ++;
          }
        }
      }
    prevButtonState[i] = cur;
    }
  }

}//end loop




//expect morse code to start with . or - and sequences are in the form 12031021, BOTH END WITH ~
void serialEvent()
{
  while (Serial.available())
  {  
    char inChar = (char)Serial.read();
    recString += inChar;
    
    if (inChar == '~')
    {
      int endIndex = recString.indexOf("~");
      //start index = inclusive, end index = exlusive (this removes tilde)
      recString = recString.substring(0, endIndex);

      
      //PROCESS MORSE CODE
      if (recString.charAt(0) == '.' || recString.charAt(0) == '-') 
      {
        //Add to Queue
      }
   
      //PROCESS SEQUENCE  
      else 
      {      
        for (int i = 0; i < recString.length(); i ++)
        {
          seqArr[i] = recString.charAt(i) - '0';
        } 
       
        seqIndex = 0;
        seqLength = recString.length();
        displayingSequence = true;
        //erase as much of the button log as required to enter the correct sequence
        for (int i = 0; i < seqLength; i ++)
          btnLog[i] = -1;
        btnLogIndex = 0;
      }
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

