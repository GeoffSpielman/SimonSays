 /*
Created by Geoff Spielman on 2016-06-25.
Copyright (c) 2016 Geoff Spielman. All rights reserved.
*/
#include <QueueList.h>

//Hardware
const int btn0 = 2;
const int btn1 = 3;
const int btn2 = 4;
const int btn3 = 5;
const int led0 = 8;
const int led1 = 9;
const int led2 = 10;
const int led3 = 11;

const int morLed = 12;
const int morBtn = 7;
const int morBuz = 6;


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

//The queue for the morse code
QueueList<int> morQue;

/*According to Wikiepedia:
 * Length of dash is three times the length of a dot
 * Space between dash or dot is equal to the length of a dot
 * letters are seperated by a pause equal to the length of a dash (three dots)
 * words are seperated by 7 dots
 * For our application, we will say the space between messages is 12 dots
 */
//morse code parameters
int morState = 0;
unsigned long morLength = 0;
unsigned long morStart = 0;
boolean morseAvailable = false;
const int dot = 300;
const int buzzerLevel = 8; //18 is pretty good
boolean morINlast = false;
boolean morINstate = false;
unsigned long morINlastChange = 0;
unsigned long morTimePressed = 0;
unsigned long morTimeReleased = 0;
String morMessage = "";


//Status trackers
boolean displayingSequence = false;
boolean repeatingSequence = false;
boolean displayResultAnimation = false;
boolean creatingSequence = false;
boolean playerWasCorrect = true;
boolean listeningToButtons = false;
boolean phoneUserLost;

//customizable display, just set indexes appropriately in displayResultAnimation
int animationArr[] = {2,-1,2,-1,2,-1,-1,0,-1,0,-1,0,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0};
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
        resetButtonLog();
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
      repeatingSequence = false;
      listeningToButtons = false;
      LEDon = false;
      animationIndex = -1;
    }
    
  }

  else if(displayResultAnimation)
  {
    //only set indices the first time
    if (animationIndex == -1)
    {
      if(phoneUserLost)
      {
        animationIndex = 12;
        endI = 28;
      }
      else
      {
        if (playerWasCorrect)
        {
          animationIndex = 0;
          endI = 5;
        }
        else {
          animationIndex = 6;
          endI = 11;
        }
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
        if (playerWasCorrect && !phoneUserLost) {
          creatingSequence = true;
          listeningToButtons = true;
          resetButtonLog();
        }
        else if (!phoneUserLost) {
            Serial.println(9);

          displayResultAnimation = false;
          playerWasCorrect = true;
          
        }
        //get whole game ready to go again
        else if (phoneUserLost)
        {
          animationIndex = -1;
          phoneUserLost = false;
          playerWasCorrect = true;
          displayResultAnimation = false;
        }
        
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
      sendSequence = "";
      creatingSequence = false;
      listeningToButtons = false;
    }
  }
  

  //-----------------------------------------------------------------------------------------------
  //----------------------seperate from status blocks of code -------------------------------------
  //-----------------------------------------------------------------------------------------------
  if (listeningToButtons){
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

  //Morse Code button specifically (always listening)
    boolean curINmor = digitalRead(morBtn);
    if (curINmor != morINlast)
      morINlastChange = millis();
    if (((millis() - morINlastChange) > debounceTime) && (curINmor != morINstate))
    {
      morINstate = curINmor;    
      if (curINmor)
      {
        morTimePressed = millis();
        int diff = morTimePressed - morTimeReleased;
        if (diff >= (2 * dot) && diff < (5 * dot)) //between letters is 3 dots
            morMessage += " ";
        else if (diff >= (5 * dot) && diff < (10 * dot)) //between words is 7 dots
            morMessage += "|";
      }
      else {
        morTimeReleased = millis();
        int diff = morTimeReleased - morTimePressed;
        if (diff < (1.75 * dot)) //single dot
            morMessage += ".";
        else if (diff >= (1.75 * dot)) //dash
            morMessage += "-";
      }
    }
    morINlast = curINmor;
    

  //always see if enough time has passed to send a message
  if (morMessage != "" && (millis() - morTimeReleased > (dot * 11)) && !curINmor) //using 11 as an offest
  { 
    Serial.println(morMessage);
    morMessage = "";
  }


  
  //if we're not in bypass mode and a change needs to be made
  if (morseAvailable && (millis() - morStart > morLength))
  {  
    //if the state is currently on
    if (morState)
    {
       morState = 0;
       digitalWrite(morLed, LOW);
       analogWrite(morBuz, 0);
     
       
      //if the next character is another dit or dah, you must wait one dot duration worth
      if (morQue.peek() % 10)
        morLength = dot;
      //otherwise next is a pause of some sort
      else
      { 
        morLength = (morQue.pop() / 10) * dot;       
       //we have run out of morse code
        if(morQue.isEmpty())
         morseAvailable = false;
      }
    }
    //just finished a pause, so no matter what we need the next character
    else{
      morState = 1;
      digitalWrite(morLed, HIGH);
      analogWrite(morBuz, buzzerLevel);
      morLength = (morQue.pop() / 10) * dot;
    }
   morStart = millis();
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

      //PROCESS MORSE CODE (space between letters, pipe between words,? after message)
      if (recString.charAt(0) == '.' || recString.charAt(0) == '-') 
      {
        //Add to Queue
        //Last digit is if it should be on (1) or off(0)
        //All other digits are how many units to perform the action
        for (int i = 0; i < recString.length(); i++)
        {
          switch(recString.charAt(i)){
            case '.':
              morQue.push(11);
              break;
            case '-':
              morQue.push(31);
              break;
            case ' ':
              morQue.push(30); //space between letters
              break;
            case '|':
              morQue.push(70); //space between words
              break;
          }
        }
        morQue.push(120); //space between messages (arbitrary, in case there are multiple messages)
        morseAvailable = true;
      }
   
      //PROCESS SEQUENCE  
      else 
      {  
        if (recString.charAt(0) == '9')
        {
          phoneUserLost = true;
          animationIndex = -1;
          displayResultAnimation = true;
        }
        else {
          for (int i = 0; i < recString.length(); i ++)
          {
            seqArr[i] = recString.charAt(i) - '0';
          } 
         
          seqIndex = 0;
          seqLength = recString.length();
          displayingSequence = true;
          //erase as much of the button log as required to enter the correct sequence
          resetButtonLog();
          }
      }
      recString = "";
    }
  }
}


void resetButtonLog(){
  for (int i = 0; i < seqLength; i ++)
    btnLog[i] = -1;
  btnLogIndex = 0;
}
