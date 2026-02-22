// Main code
#include <Controller.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

//Screen pin setup
#define TFT_CS A0
#define TFT_RST A1
#define TFT_DC A2
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

//Controller initialization:
Controller controller("LeBot", "123456789");

// Inverted up and down: In1-2 In2-4 In3-7 In4-8
// Normal: In1-8 In2-7 In3-4 In4-2
int MOTOR_ENA = 9;
int MOTOR_IN1 = 8;
int MOTOR_IN2 = 7;
int MOTOR_ENB = 10;
int MOTOR_IN3 = 4;
int MOTOR_IN4 = 2;

int CATAPULT_IN1 = 5;
int CATAPULT_IN2 = 3;
int CATAPULT_ENA = 6;

unsigned long catapultStopMs = 0;
bool catapultFiring = false;

//Not sure if necessary, but initialize functions:
void fire();
void fireCatapult(int speed, unsigned long durationMs);

void forward();
void backward();
void right();
void left();
void microAdjust(int leftDir, int rightDir);

void drawSmileyFace();
void drawShootingFace();

void setup() {
    
    Serial.begin(115200);

    //Screen initilizattion:
    tft.begin();
    tft.setRotation(1);
    drawSmileyFace();

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
    
    //Custom buttons for catapult:
    controller.registerButton("Fire!", fire);

    //Custom buttons for microadjustments:
    controller.registerButton("Forward", forward);
    controller.registerButton("Backward", backward);
    controller.registerButton("Right", right); 
    controller.registerButton("Left", left);
}

void loop() {

    controller.update();

    if (catapultFiring && millis() >= catapultStopMs) {
        // Stop the motor immediately
        digitalWrite(CATAPULT_IN1, LOW);
        digitalWrite(CATAPULT_IN2, LOW);
        analogWrite(CATAPULT_ENA, 0);
        
        catapultFiring = false; // Reset the state
        drawSmileyFace(); //Back to normal!
    }
}

// Microadjustent Buttons:

void forward() {
    microAdjust(1,1);
}

void backward() {
    microAdjust(-1,-1);
}

void right() {
    microAdjust(1,-1);
}

void left() {
    microAdjust(-1,1);
}

void microAdjust(int leftDir, int rightDir) {
    int burstSpeed = 250; // High speed to overcome friction instantly
    int burstTime = 80;   // 80ms to 120ms 

    // Set Left Motor direction (1 = Forward, -1 = Backward)
    if (leftDir == 1) { 
        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);
    } else { 
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, HIGH);
    }
    analogWrite(MOTOR_ENA, burstSpeed);

    // Set Right Motor direction
    if (rightDir == 1) { 
        digitalWrite(MOTOR_IN3, HIGH);
        digitalWrite(MOTOR_IN4, LOW);
    } else { 
        digitalWrite(MOTOR_IN3, LOW);
        digitalWrite(MOTOR_IN4, HIGH);
    }
    analogWrite(MOTOR_ENB, burstSpeed);

    // Pause briefly to let the robot actually move
    delay(burstTime); 

    // HARD BRAKE: Setting both IN pins HIGH stops the motor faster than just cutting power
    digitalWrite(MOTOR_IN1, HIGH); digitalWrite(MOTOR_IN2, HIGH);
    digitalWrite(MOTOR_IN3, HIGH); digitalWrite(MOTOR_IN4, HIGH);
    analogWrite(MOTOR_ENA, 0);     analogWrite(MOTOR_ENB, 0);
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

  drawShootingFace(); //Anrgy!

    // 1. Turn motor ON and set speed
  analogWrite(CATAPULT_ENA, speed);
  digitalWrite(CATAPULT_IN1, HIGH);
  digitalWrite(CATAPULT_IN2, LOW);
  
  catapultStopMs = millis() + durationMs;
  catapultFiring = true;
}

void drawSmileyFace() {
    tft.fillScreen(ILI9341_BLACK);
    tft.fillCircle(100, 80, 25, ILI9341_YELLOW);
    tft.fillCircle(220, 80, 25, ILI9341_YELLOW);
    tft.fillCircle(160, 150, 60, ILI9341_YELLOW); 
    tft.fillCircle(160, 135, 60, ILI9341_BLACK); 
}

void drawShootingFace() {
    tft.fillScreen(ILI9341_BLACK);
    tft.fillCircle(100, 100, 25, ILI9341_RED);
    tft.fillCircle(220, 100, 25, ILI9341_RED);
    tft.drawLine(60, 50, 120, 90, ILI9341_WHITE);
    tft.drawLine(260, 50, 200, 90, ILI9341_WHITE);
    tft.fillRect(120, 180, 80, 10, ILI9341_WHITE);
}

