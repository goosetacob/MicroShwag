unsigned int right_motorA  = 11;
unsigned int right_motorB = 10;
unsigned int left_motorB = 6;
unsigned int left_motorA = 5;

unsigned int right_enable = 3;
unsigned int left_enable = 9;

unsigned int right_dec = A4;
unsigned int left_dec = A5;

unsigned int FRONT  = A0;
unsigned int LEFT = A1;
unsigned int RIGHT = A2;

signed int setpoint_right= 0;
signed int setpoint_left = 0;
signed int setpoint_front = 0; 

signed int error_right = 0;
signed int error_left = 0;

signed int to_right = 0;
signed int to_left = 0;

signed int motorPWN_right = 0;
signed int motorPWN_left = 0;

const double Kp = .20 ;

void setup(){
  Serial.begin(9600);
  
  
  delay(3000);
  setpoint_right = analogRead( RIGHT);
  setpoint_left = analogRead( LEFT);
  setpoint_front = analogRead(FRONT);
  
  pinMode(right_motorA, OUTPUT);
  pinMode(right_motorB, OUTPUT);
  pinMode(left_motorA, OUTPUT);
  pinMode(left_motorB, OUTPUT);

  pinMode(right_dec, INPUT);
  pinMode(left_dec, INPUT);

  pinMode(FRONT , INPUT );
  pinMode(LEFT , INPUT );
  pinMode(RIGHT , INPUT );
  
  digitalWrite( right_enable , HIGH);
  digitalWrite(left_enable, HIGH);
}

void loop(){  
  
  unsigned int left_val = analogRead(LEFT);
  unsigned int right_val = analogRead(RIGHT);
  unsigned int front_val = analogRead(FRONT);
  unsigned int right_dec_val = analogRead(right_dec);
  unsigned int left_dec_val = analogRead(left_dec);



  
  if (left_val > setpoint_left+50 && left_val > setpoint_left-50 && right_val > setpoint_right && front_val < setpoint_front) {
    stop_motor();
    delay(500);
    turn_right();
  }
  else if (right_val > setpoint_right+50 && right_val > setpoint_right-50 && left_val > setpoint_left && front_val < setpoint_front) {
    stop_motor();
    delay(500);
    turn_left();
  }
  
  else if (right_val > setpoint_right+50 && right_val > setpoint_right-50 && left_val > setpoint_left+50 && left_val > setpoint_left-50 && front_val > setpoint_front+50 && front_val > setpoint_front-50) {
    stop_motor();
  }
  else {
      error_right = setpoint_right - right_val;
  motorPWN_right = 190 + (Kp*error_right);
  
  error_left = setpoint_left - left_val;
  motorPWN_left = 190 + (Kp*error_left);

  forward(motorPWN_right, motorPWN_left);
  }
  
  
  Serial.print(" Left: ");
  Serial.print(left_val);
  Serial.print(" Right: ");
  Serial.print(right_val);
  Serial.print(" Front: ");
  Serial.print(front_val);
  Serial.print(" Right Decoder: ");
  Serial.print(right_dec_val);
  Serial.print(" Left Decoder: ");
  Serial.println(left_dec_val);

  delay(1);
}
  

void stop_motor() {
    digitalWrite( right_motorA , LOW);
    digitalWrite( right_motorB , LOW);
    digitalWrite( left_motorA , LOW);
    digitalWrite( left_motorB , LOW);
    delay(200);  
}

void turn_left() {
    digitalWrite( right_motorA , LOW);
    digitalWrite( right_motorB , HIGH);
    digitalWrite( left_motorA , HIGH);
    digitalWrite( left_motorB , LOW);
    delay(1000);
    stop_motor();
}
void turn_right() {
    digitalWrite( right_motorA , HIGH);
    digitalWrite( right_motorB , LOW);
    digitalWrite( left_motorA , LOW);
    digitalWrite( left_motorB , HIGH);
    delay(1000);
    stop_motor();
}

void forward(int motorPWN_right, int motorPWN_left) {
  //if error is negative we are turning AWAY
  if( error_right > 0 && error_left < 0) {
    analogWrite(left_motorA, 0);
    analogWrite(left_motorB , motorPWN_left);
    
    analogWrite(right_motorA , 0);
    analogWrite(right_motorB, motorPWN_right);
  }
  
  //if error is positive we are turning TO
  if (error_right < 0 && error_left > 0) {
    analogWrite(left_motorA, 0);
    analogWrite(left_motorB, motorPWN_left);
    
    analogWrite(right_motorA, 0);
    analogWrite(right_motorB, motorPWN_right);
  }
}



