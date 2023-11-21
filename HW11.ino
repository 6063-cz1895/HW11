// Constants for pins
const int POT_PIN = A0;// Analog pin for potentiometer
const int RED_LED_PIN = 6;// Digital pin for the red LED
const int GREEN_LED_PIN = 7;// Digital pin for the green LED
const int BLUE_LED_PIN = 8;// Digital pin for the blue LED
const int BUTTON1_PIN = 2;// Digital pin for button 1
const int BUTTON2_PIN = 3;// Digital pin for button 2
const int POT_MIDDLE = 512;// Middle value for 10-bit ADC (1024/2=512)

// State variables
bool isBlueActive = false;// Tracks if blue LED should be on based on potentiometer turning
int buttonPressCount = 0;// Tracks the number of button pressed
bool firstButtonWasButton1 = false; // Tracks if the first button pressed was Button 1

// Clicky Class
class Clicky {
private:
    int STATE_0 = 0;// State representing button not pressed
    int STATE_1 = 1;// State representing button pressed
    int cState = STATE_0;// Current state of the button
    int pin;// Pin number to which the button is referred to

public:
    Clicky() {}

    void setup(int _pin) {
        pin = _pin;
        pinMode(pin, INPUT);// Initialize the button pin as input
    }

    // check if the button was clicked
    bool wasClicked() {
        int v = digitalRead(pin);// Read the current state of the button
        bool clicked = false;
        if (cState == STATE_0 && v) {
            cState = STATE_1;
            clicked = true;// If the state changed to pressed, mark as clicked
        } else if (cState == STATE_1 && !v) {
            cState = STATE_0;// Reset state to not pressed when button is released
        }
        return clicked;
    }
};

// Button objects
Clicky button1;
Clicky button2;

void setup() {
    // Initialize LEDs as outputs
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);

    // Initialize button objects for managing button clicking
    button1.setup(BUTTON1_PIN);
    button2.setup(BUTTON2_PIN);
}

void loop() {
    // Read potentiometer value to determine if blue LED should be on
    int potValue = analogRead(POT_PIN);
    isBlueActive = potValue > POT_MIDDLE;
    digitalWrite(BLUE_LED_PIN, isBlueActive); // Set blue LED based on potentiometer position

    // Check for button pressing
    bool button1Clicked = button1.wasClicked();
    bool button2Clicked = button2.wasClicked();

    // if statement for button pressing
    if (button1Clicked || button2Clicked) {
        buttonPressCount++; // Increase the button press count

        if (buttonPressCount == 1) {
            // Record which button was pressed first
            firstButtonWasButton1 = button1Clicked;
        } else if (buttonPressCount == 2) {
            if (isBlueActive && firstButtonWasButton1 && button2Clicked) {
                // Correct sequence (Button 1 then Button 2) with potentiometer turned
                digitalWrite(GREEN_LED_PIN, HIGH); // Turn on green LED
                delay(2000);  // Keep it on for 2 seconds
                digitalWrite(GREEN_LED_PIN, LOW); // Turn off green LED
            } else {
                // Any other sequence or potentiometer not turned appropriately
                digitalWrite(RED_LED_PIN, HIGH);  // Turn on red LED
                delay(2000);  // Keep it on for 2 seconds
                digitalWrite(RED_LED_PIN, LOW); // Turn off red LED
            }

            // Reset the status
            buttonPressCount = 0;
            firstButtonWasButton1 = false;
        }
    }

    // Small delay as usual
    delay(10);
}