// Pin definitions
const int joyvRxPin = A0; // X-axis pin
const int joyvRyPin = A1; // Y-axis pin
const int joyvSwPin = 8; // Joystick switch pin

const int motor1 = 12; // Left motor backward
const int motor2 = 11; // Left motor forward
const int motor3 = 10; // Right motor forward
const int motor4 = 9;  // Right motor backward

// Center range values for joystick
const int centerRangeMin = 600;
const int centerRangeMax = 700;

// Power value toggle (1 or 0)
int powerValue = 0;
bool lastSwitchState = HIGH; // For detecting toggle

void setup() {
    // Initialize motor pins as outputs
    pinMode(motor1, OUTPUT);
    pinMode(motor2, OUTPUT);
    pinMode(motor3, OUTPUT);
    pinMode(motor4, OUTPUT);
    
    // Set initial motor states to LOW (off)
    stopMotors();

    // Joystick switch pin as input with pull-up resistor
    pinMode(joyvSwPin, INPUT_PULLUP);
    
    // Initialize serial communication for debugging
    Serial.begin(9600);
}

void loop() {
    // Read joystick values
    int xValue = analogRead(joyvRxPin);
    int yValue = analogRead(joyvRyPin);
    int switchState = digitalRead(joyvSwPin); // Read switch state

        // Print the joystick values
    Serial.print("X: ");
    Serial.print(xValue);
    Serial.print("\tY: ");
    Serial.print(yValue);
    Serial.print("\tSwitch: ");
    Serial.print(switchState == LOW ? "Pressed" : "Released");
    
    // Read switch value and handle toggle functionality
    bool currentSwitchState = digitalRead(joyvSwPin);
    if (currentSwitchState == LOW && lastSwitchState == HIGH) {
        // Toggle powerValue when switch is pressed
        powerValue = !powerValue;
        delay(300); // Debounce delay
    }
    lastSwitchState = currentSwitchState;

    // Control the motors if powerValue is 1
    if (powerValue == 1) {
        controlMotors(xValue, yValue);
    } else {
        stopMotors();
    }

    Serial.println("");
    delay(100); // Short delay to avoid spamming motor changes
}

// Function to control motors based on joystick values
void controlMotors(int xValue, int yValue) {
    // Check if joystick is within center range for both X and Y
    if (xValue >= centerRangeMin && xValue <= centerRangeMax &&
        yValue >= centerRangeMin && yValue <= centerRangeMax) {
        // Stop motors if joystick is centered
        stopMotors();
    } else {
        // Reset all motor pins to LOW before setting new direction
        stopMotors();

        // Control movement based on x-axis (left-right)
        if (xValue < centerRangeMin) { // Move left
            digitalWrite(motor2, HIGH); // Left motor forward
            digitalWrite(motor3, HIGH); // Right motor forward
            digitalWrite(motor1, LOW); // Right motor forward
            digitalWrite(motor4, LOW); // Right motor forward
            Serial.print("\tForward");
        } 
        else if (xValue > centerRangeMax) { // Move right
            digitalWrite(motor1, HIGH); // Left motor backward
            digitalWrite(motor4, HIGH); // Right motor backward
            digitalWrite(motor2, LOW); // Right motor backward
            digitalWrite(motor3, LOW); // Right motor backward
            Serial.print("\tBackward");
        }

        // Control movement based on y-axis (forward-backward)
        if (yValue < centerRangeMin) { // Move forward
            digitalWrite(motor2, HIGH); // Left motor forward
            digitalWrite(motor4, HIGH); // Right motor backward
            digitalWrite(motor1, LOW); // Right motor backward
            digitalWrite(motor3, LOW); // Right motor backward
            Serial.print("\tRight");
        } 
        else if (yValue > centerRangeMax) { // Move backward
            digitalWrite(motor1, HIGH); // Left motor backward
            digitalWrite(motor3, HIGH); // Right motor forward
            digitalWrite(motor2, LOW); // Right motor forward
            digitalWrite(motor4, LOW); // Right motor forward
            Serial.print("\tLeft");
        }
    }
}

// Function to stop all motors
void stopMotors() {
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, LOW);
    digitalWrite(motor3, LOW);
    digitalWrite(motor4, LOW);
}
