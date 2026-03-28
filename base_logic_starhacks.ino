#include <SoftwareSerial.h>

// pin for LED : 
// pin for button 1 (main button) : 
// pin for button 2 (substitute for heartrate monitor) : 
// pin for ultrasonic sensor : 
// pin for left motor : 
// pin for right motor : 
// pin for passive buzzer : 

// FOR JACKALOPE: figure out what kind of beeper, make beeper start in redstate, stop in yellow state. 
// also add: if main button pressed in BlueState, go into blank state

/// RGB pin setup
int redPin= 5;
int greenPin = 6;
int  bluePin = 7;

int buttonMonitorPin = 2;
int buttonControlPin = 3;

const int motorPin1 = 9;      // Motor control pin(s)
const int motorPin2 = 10;     // If using H-bridge for direction


void setup() {

  // LinkedList<int> queue;

   //Defining the pins as OUTPUT
    pinMode(redPin,  OUTPUT);              
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    pinMode(buttonMonitorPin, INPUT_PULLUP);
    pinMode(buttonControlPin, INPUT_PULLUP);

    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);

}

void loop() {
  
  blueState()

  // put your main code here, to run repeatedly:
  // While not in panic mode, monitor heart rate and make green LED light up.
  // When abnormal heart rate: go into panic mode (light up red LED) -> Go in straight line until object is detected, beeping. (implement pathfinding to turn and go back if time)
  // When object found: Guiding mode -> lead back to original point.
  // Extra: Add indicator when heart rate is near abnormal threshold, enable voluntary activation -> automatic at certain threshold

}

void blueState() {
  // it's idling, so it just going straight until someone presses the main button
  // when someone presses the button (main button) --> call measureHeartrateState()

  setColor(0, 0, 255); // Blue Color

  // PUT MOVE FORWARD COMMAND HERE

  // Read button (LOW = pressed when using INPUT_PULLUP)
  bool pressed = (digitalRead(buttonControlPin) == LOW);

  if (!pressed) {
    // Button NOT pressed → keep motors running
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);   // Example: forward direction
  } else {
    // Button IS pressed → stop motors
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);

  }

   // Button pressed. Motors stopped.

  blankState()




}


void blankState() {
  // complete!

  // blank (colorless)
  // measure heart rate state
  // this is where it would normally detect heartrate
  // since we don't have a heartrate detector availible, we will indicate abnormal heartrate by a button push, and normal by no button push
  // it will turn green if no push (normal), wait a moment, then go back to blue mode --> call blueState()
  // if it receives a push within the time, it will go into red mode (abnormal heartrate detected) --> call redState()

  setColor(0, 0, 0); // no Color

  unsigned long startTime = millis();
  bool buttonPressed = false;

  // Wait up to 5 seconds
  while (millis() - startTime <   5000) { // its set to 5000 miliseconds (5 seconds) right now, but can be changed if needed!!
    if (digitalRead(buttonMonitorPin) == LOW) {  // LOW = pressed (with INPUT_PULLUP)
      buttonPressed = true;
      break;
    }
  }

  if (buttonPressed) {
    // Button was pressed within 5 seconds (indicating high heartrate, to )
    redState()


  } else {
    // Button was NOT pressed within 5 seconds
    setColor(0, 255, 0);
    delay(4000); // Pause before entering blue mode again
    blueState()
  }

  
}


}

void redState() {
  // emergency protocol - abnormal heart rate detected
  // led turns red, and beeper starts beeping
  // it starts to move, avoiding obstacles, until main button is pressed again. 
  // when main button is pressed again --> call yellowState()

  setColor(255, 0, 0); // Red Color
  // add buzzer sound here
  // add movement

  // Read button (LOW = pressed when using INPUT_PULLUP)
  bool pressed = (digitalRead(buttonControlPin) == LOW);

  if (!pressed) {
    // Button NOT pressed → keep motors running
    // ADD MOVEMENT CODE HERE

  } else {
    // Button IS pressed → stop motors. Help has been found
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);

    // STOP BUZZER SOUND

    yellowState()

  }





}

void yellowState() {
  // the robot retraces it's steps back to initial location. 
  // When it arrives with help (another button push on main button), back to bluestate --> call blueState()

  setColor(255, 255, 0); // Yellow Color
  // add movement


  // assume everything is done now
  blueState()


}