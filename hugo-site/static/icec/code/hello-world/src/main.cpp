#include <Arduino.h>
#include <WiFi.h>

#include <IceC.h>
#include <IceC/platforms/esp8266/TCPEndpoint.h>
#include <IceC/platforms/esp8266/debug.hpp>

#include "st.h"

// Some global variables
Ice_Communicator ic;
Ice_ObjectAdapter adapter;
st_IBool servant;

// The servant implementation
void st_IBoolI_set(st_IBoolPtr self, Ice_Bool v, Ice_String sourceAddr) {
  digitalWrite(LED_BUILTIN, v ? HIGH : LOW);
}

void setup() {
  // Common initialization
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  String ssid = "hello-node";
  Serial.printf("\n--------\nWiFi: setting up AP as '%s'\n", ssid.c_str());
  WiFi.mode(WIFI_AP);
  WiFi.enableSTA(false);
  WiFi.softAP(ssid.c_str());

  // Initialize the communicator
  Ice_Communicator_init(&ic);
  TCPEndpoint_init(&ic);

  // Create the object adapter
  Ice_Communicator_createObjectAdapterWithEndpoints(
    &ic, "adapter", "tcp -p 1234", &adapter
  );
  Ice_ObjectAdapter_activate(&adapter);

  // Register servant
  st_IBool_init(&servant);
  Ice_ObjectAdapter_add(&adapter, (Ice_ObjectPtr)&servant, "led");

  Serial.println("Ready, waiting events...");
}

void loop() {
  // Wait for events
  Ice_Communicator_loopIteration(&ic);
}