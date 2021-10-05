// WesternU Baja Club
// Tachometer Test 2
// Code by JamesN

// User-adjustable variables
#define sensPin 2 // CHANGE THIS to change sensor pin
#define invert true // CHANGE THIS if your sensor outputs HIGH on trigger
#define bufferSize 5 // CHANGE THIS for better ACCURACY -- reduces time between reading

// Leave alone pls
unsigned long timeBuffer[bufferSize];
unsigned long lastMicros = 0;
bool lastState = false;
int valueCounter = 0;


void setup() {
  // Setup pins and serial
  pinMode(sensPin, INPUT);
  Serial.begin(9600);
}


// Checks if the sensor is triggered (true) or false
bool checkIfMagnet() {
  if (invert) {
    return !digitalRead(sensPin);
  } else {
    return false;
  }
}


// Adds a time value to the time buffer
void addTimeToBuffer(long triggerTime) {
  timeBuffer[valueCounter] = triggerTime;
  Serial.print("Stored in buffer slot ");
  Serial.println(valueCounter);
  valueCounter++;
  if (valueCounter == bufferSize - 1) {
    // Calculate speed and reset buffer

    // Calculate the average speed:
    long avgTime = 0;
    for (int i = 0; i <= valueCounter; i++) {
      avgTime += timeBuffer[i];
    }
    avgTime /= bufferSize;

    // Output
    Serial.print("##Buffer full! The current speed is ");
    Serial.print(avgTime);
    Serial.println(" microseconds per trigger.##");

    // Reset buffer
    for (int i = 0; i <= bufferSize; i++) {
      timeBuffer[i] = 0;
    }
    Serial.print("~~~Buffer Cleared!~~~");
    for (int i = 0; i < bufferSize; i++) {
      Serial.print(" ");
      Serial.print(timeBuffer[i]);
      Serial.println(" |");
    }

    valueCounter = 0;
  }
}


void loop() {
  
  // Debounce
  if (lastState) {
    if (checkIfMagnet()) {
      return;
    }
  }

  lastState = checkIfMagnet();
  if (checkIfMagnet()) {
    // Magnet fired!

    // Count the time between magnet being last fired and now
    unsigned long currentMicros = micros();

    if (currentMicros < lastMicros) {
      // Overflow case, skip
      lastMicros = micros();
      return; 
    }
    
    unsigned long timeBetween = currentMicros - lastMicros;
    Serial.print("----    Triggered! Time = ");
    Serial.println(timeBetween);
    
    // Store the current time
    lastMicros = currentMicros;

    // Store in the buffer
    addTimeToBuffer(timeBetween);
  }
}
