// Main code
#include <Controller.h>

Controller controller("LeBot", "123456789");

int MOTOR_ENA = 9;
int MOTOR_IN1 = 2;
int MOTOR_IN2 = 4;
int MOTOR_ENB = 10;
int MOTOR_IN3 = 7;
int MOTOR_IN4 = 8;

int CATAPULT_IN1 = 12;
int CATAPULT_ENA = 6;


void setup() {
    
    Serial.begin(115200);

    //Enable status LED:
    controller.enableStatusLED(LED_BUILTIN);
    
    //Controller library settings:
    controller.setMotorMinPWM(90);
    controller.configureL298N(
    MOTOR_ENA, MOTOR_IN1, MOTOR_IN2,    // ENA, IN1, IN2
    MOTOR_ENB, MOTOR_IN3, MOTOR_IN4   // ENB, IN3, IN4
    );
    controller.beginAP(true);
    controller.setFailsafeTimeoutMs(800);
    
    //Custom buttons for controller:
    controller.registerButton("Three Pointer", threePointer);
    controller.registerButton("Two Pointer", twoPointer);
    controller.registerButton("One Pointer", onePointer);
    controller.registerButton("Star 1", star_1);
    controller.registerButton("Star 2", star_2);
    controller.registerButton("Star 3", star_3);
    controller.registerButton("Star 4", star_4);
    controller.registerButton("Star 5", star_5);
    controller.registerButton("Star 6", star_6);
    controller.registerButton("Star 7", star_7);

}


void loop() {

    controller.update();
}

// Custom shot buttons:

void threePointer() {
  fireCatapult(255, 100);
}

void twoPointer() {

}

void onePointer() {

}

void star_1() {

}

void star_2() {

}

void star_3() {

}

void star_4() {

}

void star_5() {

}

void star_6() {

}

void star_7() {

}

// Function to fire the catapult
// speed: 0 (off) to 255 (max speed)
// durationMs: how long the gear spins to release the catapult (in milliseconds)
void fireCatapult(int speed, unsigned long durationMs) {
  
  // 1. Turn motor ON and set speed
  analogWrite(CATAPULT_ENA, speed);
  digitalWrite(CATAPULT_IN1, HIGH);
  
  // 2. Keep spinning to wind/release the catapult
  delay(durationMs);
  
  // 3. STOP the motor immediately after
  digitalWrite(MOTOR_IN1, LOW);
  analogWrite(MOTOR_ENA, 0);
}
