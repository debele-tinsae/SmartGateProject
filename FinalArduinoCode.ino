/* SYSC 3010 Project - Arduino Code for
 * Motor and Ultrasonic Sensor 
 *
 * Author: Abdelrahman Darwish  
 * Date: November 25, 2019 
 * 
 */
 
// Include the Motor library
#include <Stepper.h>

// number of steps per rotation, Full step mode 2048 steps for 360 degrees 
const int StepsPerRev=2048; 

// True if an incoming car has been detected by UltraSonic Sensor One and false otherwise
bool IncomingCarDetected;

// Declaring the data that is coming from the Raspberry Pi that is in Serial Communication with Arduino
// 1 means open gate ( Access Granted) and 0 means gate remains closed (Access Denied)
int incoming_state;

// Motor Pins Initiliziation
const int MotorPin1 = 3; // IN1 on the Motor Driver
const int MotorPin2 = 4; // IN2 on the Motor Driver
const int MotorPin3 = 5; // IN3 on the Motor Driver
const int MotorPin4 = 6; // IN4 on the Motor Driver

// Initializing a Stepper Object specifying the pins connected to the Motor 
Stepper myStepper = Stepper(StepsPerRev,MotorPin1,MotorPin2,MotorPin3,MotorPin4);
 
// UltraSonic Sensor GPIO Initilization
// Trigger pin of Sensor
const int trigPin = 12 ;
// Echo Pin of Sensor
const int echoPin = 11 ;

// Declaring Variables used for calulating the distance from UltraSonic Sensor One 
// TimePeriod is the time taken for the pulse to be emitted from the sensor and bounce back from the sensed object, used for calculating the distance
long TimePeriod;

/**
 * checkSerial Method will check if there is any serial data coming from the Database Pi
 * The incoming data determines whether or not the gate opens.
 * 
 */
 
int checkSerial(){
  
    // Gets the number of bytes available from the Serial Port
    // If number of bytes is  greater than zero means there is incoming data from Pi
    
    if(Serial.available() > 0){
      
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
      myStepper.step(StepsPerRev/4);
      
      // Wait 8 seconds for the car to fully enter the garage 
      delay(800);
      
      // now close the gate 
      // Rotate the motor in an anticlockwise direction for a quarter of a revolution/cycle
      myStepper.step(-StepsPerRev/4);     
   }

    else if( incoming_state == 0){
      
      delay(500);
      
      // Plate Number not in the database
      // Hence, Access Denied, Gate remains closed
      // Do Nothing i.e Gate remains closed
      
    }
}

/**
 * calculateDistanceOne Method calculate the distance of objects from sensor 
 */
 
int calculateDistanceOne(){
  
  // UltraSonic Sensor One 
  // Clear trigPin
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse 
  digitalWrite(trigPin,LOW);
  delayMicroseconds(4);
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
  
  delay(900); // wait for car to completely stop
  // Call the method that checks for incoming data from Pi 
  checkSerial();

} 
else{
  
  
      // Cars are at a distance greater than 25 cms away from the sensor
      // Do nothing, no incoming car detected 
      delay(250);
      
      // Notify the Pi that it should not do anything, i.e DO NOT take any pictures
      Serial.println("0");
      
      delay(400);
}
}
 
void setup() {
  
  // Motor speed to 5 RPM
  myStepper.setSpeed(5);
 
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
 
// Check for Incoming Serial coming from the Image Recognition Pi to either or open the gate or keep the gate closed
 checkSerial();
   
} 





 



 
  




   
 
 


  
 

 
