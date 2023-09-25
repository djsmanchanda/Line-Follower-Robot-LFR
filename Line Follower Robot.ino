 #include <SparkFun_TB6612.h>

int PWMA = 3; //for speed control
int AIN1 = 2; //Direction
int AIN2 = 4; //Direction
//Motor B
int PWMB = 8; //for speed control
int BIN1 = 6; //Direction
int BIN2 = 7; //Direction
#define STBY 5

// these constants are used to allow you to make your motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;

// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.

Motor motor1 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);
Motor motor2 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);


int P, D, I, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 200;

float Kp = 0;
float Kd = 0;// Initializing PWM Values
float Ki = 0 ;


int minValues[8], maxValues[8], threshold[8];

void setup()
{
  Serial.begin(9600);
  pinMode(11, INPUT_PULLUP);// button to calibrate the bot
  pinMode(12, INPUT_PULLUP);// button to start the bot post calibration
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

}


void loop()
{
  while (digitalRead(11)) {}//Calibrating the bot
  delay(1000);
  calibrate();
  while (digitalRead(12)) {}//starts bot
  delay(1000);

  while (1)
  {
    if ((analogRead(0) > threshold[0] && analogRead(1) < threshold[1] && analogRead(2) > threshold[2]) || (analogRead(0) > threshold[0] && analogRead(1) < threshold[1] && analogRead(2) < threshold[2] && analogRead(3) > threshold[3]) || (analogRead(0) > threshold[0] && analogRead(1) > threshold[1] && analogRead(2) < threshold[2] && analogRead(3) > threshold[3]))
    {
      obl();// Obtuse Left Case
    }
    if ((analogRead(7 - 0) > threshold[7 - 0] && analogRead(7 - 1) < threshold[7 - 1] && analogRead(7 - 2) > threshold[7 - 2]) || (analogRead(7 - 0) > threshold[7 - 0] && analogRead(7 - 1) < threshold[7 - 1] && analogRead(7 - 2) < threshold[7 - 2] && analogRead(7 - 3) > threshold[7 - 3]) || (analogRead(7 - 0) > threshold[7 - 0] && analogRead(7 - 1) > threshold[7 - 1] && analogRead(7 - 2) < threshold[7 - 2] && analogRead(7 - 3) > threshold[7 - 3]))
    {
      obr();// Obtuse Right Case
    }

    if (analogRead(0) > threshold[0] && analogRead(7) < threshold[7] )
    {
      //left 90 turn
      lsp = -25; rsp = lfspeed;
      motor1.drive(0);
      motor2.drive(lfspeed);
    }

    else if (analogRead(7) > threshold[7] && analogRead(0) < threshold[0])
    { 
      //right 90 turn
      lsp = lfspeed; rsp = -25;
      motor1.drive(lfspeed);
      motor2.drive(0);
    }
    else if ((analogRead(4) > threshold[4]) || (analogRead(3) > threshold[3]))
    {
      //Straight with PID tuning
      //Change KP KD values to tune PID, no Ki required since there is no error from sensor and it only introduces lag, better at lower speeds
      Kp = 0.01* (1000  - analogRead(4));
      Kd = 20*Kp;

      linefollow();
    }
    // if the white line ends it turns around and and gets back on the line
  }
}

void obl()
{
  //function for left obtuse angle turn
  lsp = -25; rsp = lfspeed;
  motor1.drive(-150);
  motor2.drive(150);
  delay(150);
}

void obr()
{
  //function for right obtuse angle turn
  lsp = lfspeed; rsp = -25;
  motor1.drive(150);
  motor2.drive(-150);
  delay(150);
}

void linefollow()
{
  int error = (analogRead(3) - analogRead(5));
  P = error;
  I = I + error;
  D = error - previousError;

  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  previousError = error;

  lsp = lfspeed - PIDvalue;
  rsp = lfspeed + PIDvalue;

  if (lsp > 255) {
    lsp = 255;
  }
  if (lsp < -21) {
    lsp = 0;
  }
  if (rsp > 255) {
    rsp = 255;
  }
  if (rsp < -21) {
    rsp = 0;
  }
  motor1.drive(lsp);
  motor2.drive(rsp);

}

void calibrate()
{
  for ( int i = 0; i < 8; i++)
  {
    minValues[i] = analogRead(i);
    maxValues[i] = analogRead(i);
  }

  for (int i = 0; i < 3500; i++)
  { 
    //increase i value if 1-1.5  circle isn't completed;
    motor1.drive(75);
    motor2.drive(-75);
    //let it take 2 seconds to finish a circle, too fast will negatively affect calibration
    for ( int i = 0; i < 8; i++)
    {
      if (analogRead(i) < minValues[i])
      {
        minValues[i] = analogRead(i);
      }
      if (analogRead(i) > maxValues[i])
      {
        maxValues[i] = analogRead(i);
      }
    }
  }

  for ( int i = 0; i < 8; i++)
  {
    threshold[i] = (minValues[i] + maxValues[i]) / 2;
    Serial.print(threshold[i]);
    Serial.print("   ");
  }
  Serial.println();

  motor1.drive(0);
  motor2.drive(0);
}
