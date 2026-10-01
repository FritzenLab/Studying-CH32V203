/*
This sketch reads a push button and decides what to do with an LED:
- Either blink the LED or turn it OFF, for every button push.
So it alternates between OFF and blinking.
*/
#define LED PB2
#define BUTTON PC14

unsigned long ledTimer = 0;
uint16_t ledTime = 500;
uint8_t ledControl = 0;
bool buttonStatus = true;
bool previousState = false;
bool debouncing= true;
unsigned long buttonDebounceTimer = 0;
uint16_t debounceTime = 300;
bool toggleLED = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  
  buttonStatus= digitalRead(BUTTON);

  // If button is pressed and was not previously pressed (in the last 300ms at least)
  if(!buttonStatus && !previousState && !debouncing){
    buttonDebounceTimer= millis();
    debouncing= true;
    previousState= true;
    toggleLED = !toggleLED; // toggle the variable that decides what to do with the LED
  }
  // If debounce time has passed, reset the debouncing control
  if(millis() - buttonDebounceTimer > debounceTime){
    debouncing= false;
    previousState= false;
  }

  if(millis() - ledTimer > ledTime){
    ledTimer += ledTime;
    if(toggleLED){ // if toggleLED is true, blink LED
      if(ledControl == 0){
      ledControl= 1;
      }else{
        ledControl= 0;
      }
    }else{ // if toggleLED is not true, turn LED off
      ledControl= 0;
    }
    
    digitalWrite(LED, ledControl);
  }
}
