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

// Manually making more readable states because enums don't work well
const int IDLE = 0;
const int MONITORING = 1;
const int EMERGENCY = 2;
const int GUIDING = 3;

// Another manually created enum for instructions
const int STRAIGHT = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int BACK = 3;


int state = IDLE; // Sets the state to the default value

#ifndef LinkedList_hpp
#define LinkedList_hpp


template <class T>
class ListNode {
  public:
    T element;
    ListNode* next;
    ListNode* prev;

    ListNode(T element, ListNode* prev, ListNode* next) : element(element)
    {
      this->next = next;
      this->prev = prev;
    };
};

template <class T>
class LinkedList  {
  private:
    ListNode<T>* head;
    ListNode<T>* tail;
    ListNode<T>* curr;
  public:
    int length;
    LinkedList();
    LinkedList(const LinkedList<T>&);
    ~LinkedList();
    T& getCurrent();
    T& First() const;
    T& Last() const;
    int getLength();
    void Append(T);
    void DeleteLast();
    void DeleteFirst();
    void DeleteCurrent();
    bool next();
    bool moveToStart();
    bool prev();
    void Delete(T&);
    bool Search(T);
    void Clear();
    void PutFirstToLast();
    void Update(T elem);
    LinkedList& operator = (const LinkedList<T>&);
};

template <class T>
LinkedList<T>::LinkedList() {
    length = 0;
    head = nullptr;
    tail = nullptr;
    curr = nullptr;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T> & list) {
    length = 0;
    head = nullptr;
    tail = nullptr;
    curr = nullptr;

    ListNode<T> * temp = list.head;

    while(temp != nullptr)
    {
        Append(temp->element);
        temp = temp->next;
    }
}

template <class T>
LinkedList<T> & LinkedList<T>::operator=(const LinkedList<T> & list)
{
    Clear();

    ListNode<T> * temp = list.head;

    while(temp != nullptr)
    {
        Append(temp->element);
        temp = temp->next;
    }

    return *this;
}

template <class T>
LinkedList<T>::~LinkedList() {
    Clear();
}

template<class T>
T& LinkedList<T>::getCurrent()
{
  return curr->element;
}

template<class T>
T& LinkedList<T>::First() const
{
  return head->element;
}

template<class T>
T& LinkedList<T>::Last() const
{
  return tail->element;
}

template<class T>
int LinkedList<T>::getLength()
{
  return length;
}

template <class T>
void LinkedList<T>::Append(T element)
{
    ListNode<T> * node = new ListNode<T>(element, tail, nullptr);

    if(length == 0)
        curr = tail = head = node;
    else {
        tail->next = node;
        tail = node;
    }

    length++;

}

template <class T>
void LinkedList<T>::DeleteLast()
{
    if(length == 0)
      return;
    curr = tail;
    DeleteCurrent();
}

template <class T>
void LinkedList<T>::DeleteFirst()
{
    if(length == 0)
      return;
    curr = head;
    DeleteCurrent();
}

template <class T>
bool LinkedList<T>::next()
{
    if(length == 0)
        return false;

    if(curr->next == nullptr)
        return false;

    curr = curr->next;
    return true;
}

template <class T>
bool LinkedList<T>::moveToStart()
{
    curr = head;
    return length != 0;
}

template<class T>
bool LinkedList<T>::prev()
{
    if(length == 0)
        return false;

    if(curr->prev != nullptr)
        return false;

    curr = curr->prev;
    return true;
}

template <class T>
void LinkedList<T>::Delete(T & elem)
{
    if(Search(elem))
        DeleteCurrent();
}

template <class T>
void LinkedList<T>::DeleteCurrent()
{
    if(length == 0)
        return;
    length--;
    ListNode<T> * temp = curr;

    if(temp->prev != nullptr)
        temp->prev->next = temp->next;
    if(temp->next != nullptr)
        temp->next->prev = temp->prev;

    if(length == 0)
        head = curr = tail = nullptr;
    else if(curr == head)
        curr = head = head->next;
    else if(curr == tail)
        curr = tail = tail->prev;
    else
        curr = curr->prev;

     delete temp;
}

template <class T>
bool LinkedList<T>::Search(T elem)
{
    if(length == 0)
        return false;
    if(moveToStart())
        do {
            if(curr->element == elem)
                return true;
        } while (next());
    return false;
}

template <class T>
void LinkedList<T>::PutFirstToLast()
{
  if(length < 2)
    return;
  ListNode<T>* temp = head->next;
  head->next->prev = nullptr;
  head->next = nullptr;
  head->prev = tail;
  tail->next = head;
  tail = head;
  head = temp;
}

template <class T>
void LinkedList<T>::Update(T elem)
{
    if(Search(elem))
        curr->element = elem;
}

template <class T>
void LinkedList<T>::Clear()
{
    if(length == 0)
        return;
    ListNode<T> * temp = head;

    while(temp != nullptr)
    {
        head = head->next;
        delete temp;
        temp = head;
    }

    head = curr = tail = nullptr;

    length = 0;

}


#endif
LinkedList<int*> queue;

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
    blueState();
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
        state = MONITORING;
    }
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
    state = EMERGENCY;


  } else {
    // Button was NOT pressed within 5 seconds
    setColor(0, 255, 0);
    delay(4000); // Pause before entering blue mode again
    state = IDLE;
  }
  changeState(); 
}

void redState() {
  // emergency protocol - abnormal heart rate detected
  // led turns red, and beeper starts beeping
  // it starts to move, avoiding obstacles, until main button is pressed again. 
  // when main button is pressed again --> call yellowState()

    setColor(255, 0, 0); // Red Color
    int start_time = millis();
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, HIGH);
  // add buzzer sound here
  // add movement

  // Read button (LOW = pressed when using INPUT_PULLUP)
    while (digitalRead(buttonControlPin) == HIGH){
    }
    // Button IS pressed → stop motors. Help has been found
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    int instruction[] = {STRAIGHT, millis() - start_time};
    queue.Append(instruction);

    // STOP BUZZER SOUND

    state = GUIDING;
    changeState();
}

void yellowState() {
  // the robot retraces it's steps back to initial location. 
  // When it arrives with help (another button push on main button), back to bluestate --> call blueState()


  setColor(255, 255, 0); // Yellow Color
  // add movement
  while (queue.length > 0) {
    Serial.println(queue.Last()[0]);
    Serial.println(queue.Last()[1]);
    reverse(queue.Last());
    queue.DeleteLast();
  }


  // assume everything is done now
  state = IDLE;
  changeState();
}

void setColor(int redValue, int greenValue, int blueValue) {
    analogWrite(redPin, redValue);
    analogWrite(greenPin, greenValue);
    analogWrite(bluePin, blueValue);
}

void changeState() {
    switch (state) {
        case IDLE:
            blueState();
        case MONITORING:
            blankState();
        case EMERGENCY:
            redState();
        case GUIDING:
            yellowState();
    }
}

void reverse(int instruction[2]) {
    Serial.println(instruction[0]);
    Serial.println(instruction[1]);
    switch (instruction[0]){
        case STRAIGHT:
            // Code that make go forward
            Serial.println("Just give me a 'pass' control thingy, please");
        case LEFT:
            // Code that goes spinny towards the right
            Serial.println("Just give me a 'pass' control thingy, please");
        case RIGHT:
            // Code that goes spinny towards the left
            Serial.println("Just give me a 'pass' control thingy, please");
        case BACK:
            // Code thet make go backwards
            Serial.println("Just give me a 'pass' control thingy, please");
    }
    delay(instruction[1]);
}