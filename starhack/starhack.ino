void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  // While not in panic mode, monitor heart rate and make green LED light up.
  // When abnormal heart rate: go into panic mode (light up red LED) -> Go in straight line until object is detected, beeping. (implement pathfinding to turn and go back if time)
  // When object found: Guiding mode -> lead back to original point.
  // Extra: Add indicator when heart rate is near abnormal threshold, enable voluntary activation -> automatic at certain threshold

}

int get_heart_rate() {
  //Return heart rate by taking input from heart rate monitor
}