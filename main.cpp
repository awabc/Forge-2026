// Main code
#include <Controller.h>

Controller controller("LeBot_James", "1234");


void setup() {
    
    serial.begin(115200);

    //Enable status LED:
    controller.enableStatusLED(LED_BUILTIN);
    
    //Controller library settings:
    controller.setMotorMinPWM(90);
    controller.configureL298N(
    1, 1, 1,    // ENA, IN1, IN2
    1, 1, 1    // ENB, IN3, IN4
    );
    controller.beginAP(true);
    controller.setFailsafeTimeoutMs(1500);
    
    //Custom buttons for controller:
    controller.registerButton("Three Pointer", threePointer);
}


void loop() {




    controller.update();
}

void threePointer() {
  Serial.println("Button pressed!");
}