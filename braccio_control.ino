#include <SoftwareSerial.h>
#include <Braccio.h>
#include <Servo.h>


Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;
// Bluetooth pins for HC-05
SoftwareSerial Bluetooth(8, 7); 

int INCREMENTOR = 5;
int M0 = 20;
int M1 = 90;
int M2 = 45;
int M3 = 180;
int M4 = 180;
int M5 = 90;
int M6 = 10;

void setup() {  
  Bluetooth.begin(9600);   
  Serial.begin(9600);
  delay(5000);
  Bluetooth.begin(38400);   
  Serial.begin(38400);
  
  //Initialization functions and set up the initial position for Braccio
  //All the servo motors will be positioned in the "safety" position:
  //Base (M1):90 degrees
  //Shoulder (M2): 45 degrees
  //Elbow (M3): 180 degrees
  //Wrist vertical (M4): 180 degrees
  //Wrist rotation (M5): 90 degrees
  //gripper (M6): 10 degrees
  Braccio.begin();
  Braccio.ServoMovement(M0, M1, M2, M3, M4, M5, M6); 
}

void moveArm(){
  Serial.println("moveArm " + String(M0) + " " + String(M1) + " " + String(M2) + " " + String(M3) + " " + String(M4) + " " + String(M5) + " " + String(M6));
  Braccio.ServoMovement(M0, M1, M2, M3, M4, M5, M6); 
 }

void setMotorValue(String motor, int value) {
    /*
   
   M1=base degrees. Allowed values from 0 to 180 degrees
   M2=shoulder degrees. Allowed values from 15 to 165 degrees
   M3=elbow degrees. Allowed values from 0 to 180 degrees
   M4=wrist vertical degrees. Allowed values from 0 to 180 degrees
   M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
   M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the toungue is open, 73: the gripper is closed.
  */
  Serial.println("setMotor ("+motor+") to ("+String(value)+")");
  if (motor == "M0") {
    M0 = value;
  } else if (motor == "M1" && 0 < value <= 180 ) {
    M1 = value;
  } else if (motor == "M2" && 15 < value <= 180 ) {
    M2 = value;
  } else if (motor == "M3" && 0 < value <= 180 ) {
    M3 = value;
  } else if (motor == "M4" && 0 < value <= 180 ) {
    M4 = value;
  } else if (motor == "M5" && 0 < value <= 180 ) {
    M5 = value;
  } else if (motor == "M6" && 10 < value <= 73 ) {
    M6 = value;
  }
}

int getMotorValue(String motor) {
  Serial.println("getMotorValue ("+motor+")");
  if (motor == "M0") {
    return M0;
  } else if (motor == "M1") {
    return M1;
  } else if (motor == "M2") {
    return M2;
  } else if (motor == "M3") {
    return M3;
  } else if (motor == "M4") {
    return M4;
  } else if (motor == "M5") {
    return M5;
  } else if (motor == "M6") {
    return M6;
  }
}



boolean controlArm(String command){
  Serial.println("controlArm ("+command+")");
  /*
   * command Types
   * 0 - Set
   * 1 - Increment
   * 2 - Decrement
   * 
   * Based on the latest MIT App inventor code, the command will always be 0, but I am leaving this here for now
   */
  int commandType = command.substring(0, 1).toInt(); // Get first character
  String motor = command.substring(1, 3); // Get second and third character
  int value = command.substring(3).toInt(); // Get third character to the end
  Serial.println("ControlArm CommandType:("+String(commandType)+"), motor:("+motor+"), value:("+String(value)+")");
  if (commandType == 0){
    setMotorValue(motor, value);
    moveArm();
    return true;
  } else if (commandType == 1 || commandType == 2){
    // This will never run 
    int currentMotorValue = getMotorValue(motor);
    if (commandType == 1) {
      currentMotorValue = currentMotorValue + INCREMENTOR;
    } else {
      currentMotorValue = currentMotorValue - INCREMENTOR;
    }
    setMotorValue(motor, currentMotorValue);
    moveArm(); 
    return true;
  }
  return false;
}


  
void loop() {

  while(Bluetooth.available()){
    //String Data = Bluetooth.readString();
    Serial.println("||"+String(M1) + "|" + String(M2) + "|" + String(M3) + "|" + String(M4) + "|" + String(M5) + "|" + String(M6) + "||");
    Bluetooth.println("||"+String(M1) + "|" + String(M2) + "|" + String(M3) + "|" + String(M4) + "|" + String(M5) + "|" + String(M6) + "||");      

    String Data = Bluetooth.readStringUntil('\n');
    
    Serial.println(Data);
    boolean armMoved = controlArm(Data);
    if (armMoved){
      Serial.println("||"+String(M1) + "|" + String(M2) + "|" + String(M3) + "|" + String(M4) + "|" + String(M5) + "|" + String(M6) + "||");
      Bluetooth.println("||"+String(M1) + "|" + String(M2) + "|" + String(M3) + "|" + String(M4) + "|" + String(M5) + "|" + String(M6) + "||");      

     }
  }
}
