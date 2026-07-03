#define BLYNK_TEMPLATE_ID "TMPL6kb0IH4I6"
#define BLYNK_TEMPLATE_NAME "Food Dispenser"
#define BLYNK_PRINT Serial

#include <Wire.h>
#include <RTClib.h>
#include "HX711.h"
#include <ESP32Servo.h>  // Use ESP32Servo library for ESP32 compatibility
#include <BlynkSimpleEsp32.h> 

RTC_DS3231 rtc; // Initialize RTC (Real Time Clock)
HX711 scale;    // Initialize Load Sensor (HX711)
Servo myServo;  // Initialize Servo Motor

// Pin assignments for ESP32
const int DOUT = 26;   // Data pin for HX711 -> Connect to ESP32 GPIO 26
const int CLK = 27;    // Clock pin for HX711 -> Connect to ESP32 GPIO 27
const int SERVO_PIN = 18; // Servo motor signal pin -> Connect to ESP32 GPIO 18

// WiFi Credentials for the hotspot
const char* ssid = "DAN";    // Replace with your Wi-Fi SSID
const char* pass = "12345678";   // Replace with your Wi-Fi Password
const char* auth = "jXH0r6pemIw1YdbQhZg7u_s9KfL0Z2is";

float targetWeight = 50.0; // Target weight in grams
float currentWeight = 0;
bool dispensing = false; // Flag to indicate if dispensing is in progress
bool dispenseState = false; // Flag to track Blynk switch state

// Feeding times (hour, minute)
const int feedingTimes[3][2] = {
  {8, 0},   // 8:00 AM
  {14, 42},  // 2:00 PM (14:00)
  {18, 46}   // 8:00 PM (20:00)
};
bool hasDispensed[3] = {false, false, false};  // Track if food was dispensed for each time slot

void setup() {
  Serial.begin(115200);
  Serial.println("System is starting...");


  
  // RTC Setup (DS3231 or DS1307)
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC module. Check connections.");
    while (1);
  }


  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time.");
    rtc.adjust(DateTime(2024,10,9,16,45,0));
    
  } else {
    
    Serial.println("RTC initialized successfully.");
  }

  // Load Sensor Setup (HX711 module)
  Serial.println("Initializing Load Cell (HX711)...");
  scale.begin(DOUT, CLK);  // Connect DOUT to GPIO 26 and CLK to GPIO 27
  scale.set_scale(2280.f); // Calibration factor (adjust as needed for accuracy)
  scale.tare(); // Reset scale to zero
  Serial.println("Load cell initialized and tared.");

  // Servo Setup
  Serial.println("Attaching servo...");
  myServo.attach(SERVO_PIN); // Connect the servo signal pin to GPIO 18
  myServo.write(0); // Initially close the servo (0 degrees)
  Serial.println("Servo motor initialized.");

  // Blynk Setup
  Blynk.begin(auth, ssid, pass); // Connect to Blynk

  // Print initial time for reference
  DateTime now = rtc.now();
  Serial.print("Current time: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.println(now.minute());
}

void loop() {
  Blynk.run(); // Run Blynk to keep connection alive

  // Get the current time
  DateTime now = rtc.now();

  // Print current time every minute
  Serial.print("Checking time: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.println(now.minute());

  // Check for automatic dispensing times
  for (int i = 0; i < 3; i++) {
    if (now.hour() == feedingTimes[i][0] && now.minute() == feedingTimes[i][1] && !hasDispensed[i]) {
      Serial.print("Automatic feeding at ");
      Serial.print(feedingTimes[i][0]);
      Serial.print(":");
      Serial.println(feedingTimes[i][1]);
      dispenseFood();
      hasDispensed[i] = true; // Mark as dispensed for this time slot
    }

    // Reset hasDispensed flag if the time has passed (to allow future dispensing)
    if (now.hour() != feedingTimes[i][0]) {
      hasDispensed[i] = false;
    }
  }

  // Check if Blynk-controlled dispensing is active
  if (dispenseState) {
    dispenseFood();
  }

  delay(100); // Small delay to avoid rapid looping
}

void dispenseFood() {
  if (dispensing) {
    Serial.println("Already dispensing. Ignoring additional requests.");
    return; // Exit if already dispensing
  }

  dispensing = true; // Set the flag to indicate dispensing has started
  myServo.write(90); // Open the dispenser (90 degrees)
  Serial.println("Opening servo to dispense food...");

  // Monitor weight until target is reached
  currentWeight = 0; // Reset current weight before dispensing
  while (currentWeight < targetWeight) {
    currentWeight = scale.get_units(5); // Read the weight (average of 5 samples)
    Serial.print("Current Weight: ");
    Serial.println(currentWeight);
    delay(1000); // Add a small delay to avoid rapid looping
  }

  // Close the servo once the target weight is reached
  myServo.write(0); // Close the dispenser (0 degrees)
  Serial.println("Target weight reached. Closing servo.");
  dispensing = false; // Reset flag after dispensing is complete
}

// Function to control the dispenser from Blynk
BLYNK_WRITE(V1) { // Assuming you are using Virtual Pin 1 to control the dispenser
  dispenseState = param.asInt(); // Update dispense state based on Blynk button (1 for ON, 0 for OFF)
  if (dispenseState) {
    Serial.println("Dispense state is ON. Starting to dispense...");
    dispenseFood(); // Call dispense food function when command is received
  } else {
    Serial.println("Dispense state is OFF. Stopping dispensing if in progress...");
    if (dispensing) {
      myServo.write(0); // Close the servo if it's currently dispensing
      dispensing = false; // Reset the dispensing flag
      Serial.println("Dispensing stopped.");
    }
  }
}
