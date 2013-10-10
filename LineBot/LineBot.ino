const int leftWheelPin = 2;
const int rightWheelPin = 4;

/* init motors to rest-point */
int leftWheelPos = 90;
int rightWheelPos = 90;

/* ldr pins */
const int sensorPins[3] = {0, 2, 4};

/* last read values */
int sensorValues[3];

/* computed from sensor values */
int blackValues[3] = {0, 180, 0};

enum {LEFT_LDR, FRONT_LDR, RIGHT_LDR};

void setup() {
  pinMode(leftWheelPin, OUTPUT);
  pinMode(rightWheelPin, OUTPUT);
  
  int i;
  for (i=0; i<3; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }
}

void loop() {
  int i, val;
  
  /* for each ldr */
  for (i=0; i<3; i++) {
    val = analogRead(sensorPins[i]);
    blackValues[i] = constrain(blackValues[i] + (val - sensorValues[i]), 0, 180);
    sensorValues[i] = val;
  }
  
  int blackLeft  = blackValues[LEFT_LDR];
  int blackFront = blackValues[FRONT_LDR];
  int blackRight = blackValues[RIGHT_LDR];
  
  setWheels(blackFront + blackRight, blackFront + blackLeft);
  
  /* make sure it doesn't stop */
  if (blackFront + blackRight < 100 && blackFront + blackLeft < 100) {
    blackValues[FRONT_LDR]++;
  }
}

void setWheels(int leftVal, int rightVal) {
  leftWheelPos = motoMove(leftWheelPin, leftWheelPos, 180 - constrain(leftVal, 0, 180));
  rightWheelPos = motoMove(rightWheelPin, rightWheelPos, constrain(rightVal, 0, 180));
}

/* write to motor signal pin */
int motoMove(int pin, int fromPos, int toPos) {
  int pos = fromPos;
  int out;

  if (fromPos < toPos) pos++;
  if (fromPos > toPos) pos--;

  out = map(pos, 0, 180, 500, 2500);  

  digitalWrite(pin, HIGH);
  delayMicroseconds(out);
  digitalWrite(pin, LOW);
  delayMicroseconds(out);

  return pos;
}

