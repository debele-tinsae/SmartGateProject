/* SYSC 3010 Project - Arduino Code for Database Pi
 * 
 *
 * Author: Abdelrahman Darwish  
 * Date: November 20, 2019 
 * 
 */
 
 // Include the Motor library
 #include <Stepper.h>

const int StepsPerRev=2048; // number of steps per rotation, Full step mode 2048 steps for 360 degrees 

// Initializing Motor Pins 
const int MotorPin1 = 3; // IN1 on the Motor Driver
const int MotorPin2 = 4; // IN2 on the Motor Driver
const int MotorPin3 = 5; // IN3 on the Motor Driver
const int MotorPin4 = 6; // IN4 on the Motor Driver

// Declaring the state that is coming from the Database Raspberry Pi
// 1 means open gate ( Access Granted) and 0 means gate remains closed (Access Denied)
int incoming_state;


//  Initializing a Stepper Object specifying the pins connected to the Motor
Stepper myStepper = Stepper(StepsPerRev,MotorPin1,MotorPin3,MotorPin2,MotorPin4);

 /**
 * checkSerial Method will check if there is any serial data coming from the Database Pi
 * The incoming data determines whether or not the gate opens 
 * 
 */
int checkSerial(){
  
    if(Serial.available()){
    
    // Checking  for incoming data from Database Pi
    // Reading the incoming state from the Pi
    
    incoming_state= Serial.read() -'0';   
    delay(250);
    
    
  }

   if( incoming_state == 1){
      
      // Database Pi confirmed that the car can enter the Garage
      // Access Granted, Gate opens
      
      Serial.println("Access Granted, please proceed");
      
      // Rotate motor in a clockwise direction for a quarter of a revolution
      
      myStepper.step(StepsPerRev/4);
      
      delay(800);

      // now close the gate 

      myStepper.step(-StepsPerRev/4);
      
   }

    else if( incoming_state == 0){
      
      delay(500);
      
      // Plate Number not in the database
      // Hence, Access Denied, Gate remains closed
      
      Serial.println(" Access Denied ");
      
    }
}

void setup(){
  
  // Set the data rate to 9600, start Serial Communication with Database Pi
  Serial.begin(9600);

  //Setting the Motor speed to 5 RPM
  myStepper.setSpeed(5);

}
void loop(){

  checkSerial();
  
}
 
