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
 
 Serial.println("Starting Program");
 
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 4; i ++) {
    if (digitalRead(btn[i]) == 1) {
      digitalWrite(led[i], HIGH);
      Serial.print("LED ");
      Serial.print(i);
      Serial.print(" on    ");
    }
    else {
      digitalWrite(led[i], LOW);
      Serial.print("LED ");
      Serial.print(i);
      Serial.print(" off    ");
    }
      
  }
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

}
