// Adjustment Constants
#define tol 200 // Right and left sensor tolerance value
#define frontTol 700 // Front sensor tolerance value
#define KP  1 //Proportional gain value
#define nSize 5 // Number of samples for acquireSensor()
#define leftDelay 500 // Time delay to make a 90 degree left turn
#define rightDelay 600 // Time delay to make a 90 degree right turn


// Digital write for each motor
// Combination of 1 and 2 for each side determine forward, backward, coast, and stop motions
#define leftM1 11
#define leftM2 12
#define rightM1 13
#define rightM2 14

// Analog write to control speed of wheel on each side
#define leftMS 10
#define rightMS 20

// Analog read to detect light on each sensor
#define leftSensor 23
#define rightSensor 22
#define frontSensor 21

// Turn control parameters (i.e. front sensor)
int frontDistance;
int frontLoad;

// Speed control parameters
int leftSpeed;
int rightSpeed;

int leftDistance;
int rightDistance;

int leftBaseline;
int rightBaseline;

// Controller parameters
int leftError = 0;
int rightError = 0;
int prevRightError = 0;
int prevLeftError = 0;
int totalError = 0;
int tolerance;

double kp; // Proportional gain

// Various functions
int acquireSensor(int pin)
{
  int sum = 0;
  for (int i = 0; i < nSize; i++)
  {
    sum += analogRead(pin);
  }
  return sum / nSize;
}

void stopMoving()
{
  digitalWrite(leftM1, HIGH);
  digitalWrite(leftM2, HIGH);
  digitalWrite(rightM1, HIGH);
  digitalWrite(rightM2, HIGH);
}

void turnLeft()
{
  digitalWrite(leftM1, LOW);
  digitalWrite(leftM2, HIGH);
  digitalWrite(rightM1, LOW);
  digitalWrite(rightM2, HIGH);
  delay(leftDelay);
  stopMoving();
}

void turnRight()
{
  digitalWrite(leftM1, HIGH);
  digitalWrite(leftM2, LOW);
  digitalWrite(rightM1, HIGH);
  digitalWrite(rightM2, LOW);
  delay(rightDelay);
  stopMoving();
}

void moveStraight()
{
  frontDistance = acquireSensor(frontSensor);
  
  leftDistance = acquireSensor(leftSensor);
  rightDistance = acquireSensor(rightSensor);
  leftError = leftBaseline - leftDistance;
  rightError = rightBaseline - rightDistance;
  totalError = rightError - leftError;

  int adjustSpeed = abs(kp * totalError);

  if (totalError < -tolerance)
  {
    rightSpeed -= adjustSpeed;
    leftSpeed += adjustSpeed;
  }
  else if (totalError > tolerance)
  {
    rightSpeed += adjustSpeed;
    leftSpeed += adjustSpeed;
  }



  if (leftSpeed < 0)
  {
    leftSpeed = 0;h
  }
  else if (leftSpeed > 255)
  {
    leftSpeed = 255;
  }

  if (rightSpeed < 0)
  {
    rightSpeed = 0;
  }
  else if (rightSpeed > 255)
  {
    rightSpeed = 255;
  }
  
  digitalWrite(leftMS, leftSpeed);
  digitalWrite(rightMS, rightSpeed);
  digitalWrite(leftM1, HIGH);
  digitalWrite(leftM2, LOW);
  digitalWrite(rightM1, LOW);
  digitalWrite(rightM2, HIGH);
  delay(25);
}


void setup()
{
  pinMode(leftM1, OUTPUT);
  pinMode(leftM2, OUTPUT);
  pinMode(rightM1, OUTPUT);
  pinMode(rightM2, OUTPUT);
  pinMode(leftMS, OUTPUT);
  pinMode(rightMS, OUTPUT);
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
  pinMode(frontSensor, INPUT);

  // Keeping wheels initially stopped
  stopMoving();
  leftSpeed = 225;
  rightSpeed = 225;
  digitalWrite(leftMS, leftSpeed);
  digitalWrite(rightMS, rightSpeed);

  delay(3000);
  leftBaseline = acquireSensor(leftSensor);
  rightBaseline = acquireSensor(rightSensor);
  frontDistance = acquireSensor(frontSensor);
  
  kp = KP;
  tolerance = tol;
  frontLoad = frontDistance + frontTol;
  // Errors initialized in the global scope
}

void loop()
{
  while (frontDistance < 900)
  {
    moveStraight();
  }
  
  stopMoving();
  delay(1000);
  
  // Changing speeds so turning is consistent
  int rightTS = 220;
  int leftTS = 220;
  digitalWrite(leftMS, leftTS);
  digitalWrite(rightMS, rightTS);
 
  if (acquireSensor(leftSensor) < acquireSensor(rightSensor))
    turnLeft();
  else
    turnRight();

  stopMoving();
  delay(75);
  digitalWrite(leftMS, leftSpeed);
  digitalWrite(rightMS, rightSpeed);
  moveStraight();
  delay (350);
  stopMoving();
  
  leftBaseline = acquireSensor(leftSensor);
  rightBaseline = acquireSensor(rightSensor);
  frontDistance = acquireSensor(frontSensor);
  frontLoad = frontDistance + frontTol;
  delay(100);
}


