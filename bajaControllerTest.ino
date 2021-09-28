// Pin 6 = sensor pin

#define sensPin 2

float timer = 0;
bool lastState;

void setup() {
  pinMode(sensPin, INPUT);
  Serial.begin(9600);
}

bool checkIfMagnet() {
  bool isOn = digitalRead(sensPin);
  return !isOn;
}

void loop() {

  // Debounce
  if (lastState) {
    if (checkIfMagnet()) {
      return;
    }
  }

  // Okay we're off do thing
  lastState = checkIfMagnet();
  if (lastState) {

    // Fancy output
    String output = "Rotations per second: ";
    output += pow((timer / 1000), -1);

    // Do thing
    Serial.println(output);
    timer = 0;
  } else {
    // Time the time
    timer++;
  }
  
  delay(1);
}
