#define dirPin1 2  // Motor1 direction pin
#define stepPin1 3 // Motor1 sptep pin

#define dirPin2 4  // Motor2 direction pin
#define stepPin2 5 // Motor2 step pin

#define direction 1

#define delayBetweenSteps 400 // Delay between steps microsecondes

// distance sensor
#define analogPin A0         // Pin analogique pour le capteur de distance
#define averageReads 10      // Montant de lectures pour la moyenne
#define delayBetweenReads 10 // Délai entre les lectures en mico-secondes

// button
#define boutonForward 13  // Bouton qui descend le scraper
#define boutonBackward 12 // Bouton qui monte le scraper
int defaultBoutonState;   // Etat par défaut du bouton
// Indicator led
#define indicatorLed 10 // Pin pour l'indicateur LED
int ledState; // Etat de la led
#define blinkDelay 500 // delais entre les blink
int blink; // counter

// global variables
int distance;
int previousDistance;

// fonction pour faire tourner les moteurs
void moveMotors(int steps)
{
  int rot1;
  int rot2;
  // set la direction des moteurs
  if (direction == 1)
    {
      rot1 = 1;
      rot2 = 0;
    }
  else
  {
    rot1 = 0;
    rot2 = 1;
  }
  if (steps < 0)
  {
    digitalWrite(dirPin1, rot1);
    digitalWrite(dirPin2, rot1);
  }
  else
  {
    digitalWrite(dirPin1, rot2);
    digitalWrite(dirPin2, rot2);
  }
  int current = millis();
  for (int i = 0; i < abs(steps); i++)
  {
    digitalWrite(stepPin1, HIGH);
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(delayBetweenSteps);
    digitalWrite(stepPin1, LOW);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(delayBetweenSteps);
    if (abs(current - millis()) > blink) {
      if (ledState == 1) {
        ledState = 0;
      } else {
        ledState = 1;
      }
      digitalWrite(indicatorLed, ledState);
      int current = millis();
    }
  }
  digitalWrite(indicatorLed, LOW);
}

void setup()
{
  defaultBoutonState = LOW;
  // on enables les pin digitale
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(boutonForward, INPUT);
  pinMode(indicatorLed, OUTPUT);

  // on met toutes les pin digitale à LOW
  digitalWrite(dirPin1, LOW);
  digitalWrite(stepPin1, LOW);
  digitalWrite(dirPin2, LOW);
  digitalWrite(stepPin2, LOW);
  digitalWrite(indicatorLed, LOW);

  // pour pouvoir imprimer sur le moniteur en série
  Serial.begin(9600);

  // on prend une lecture pour le capteur de distance
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
    if (digitalRead(boutonForward) == !defaultBoutonState)
    {
      break;
    }
  }
  distance /= averageReads;

  Serial.print(previousDistance);
  Serial.print(" <-before---after-> ");
  Serial.println(distance);

  // ici l'utilisateur veut utiliser le scraper
  if (/*(abs(distance - previousDistance) > 1000) ||*/ (digitalRead(boutonForward) == !defaultBoutonState))
  {
    digitalWrite(indicatorLed, HIGH);
    moveMotors(9000);
    delay(1000);
    while (digitalRead(boutonForward) == !defaultBoutonState)
    {
      moveMotors(10);
    }
  } // ici l'utilisateur veut faire remonter le scraper
  else if (digitalRead(boutonBackward) == !defaultBoutonState)
  {
    digitalWrite(indicatorLed, HIGH);
    moveMotors(-9000);
    delay(1000);
    while (digitalRead(boutonBackward) == !defaultBoutonState)
    {
      moveMotors(-10);
    }
  }
  else
  {
    digitalWrite(indicatorLed, LOW);
  }
  previousDistance = distance;
}