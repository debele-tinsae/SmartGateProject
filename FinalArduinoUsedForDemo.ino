/* SYSC 3010 Project - Arduino Code for
 * Motor and Ultrasonic Sensor 
 *
 * Author: Abdelrahman Darwish  
 * Date: November 3, 2019 
 * 
 */
// I had another github repository just for the progression of this code
// This is the clean version of it
// link to my repository is: https://github.com/abdelrahmandarwish1/SMARTGate-Project

 
// Include the Motor library
#include <Stepper.h>

// number of steps per rotation of our Stepper Motor
const int StepsPerRev=200; 

// True if an incoming car has been detected by UltraSonic Sensor One and false otherwise
bool IncomingCarDetected;

// Declaring the data that is coming from the Raspberry Pi that is in Serial Communication with Arduino
// 1 means open gate ( Access Granted) and 0 means gate remains closed (Access Denied)
int incoming_state;

// Motor Pins Initiliziation for the ULN2003 Motor Driver
const int MotorPin1 = 8; // IN1 on the Motor Driver
const int MotorPin2 = 9; // IN2 on the Motor Driver
const int MotorPin3 = 10; // IN3 on the Motor Driver
const int MotorPin4 = 11; // IN4 on the Motor Driver

// Initializing a Stepper Object specifying the pins connected to the Motor 
// Notice: that the Motor Pin numbers are not in order, this is to ensure that we get the correct step sequence
Stepper myStepper = Stepper(StepsPerRev,MotorPin1,MotorPin3,MotorPin2,MotorPin4);
 
// UltraSonic Sensor GPIO Initilization
// Trigger pin of Sensor
const int trigPin = 3;
// Echo Pin of Sensor
const int echoPin = 4 ;

// Declaring Variables used for calulating the distance from UltraSonic Sensor One 
// TimePeriod is the time taken for the pulse to be emitted from the sensor and bounce back from the sensed object, used for calculating the distance
long TimePeriod;


/**
 * checkSerial Method will check if there is any serial data coming from the Database Pi
 * Then read what is being sent to the Arduino by the Pi through the serial port , Changes ASCII to integers
 * The incoming data (integers) determines whether or not the gate opens.
 * 
 */
 
 void checkSerial() {
  
 if(Serial.available() ){
      
    // Incoming Data Detected
    // Reading the incoming data from the Pi and converting the byte to an integer 
    // 0 is the NULL Character in the ASCII Table, hence we subtract it to find the numeric value of incoming data
    incoming_state= Serial.read() -'0';   
    
    delay(250);
   }

   if( incoming_state == 1 ){
      
      // Database Pi confirmed that the car can enter the Garage
      // Access Granted, Gate opens
      // Rotate motor in a clockwise direction for a quarter of a revolution
      myStepper.step(500);
      
      // Wait 4.5 seconds for the car to fully enter the garage 
      delay(4500);
      
      // now close the gate 
      // Rotate the motor in an anticlockwise direction for a quarter of a revolution
      myStepper.step(-500);     
   }
  
 }

/**
 * calculateDistanceOne Method calculate the distance of objects from sensor 
 * and if the distance of a car is less than 25 cms away from the sensor then it would send a message 
 * through the serial port to the Image Regognition Pi to take an image because a car has been detected
 */
 
int calculateDistanceOne(){
  
  // UltraSonic Sensor One 
  // Clear trigPin
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse 
  digitalWrite(trigPin,LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Returns the duration in microseconds, The time that the sound wave travelledTimePeriod
  // Note: this is the time for the sound wave to travel to object + time of sound wave to return from object to sensor
  // Hence, the time period must be halved
  
  TimePeriod = pulseIn(echoPin,HIGH);

  // Calculate the distance in cms from object and is a whole number 
  // 0.0343 is the speed of sound in cm per microseconds
  
  int distance = ( (TimePeriod/2) * (0.0343) );
  
  delay(400);
  


if(distance < 25 ){
  
  // Incoming Car Stopped 25cms away from the sensor
  // Incoming Car Detected 
  
  IncomingCarDetected = true;

  delay(350);
  
  // Send a message to the Pi , to take a picture of the liscense plate number if its an incoming car
  // Note: Pi will not take a picture of cars trying to exit the garage only cars attempting to enter!
  Serial.println(" 1 ");
  
  // Wait 4 second 
  delay (4000);

  // Now that the Camera has taken and Plate number has been compared, check for serial data from the Pi to open or close the gate 
  // i.e Allow or deny access to the garage
  checkSerial();
  


} 
else{
      // Cars are at a distance greater than 25 cms away from the sensor
      // Do nothing, no incoming car detected 
      delay(250);
      
      // Notify the Pi that it should not do anything, i.e DO NOT take any pictures
      Serial.println(" 0 ");
      
      delay(400);
}
}
 
void setup() {
  
  // Motor speed to 100 RPM
  myStepper.setSpeed(100);
 
  // UltraSonic Sensor one setup below
  // Set the trigPin as an output
  pinMode(trigPin,OUTPUT);
  
  //Set the echoPin as an input
  pinMode(echoPin,INPUT);

  // Set the data rate to 9600, start Serial Communication with Image Recognition Pi
  Serial.begin(9600);

}

void loop() { 
// Code that will run forever

// Keep Calling the function that calculates the distance of objects from sensor
 calculateDistanceOne();

   
} 





 



 
  




   
 
 


  
 

 
