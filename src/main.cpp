#include <Arduino.h>

#define ISR_PIN                                                                \
  0 // TODO: change this to the correct pin (this pin should be the pin that
    // interrupt 2 is attached to on the arduino uno)
char buffer[128];
bool connected = false;

/**
 * @brief This function will clear the buffer
 * @param buffer The buffer to clear
 * @param size The size of the buffer
 */
void clearBuffer(char *buffer, int size) {
  for (int i = 0; i < size; i++) {
    buffer[i] = 0;
  }
}

/**
 * @brief This function will check to see if the client has requested a capture
 * via serial If the client has requested a capture, it will send a signal to
 * the camera to take a picture The signal will consist of a single pin being
 * set to high for a short period of time
 */
void tryCapture() {
  // clear the buffer
  clearBuffer(buffer, 128);
  // read the serial buffer until we get a null terminator or we have read 128 bytes
  int bytesRead = Serial.readBytesUntil('\0', buffer, 128);
  // if we have read more than 0 bytes and the buffer contains the word "capture"
  if (bytesRead > 0 && strcmp(buffer, "capture") == 0) {
    // send a signal to the camera to take a picture
    digitalWrite(10, HIGH);
    delay(100);
    digitalWrite(10, LOW);
  }
}

/**
 * @brief This function is attached to an external interrupt that is triggered
 * when the camera has finished capturing an image This function will send a
 * signal to the client that the image has been captured and downloaded. The
 * client will then be able to move to the next image. The expected signal is the
 * the word "next" sent over serial. This function is executed in an interrupt
 * context so you won't find any references to it in the main loop.
 */
void captureCompleteISR() {
  // TODO: implement this function
}

void setup() {
  // put your setup code here, to run once:
  pinMode(10, OUTPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), captureCompleteISR, RISING);
}

void loop() {
  // check for ping. This will let the client know that they are connected to
  // the correct device. We read until we get a null terminator ('\0') because
  // the client will send a string which is automatically null terminated.)
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