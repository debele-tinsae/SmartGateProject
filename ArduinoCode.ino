/* SYSC 3010 Project - Arduino Code for
 * Motor and 2 Ultrasonic Sensors 
 *
 * Author: Abdelrahman Darwish  
 * Date: November 3, 2019 
 * 
 */
 
// Note: this system can not handle an incoming car and a car trying to exit the garage at the same time 
  
 // Include the Motor library
 #include <Stepper.h>

const int StepsPerRev=2048; // number of steps per rotation, Full step mode 2048 steps for 360 degrees 

// True if an incoming car has been detected by UltraSonic Sensor One and false otherwise
bool IncomingCarDetected;

// True if a car trying to exit has been detected by UltraSonic Sensor Two and false otherwise 
bool LeavingCarDetected;

// Declaring the state that is coming from the Raspberry Pi
// 1 means open gate ( Access Granted) and 0 means gate remains closed (Access Denied)
int incoming_state;

// Motor Pins 
const int MotorPin1 = 3; // IN1 on the Motor Driver
const int MotorPin2 = 4; // IN2 on the Motor Driver
const int MotorPin3 = 5; // IN3 on the Motor Driver
const int MotorPin4 = 6; // IN4 on the Motor Driver

// change the delay values later I made them small now for faster execution time but not accurate

 // Double check if the call to the constructor is right or not 
 // Stepper Object called myStepper
 // must be in that order 1 3 2 4 
 // 
 
 Stepper myStepper = Stepper(StepsPerRev,MotorPin1,MotorPin3,MotorPin2,MotorPin4);
 

const int trigPin = 12 ;
const int echoPin = 8 ;

// Ultrasonic Sensor two is used to open the gate for cars attempting to leave the garage (opposite side of the garage)
// pin numbers for trig and echo for UltraSonic Sensor two which is placed x cm after the garage door
// Sensor one and two are in opposite directions


// Ultrasonic Sensor two is used to open the gate for cars attempting to leave the garage (opposite side of the garage)
// pin numbers for trig and echo for UltraSonic Sensor two which is placed x cm after the garage door

//const int trigPin2 = 9 ;
//const int echoPin2 = 10 ;

// Declaring Variables used for calulating the distance from UltraSonic Sensor One 

long TimePeriod;
int distance;

// Declaring Varaibles for UltraSonic Sensor Two Used to Calculate distance and Time

long TimePeriod2;
int distance2;

int checkSerial(){
  
if(Serial.available()){
    
    // Checking  for incoming data from Image Recognition Pi
    // Reading the incoming state form the Pi
    
    incoming_state= Serial.read() -'0';   
    delay(150);
    
    
  }

if( incoming_state == 1){
      
      // Image Recognition Pi has processed the liscense plate number with the database
      // Access Granted, Gate opens
      Serial.println("Access Granted, please proceed");
      // have a function call  to open motor maybe 
      // Open the Gate
      // Rotate motor in a clockwise direction for a quarter of a revolution
      
      myStepper.step(StepsPerRev/4);
      
      //Check if the car has fully entered the garage before closing the gate
      // If the distance from UltraSonic Sensor One is less than and Distance away from
      // UltraSonic Sensor Two is greater than zero that means that the car has entered the garage
      // and must now close the gate
      
      if ( (distance == 0) ){
        
        // Car is inside the garage, wait 5 microseconds then close
        delay(200);
        
        // Close the gate
        //myStepper.step(-StepsPerRev/4);
        // check the step function if properly called or not
        
       }

       delay(500);
      
   }

else if( incoming_state == 0){
      
      delay(500);
      
      // Image Recognition Pi has processed the liscense plate number and is not in the database
      // Access Denied, Gate remains closed
      
      Serial.println(" state 0 : Access Denied");
      
     }
}


int calculateDistanceOne(){
  // calculate distance from UltraSonic Sensor One 

    
  // UltraSonic Sensor One 
  // Clear trigPin
  digitalWrite(trigPin,LOW);
  delayMicroseconds(4);
  
  // Generate a Ultrasound wave for 15 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Returns the duration in microseconds, The time that the sound wave travelledTimePeriod
  // Note: this is the time for the sound wave to travel to object + time of sound wave to return from object to sensor
  // Hence, the time period must be halved
  
  TimePeriod = pulseIn(echoPin,HIGH);

  // Calculate the distance in cms from object and is a whole number 
  // 0.0343 is the speed of sound in cm per microseconds
  
  distance = ( (TimePeriod/2) * (0.0343) );
  
  Serial.println(" UltraSonic Sensor One:   "); 
  delay(170);
  Serial.println(" Distance of the Incoming Car from the gate is :  ");
  delay(170);
  Serial.println(distance);
  delay(170);
  


if(distance < 25 ){
  
  // Incoming Car getting closer 


IncomingCarDetected = true;

LeavingCarDetected = false;

delay(170);
Serial.println(" Incoming Car has been detected ");
delay(170);

// Send a message to the Pi , to take a picture of the liscense plate number if its an incoming car
// Note: Pi will not take a picture of cars trying to exit the garage only cars attempting to enter!
  
Serial.write(1); 

Serial.println(" 1 ");

delay(300);

delay(700); // wait for car to completely stop

Serial.println(" Please Wait, car has come to a complete stop infront of the gate ");

delay(200);

checkSerial();


} 
else{
     // Do nothing, no incoming car detected 
      //IncomingCarDetected = false;

      // Notify the Pi that it should not do anything, i.e DO NOT take any pictures

      delay(170);
      
      Serial.write(0);

      Serial.println("0");
      
      delay(170);

      Serial.println(" No Incoming car has been detected");
      
      delay(170);
    
}
}
 
void setup() {
  
  //Motor speed to 5 rpm
  myStepper.setSpeed(5);
 
  //UltraSonic Sensor one setup below
  //Set the trigPin as an output
  pinMode(trigPin,OUTPUT);
  
  //Set the echoPin as an input
  pinMode(echoPin,INPUT);
  
// 
//UltraSonic Sensor Two setup below
// Set the trigPin2 as an output
// pinMode(trigPin2,OUTPUT);
  
  //Set the echoPin2 as an input
  //pinMode(echoPin2,INPUT);

  // Set the data rate to 9600, start Serial Communication with Image Recognition Pi
  Serial.begin(9600);

}

void loop() { 
 
 calculateDistanceOne();
 
// calculateDistanceTwo();
   
  
  

} 
  




   
 
 


  
 

 
