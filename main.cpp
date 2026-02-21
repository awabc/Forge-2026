// Main code
#include <Controller.h>

Controller controller("LeBot", "123456789");

// Inverted up and down: In1-2 In2-4 In3-7 In4-8
// Normal: In1-8 In2-7 In3-4 In4-2

int MOTOR_ENA = 9;
int MOTOR_IN1 = 8;
int MOTOR_IN2 = 7;
int MOTOR_ENB = 10;
int MOTOR_IN3 = 4;
int MOTOR_IN4 = 2;

int CATAPULT_IN1 = 12;
int CATAPULT_IN2 = 3;
int CATAPULT_ENA = 6;

unsigned long catapultStopMs = 0;
bool catapultFiring = false;

void fire();
void fireCatapult(int speed, unsigned long durationMs);

void setup() {
    
    Serial.begin(115200);

    //Enable status LED:
    controller.enableStatusLED(LED_BUILTIN);

    //Catapult pins:
    pinMode(CATAPULT_ENA, OUTPUT);
    pinMode(CATAPULT_IN1, OUTPUT);
    pinMode(CATAPULT_IN2, OUTPUT);
    digitalWrite(CATAPULT_IN1, LOW);
    digitalWrite(CATAPULT_IN2, LOW);
    analogWrite(CATAPULT_ENA, 0);
    
    //Controller library settings:
    controller.setMotorMinPWM(90);
    controller.configureL298N(
    MOTOR_ENA, MOTOR_IN1, MOTOR_IN2,    // ENA, IN1, IN2
    MOTOR_ENB, MOTOR_IN3, MOTOR_IN4   // ENB, IN3, IN4
    );
    controller.beginAP(true);
    controller.setFailsafeTimeoutMs(800);
    
    //Custom buttons for controller:
    controller.registerButton("Fire!", fire);


}

void loop() {

    controller.update();

    if (catapultFiring && millis() >= catapultStopMs) {
        // Stop the motor immediately
        digitalWrite(CATAPULT_IN1, LOW);
        digitalWrite(CATAPULT_IN2, LOW);
        analogWrite(CATAPULT_ENA, 0);
        
        catapultFiring = false; // Reset the state
    }
}

// Function to fire the catapult
// speed: 0 (off) to 255 (max speed)
// durationMs: how long the gear spins to release the catapult (in milliseconds)

// 150/650 works well, test lower pwm higher time
void fire() {
    fireCatapult(150,650);
}

void fireCatapult(int speed, unsigned long durationMs) {
  
  if (catapultFiring) return;

    // 1. Turn motor ON and set speed
  analogWrite(CATAPULT_ENA, speed);
  digitalWrite(CATAPULT_IN1, HIGH);
  digitalWrite(CATAPULT_IN2, LOW);
  
  catapultStopMs = millis() + durationMs;
  catapultFiring = true;
    
}
