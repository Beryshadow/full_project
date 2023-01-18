#define dirPin1 2  // Motor1 direction pin
#define stepPin1 3 // Motor1 sptep pin

#define dirPin2 4  // Motor2 direction pin
#define stepPin2 5 // Motor2 step pin

#define delayBetweenSteps 400 // Delay between steps microsecondes

// distance sensor
#define analogPin A0         // Pin analogique pour le capteur de distance
#define averageReads 10      // Montant de lectures pour la moyenne
#define delayBetweenReads 10 // Délai entre les lectures en mico-secondes

// button
#define boutonForward 13    // Bouton qui descend le scraper
#define boutonBackward 12;  // Bouton qui monte le scraper
int defaultBoutonState LOW; // Etat par défaut du bouton

// Indicator led
#define indicatorLed 10 // Pin pour l'indicateur LED

// global variables
int distance;
int previousDistance;

// fonction pour faire tourner les moteurs
void moveMotors(int steps)
{
  // set la direction des moteurs
  if (steps < 0)
  {
    digitalWrite(dirPin1, LOW);
    digitalWrite(dirPin2, LOW);
  }
  else
  {
    digitalWrite(dirPin1, HIGH);
    digitalWrite(dirPin2, HIGH);
  }
  for (int i = 0; i < abs(steps); i++)
  {
    digitalWrite(stepPin1, HIGH);
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(delayBetweenSteps);
    digitalWrite(stepPin1, LOW);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(delayBetweenSteps);
  }
}

void setup()
{
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