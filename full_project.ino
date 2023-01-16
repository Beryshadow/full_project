#include <AccelStepper.h>

// Steppers
#define dirPin1 2  // Motor1 direction pin
#define stepPin1 3 // Motor1 sptep pin

#define dirPin2 4  // Motor2 direction pin
#define stepPin2 5 // Motor2 step pin

#define delayBetweenSteps 10 // Delay between steps

AccelStepper stepper1(AccelStepper::DRIVER, stepPin1, dirPin1);
AccelStepper stepper2(AccelStepper::DRIVER, stepPin2, dirPin2);

// distance sensor
#define analogPin 34         // Analog pin for the distance sensor
#define averageReads 10      // Number of reads to average
#define delayBetweenReads 25 // Delay between reads

// Reset button
#define resetButton 6 // Reset button used to initialize the machine

int distance;
int previousDistance;

// function to move the two motors at once in the same direction
void moveMotors(int steps)
{
  stepper1.for (int i = 0; i < abs(steps); i++)
  {
    stepper1.move((steps > 0) ? 1 : -1);
    stepper2.move((steps > 0) ? 1 : -1);
    delay(delayBetweenSteps);
  }
}

void setup()
{
  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(50);
  stepper1.setSpeed(1000);

  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(50);
  stepper2.setSpeed(1000);

  // pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  // taking the average of the distance sensor readings
  for (int i = 0; i < averageReads; i++)
  {
    delay(delayBetweenReads);
    previousDistance += 4095 - analogRead(analogPin);
  }
  previousDistance /= averageReads;
}

// int time;

void loop()
{
  // taking the average of the distance sensor readings
  for (int i = 0; i < averageReads; i++)
  {
    delay(delayBetweenReads);
    distance += 4095 - analogRead(analogPin);
  }
  distance /= averageReads;

  Serial.print(previousDistance);
  Serial.print(" <-before---after-> ");
  Serial.println(distance);

  // if the distance is more than 1000, move the motors
  // also to stop the motors from turning on the falling edge we set a timer
  if (abs(distance - previousDistance) > 1000)
  {
    moveMotors(100); // could make this adapt based on distance of the sensor
    delay(1000);
  }
  previousDistance = distance;
}

// Distance sensor:
// VCC -> 5V
// Trig -> 8
// Echo -> 9
// GND -> GND

// Stepper motor 1:
// A4988 Driver pin 1 -> Arduino pin 2
// A4988 Driver pin 2 -> Arduino pin 3

// Stepper motor 2:
// A4988 Driver pin 1 -> Arduino pin 4
// A4988 Driver pin 2 -> Arduino pin 5