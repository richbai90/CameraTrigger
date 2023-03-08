#include <Arduino.h>

#define CAPTURE_PIN 0 // TODO: change this to the correct pin
#define ISR_PIN 0     // TODO: change this to the correct pin (this pin should be the pin that interrupt 2 is attached to on the arduino uno)
#define CAPTURE_TIME 1000 // TODO: change this to the correct time (this is the time that the capture pin will be set to high for)
#define CAPTURE_COMPLETE_CODE 0x00 // TODO: change this to the correct code (this is the code that will be sent to the client when the capture is complete)

char buffer[128];
bool connected = false;

/**
 * @brief This function will check to see if the client has requested a capture via serial
 * If the client has requested a capture, it will send a signal to the camera to
 * take a picture The signal will consist of a single pin being set to high for
 * a short period of time
 */
void tryCapture() {
  // TODO: implement this function
}

/**
 * @brief This function is attached to an external interrupt that is triggered
 * when the camera has finished capturing an image This function will send a
 * signal to the client that the image has been captured and downloaded. The
 * client will then be able to move to the next image
 */
void captureCompleteISR() {
  // TODO: implement this function
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), captureCompleteISR, RISING);
}

void loop() {
  // check for ping. This will let the client know that they are connected to
  // the correct device. We read until we get a null terminator ('\0') because the
  // client will send a string which is automatically null terminated.)
  if (!connected && Serial.readBytesUntil('\0', buffer, 128) > 0 &&
      strcmp(buffer, "ping") == 0) {
    Serial.println("PONG"); // send pong
    // don't check for a connection again once we are connected
    // If we disconnect, we will have to reset the arduino
    connected = true;
  } else if (!connected) {
    // if we aren't connected, don't waste time trying to capture
    return;
  }

  // check for a capture request
  tryCapture();
}