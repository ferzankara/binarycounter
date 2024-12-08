const int buttonPin1 = A0;  // the number of the first pushbutton pin
const int buttonPin2 = A1;  // the number of the second pushbutton pin
const int ledPins[] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, A2, A3, A4, A5, 13};  // the numbers of the LED pins (8 red + 8 yellow)
const int ledValues[] = {1, 2, 4, 8, 16, 32, 64, 128, 1, 2, 4, 8, 16, 32, 64, 128};   // values represented by each LED (red and yellow)
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

int buttonState1 = 0;  // variable for reading the first pushbutton status
int buttonState2 = 0;  // variable for reading the second pushbutton status
int lastButtonState1 = LOW;  // previous state of the button
int lastButtonState2 = LOW;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

int count = 0;  // variable to store the count value
bool countChanged = false; // flag to track if the count has changed

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);  // initialize LED pins as outputs
  }
  pinMode(buttonPin1, INPUT);  // initialize the pushbutton pins as inputs
  pinMode(buttonPin2, INPUT);
}

void loop() {
  int reading1 = digitalRead(buttonPin1);
  int reading2 = digitalRead(buttonPin2);

  if (reading1 != lastButtonState1 || reading2 != lastButtonState2) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading1 != buttonState1) {
      buttonState1 = reading1;

      if (buttonState1 == HIGH) {
        count = (count + 1) % 256;  // Increment count and wrap around if it exceeds 255
        countChanged = true;
      }
    }

    if (reading2 != buttonState2) {
      buttonState2 = reading2;

      if (buttonState2 == HIGH) {
        count = (count - 1 + 256) % 256;  // Decrement count and wrap around if it goes below 0
        countChanged = true;
      }
    }
  }

  lastButtonState1 = reading1;
  lastButtonState2 = reading2;

  if (countChanged) {
    // Turn off all LEDs first
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW);
    }

    // Update LEDs based on the binary count
    for (int i = 0; i < numLeds; i++) {
      if (count & ledValues[i]) {
        if (i < 8) {
          digitalWrite(ledPins[i], HIGH);  // Red LED
        } else {
          digitalWrite(ledPins[i], LOW);   // Yellow LED (for numbers 8 and above)
        }
      } else {
        if (i < 8) {
          digitalWrite(ledPins[i], LOW);   // Red LED
        } else {
          digitalWrite(ledPins[i], HIGH);  // Yellow LED (for numbers 8 and above)
        }
      }
    }

    // Print the current count value to the serial monitor
    Serial.print("Count: ");
    Serial.println(count);
    
    countChanged = false; // Reset the flag
  }
}
