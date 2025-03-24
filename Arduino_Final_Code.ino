//blast-it

//LED Indicator Pins
const int led_red = 12;
const int led_green = 13;

//user Input Pins
const int hall_effect1 = A0;
const int hall_effect2 = A1; 
const int limit_switch = A2; 

//decoder output display (7 Segment Display)
const int decoder_pins[] = {2, 3, 4, 5, 6, 7, 8, 11};

//game variables
int score = 0; //initial score
bool expectingInput = false; //tracking if player is allowed to press inputs
int expectedInput = -1; //expected input type (0 - Hall1, 1 - Hall2, 2 - Limit switch)

//debounce variables
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // 50ms debounce delay

//game timing variables
unsigned long responseTime = 10000;          //atart at 10 seconds (10,000 ms)
const unsigned long minResponseTime = 2000;  //minimum response time: 2 seconds
const unsigned long timeDecreaseAmount = 100;     //reduce by 0.1 seconds (100 ms) per success
unsigned long commandStartTime = 0;

void setup() { //subject to change
 for (int i = 0; i < 8; i++) { //lopp to run through decoder pins
    pinMode(decoder_pins[i], OUTPUT);
  }
  //pinMode(9, OUTPUT);  //TX to MP3 player --> jacob will send code
  //pinMode(10, INPUT); //Rx to MP3 player --> jacob will send code

  pinMode(led_red, OUTPUT); //red LED
  pinMode(led_green, OUTPUT); //green LED

  pinMode(hall_effect1, INPUT); //mag sensor 1 - analog input
  pinMode(hall_effect2, INPUT); //mag sensor 2 - analog input
  pinMode(limit_switch, INPUT); //limit switch, using external pull-down resistor - analog input
}

//pull down resistor (need a debounce) for the analog switch 
//for decoder --> +1 point for each successful, 99 is max points 
void updateDisplay (int score) {
  int tens = score / 10; //only gives tens
  int ones = score % 10; //gives remainder divided by 10
  //send tens digit to the first decoder
  for (int i = 0; i < 4; i++) {
    digitalWrite(decoder_pins[i], (tens >> i) & 1);
  }

  //send ones digit to the second decoder
  for (int i = 4; i < 8; i++) {
    digitalWrite(decoder_pins[i], (ones >> (i - 4)) & 1);
  }
 }


bool checkUserInput() { //checks if players action matches expected action
  if(millis() - lastDebounceTime < debounceDelay){ //prevent false trigger
    return false; //no extra clicks
  }
  lastDebounceTime = millis(); //reset debounce timer

  if (expectedInput == 0 && digitalRead(hall_effect1) == LOW) return true; //pull up 
  if (expectedInput == 1 && digitalRead(hall_effect2) == LOW) return true; //pull up
  if (expectedInput == 2 && digitalRead(limit_switch) == LOW) return true; //pull up 

  return false;
}

void ledIndicator(bool success) { //indicates LED
//pass input state variable as either success or failure
   if (success) {
    digitalWrite(led_green, HIGH);
    delay(500);
    digitalWrite(led_green, LOW);

    if (score < 99) {
      score++;
      updateDisplay(score);
      // Make time shorter after success, but not less than 2 seconds
      if (responseTime > minResponseTime) {
        responseTime = responseTime - timeDecreaseAmount;
      }
    }
  } else {
    digitalWrite(led_red, HIGH);
    delay(500);
    digitalWrite(led_red, LOW);
  }
  expectingInput = false; // Reset for next command
}
  
void loop() {
  if (!expectingInput) {
    expectedInput = random(0, 3);  //pick a random command --> 0 = Hall1, 1 = Hall2, 2 = Limit switch
    //mp3 player code inserted here
    expectingInput = true;
    lastDebounceTime = millis();
    commandStartTime = millis(); // start the response timer
  }
  //check if user performed the correct action
  if (expectingInput) {
    bool success = checkUserInput();
     if (success) {
      ledIndicator(true);
    } else if (millis() - commandStartTime > responseTime) {  //if player does nothing for 3 sec --> fails
      ledIndicator(false); //led indicates red
    }
  }
}


