/*
Created by Geoff Spielman on 2016-06-25.
Copyright (c) 2016 Geoff Spielman. All rights reserved.
*/

 // put your setup code here, to run once:
  const int btn0 = 2;
  const int btn1 = 7;
  const int btn2 = 12;
  const int btn3 = 8;
  const int led0 = 5;
  const int led1 = 3;
  const int led2 = 4;
  const int led3 = 10;

 int btn[] = {btn0, btn1, btn2, btn3};
 int led[] = {led0, led1, led2, led3};

 String recString = "";
 boolean newAvail = false;
 String recMessage = "";

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
 recString.reserve(200);
 Serial.println("-----Start-----");
 
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 4; i ++) {
    if (digitalRead(btn[i]) == 1) {
      digitalWrite(led[i], HIGH);
      //Serial.print("LED ");
      //Serial.print(i);
      //Serial.print(" on    ");
    }
    else {
      digitalWrite(led[i], LOW);
      //Serial.print("LED ");
      //Serial.print(i);
      //Serial.print(" off    ");
    }
      
  }
  if (newAvail) {
    //DO SOMETHING
    recString = "";
    newAvail = false;
    
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

void serialEvent()
{
  while (Serial.available())
  {  
    char inChar = (char)Serial.read();
    recString += inChar;
    Serial.print("inChar: ");
    Serial.println(inChar);
    
    if (inChar == '|')
    {
      Serial.print("Char at 0: ");
      Serial.println(recString.charAt(0));
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
        Serial.println("I have received a sequence");
        Serial.print("It contains this many digits: ");
        int digits = (recString.length()-1)/2;
        Serial.println(digits);
        /*
        int* seq = new int[(recString.length()-1)/2];
        Serial.println(sizeof(*seq));
        for (int i = 2; i < recString.length(); i++)
        {
        }
        */
        int seqArr[digits];
        for (int i = 2; i <= (digits*2); i += 2)
        {
          seqArr[i/2 -1] = recString.charAt(i) - '0';
        }
        /*Verify that you can actually create arrays of variable size
        Serial.print("Created array size: ");
        Serial.println(sizeof(seqArr) / sizeof(int));
        */
        //Test Sequence Array
        for (int i = 0; i < digits; i ++)
        {
          Serial.println(seqArr[i]);  
        }
        
        
        
      }

      
      newAvail = true;
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

