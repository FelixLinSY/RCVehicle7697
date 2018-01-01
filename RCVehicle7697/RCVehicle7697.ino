#include <LRemote.h>

/* Linkit Remote control object */
LRemoteButton     LR_frontward;
LRemoteButton     LR_backward;
LRemoteButton     LR_right;
LRemoteButton     LR_left;
LRemoteSwitch     LR_light;

/* PIN definition for DC motor driver of robot shield */
#define PIN_REAR_WHEEL_A        10
#define PIN_REAR_WHEEL_B        12
#define PIN_FRONT_WHEEL_A       13
#define PIN_FRONT_WHEEL_B       17
/* PIN definition for LED control */
#define PIN_LIGHT_LEFT          7
#define PIN_LIGHT_RIGHT         8


#define BLE_RC_NAME           "Vehicle"

void setup() {
  /* Initialize serial and wait for port to open */
  Serial.begin(115200);
  
  /* initialize pin mode */
  pinMode(PIN_REAR_WHEEL_A, OUTPUT);
  pinMode(PIN_REAR_WHEEL_B, OUTPUT);
  pinMode(PIN_FRONT_WHEEL_A, OUTPUT);
  pinMode(PIN_FRONT_WHEEL_B, OUTPUT);
  pinMode(PIN_LIGHT_LEFT, OUTPUT);
  pinMode(PIN_LIGHT_RIGHT, OUTPUT);

  /* stop all motors */
  rear_stop();
  front_stop();

  /* trun off light */
  light_off();
  
  /* setup luinkit remote control */
  LRemote.setName(BLE_RC_NAME);
  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(4, 3);
  
  /* add push button: forward */
  LR_frontward.setText("forward");
  LR_frontward.setPos(0, 0);
  LR_frontward.setSize(1, 1);
  LR_frontward.setColor(RC_GREEN);
  LRemote.addControl(LR_frontward);
  
  /* add push button: backward */
  LR_backward.setText("backward");
  LR_backward.setPos(0, 2);
  LR_backward.setSize(1, 1);
  LR_backward.setColor(RC_GREEN);
  LRemote.addControl(LR_backward);
  
  /* add push button: right */
  LR_right.setText("right");
  LR_right.setPos(3, 1);
  LR_right.setSize(1, 1);
  LR_right.setColor(RC_GREEN);
  LRemote.addControl(LR_right);
  
  /* add push button: left */
  LR_left.setText("left");
  LR_left.setPos(2, 1);
  LR_left.setSize(1, 1);
  LR_left.setColor(RC_GREEN);
  LRemote.addControl(LR_left);
  
  /* Add light switch */
  LR_light.setText("Light");
  LR_light.setPos(1, 0);
  LR_light.setSize(1, 1);
  LR_light.setColor(RC_BLUE);
  LRemote.addControl(LR_light);
  
  /* start BLE advertisement  */
  LRemote.begin();
  Serial.println("begin() returned");
}

void light_off() {
  digitalWrite(PIN_LIGHT_LEFT, HIGH);
  digitalWrite(PIN_LIGHT_RIGHT, HIGH);
}

void light_on() {
  digitalWrite(PIN_LIGHT_LEFT, LOW);
  digitalWrite(PIN_LIGHT_RIGHT, LOW);
}

void rear_stop() {
  digitalWrite(PIN_REAR_WHEEL_A, LOW);
  digitalWrite(PIN_REAR_WHEEL_B, LOW);
}

void front_stop() {
  digitalWrite(PIN_FRONT_WHEEL_A, LOW);
  digitalWrite(PIN_FRONT_WHEEL_B, LOW);
}

void forward() {
  digitalWrite(PIN_REAR_WHEEL_A, HIGH);
  digitalWrite(PIN_REAR_WHEEL_B, LOW);
}

void backward() {
  digitalWrite(PIN_REAR_WHEEL_A, LOW);
  digitalWrite(PIN_REAR_WHEEL_B, HIGH);
}

void turn_right() {
  digitalWrite(PIN_FRONT_WHEEL_A, LOW);
  digitalWrite(PIN_FRONT_WHEEL_B, HIGH);
}

void turn_left() {
  digitalWrite(PIN_FRONT_WHEEL_A, HIGH);
  digitalWrite(PIN_FRONT_WHEEL_B, LOW);
}

void loop() {
  /**
   * check if we are connect by some 
   * BLE central device, e.g. an mobile app
   */
  if(!LRemote.connected()) {
    Serial.println("waiting for connection");
    delay(500);
  } else {
    delay(10);
  }
  
  /**
   * Process the incoming BLE write request
   * and translate them to control events
   */
  LRemote.process();

  if (LR_frontward.isValueChanged()) {
    if (LR_frontward.getValue() == 1) {
      Serial.println("forward is pressed");
      forward();
    } else {
      Serial.println("forward is released");
      rear_stop();
    }
  }
  
  if (LR_backward.isValueChanged()) {
    if (LR_backward.getValue() == 1) {
      Serial.println("backward is pressed");
      backward();
    } else {
      Serial.println("backward is released");
      rear_stop();
    }
  }
  
  if (LR_right.isValueChanged()) {
    if (LR_right.getValue() == 1) {
      Serial.println("right is pressed");
      turn_right();
    } else {
      Serial.println("right is released");
      front_stop();
    }
  }
  
  if (LR_left.isValueChanged()) {
    if (LR_left.getValue() == 1) {
      Serial.println("left is pressed");
      turn_left();
    } else {
      Serial.println("left is released");
      front_stop();
    }
  }

  if(LR_light.isValueChanged()){
    if (LR_light.getValue() == 1) {
      Serial.println("turn the light on");
    } else {
      Serial.println("turn the light off");
    }
  }
  
}
