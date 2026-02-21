// Main code
#include <Controller.h>

Controller controller("LeBot", "123456789");


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
  Serial.println("Button pressed!");
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