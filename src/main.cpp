#include "controller.h"
#include "settings.h"

Controller controller;
Settings settings;

void setup() {
  Serial.begin(115200);
  Serial.println("[Main] Capsule Counter Setup Beginning");
  controller.begin();
  settings.begin();
  Serial.println("[Main] Capsule Counter Setup Complete");
}

void loop() {}