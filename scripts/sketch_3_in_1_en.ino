#include <Servo.h> // using the library to work with the servo
#include <TroykaLight.h> // library for working with light sensor (Troyka-module)
#include <TroykaAccelerometer.h> // library for working with analog accelerometer

Servo servo; // declare a servo variable of type Servo-module
TroykaLight sensorLight(A5); // create an object for working with the light sensor and pass it the pin number of the output signal

// pins for connecting the accelerometer axes
#define PIN_X A1
#define PIN_Y A2
#define PIN_Z A0
 
// calibration values of one of the accelerometers
// use the "AccelerometerCalibration" sketch to get your calibration values
#define MIN_X   1.328
#define MAX_X   1.978
#define MIN_Y   1.284
#define MAX_Y   1.984
#define MIN_Z   1.357
#define MAX_Z   2.007
 
TroykaAccelerometer accel(PIN_X, PIN_Y, PIN_Z); // create an object for working with an analog accelerometer

// list the names of the operations that we will display
enum {
  LX_IN,    // name for the operation that writes suites
  GX_IN,    // name for the operation that records gravity X
  GY_IN,    // name for the operation that records gravity Y
  GZ_IN,    // name for the operation that records gravity Z
};

// create an array in which we will store the sequence of operations
int myArray[] = {
  LX_IN       
};  

// create an object to store the number of the operation being performed
int number_qd = 0;
int max_lum = 0;
int max_lum_id = 0;
 
// create an object to write data with a string
String  dataString = "";

void setup() // setup procedure

{
  servo.attach(10); // bind the drive to port 10
  Serial.begin(9600); // open the serial port
  Serial.println("Serial begin is OK");
  Serial.print("\n"); // added a separator - an empty string
  // each accelerometer needs to be calibrated individually
  // use the "AcelerometerCalibration" sketch to get the calibration values
  // calibrate the accelerometer using the calibration values of one of the batch
  accel.calibrate(MIN_X, MAX_X, MIN_Y, MAX_Y, MIN_Z, MAX_Z);
  // change the reference voltage source to pin AREF
  accel.setAnalogReference(EXTERNAL);
  // collect the top line with the name of the data
  dataString = "LX_IN ('Lx')\tGX_IN ()\tGY_IN ()\tGZ_IN ()";
}

void loop() //процедура loop

{
  // scan environment
  Serial.println("Loop begin");
  for (int i = 0; i <= 180; i = i + 5) // start a cycle for 36 rotations
  {
    servo.write(i);
    // reading data from the light sensor
    sensorLight.read();
    // we collect in a line first the illumination
    dataString = String(sensorLight.getLightLux()) + "\t";
    // потом ось X
    dataString += String(accel.readGX()) + "\t";
    // потом ось Y
    dataString += String(accel.readGY()) + "\t";
    // потом ось Z
    dataString += String(accel.readGZ()) + "\t";
    // print the entire line Illumination + Vector
    Serial.println(dataString); 
    myArray[i] = sensorLight.getLightLux();
    // looking for maximum illumination
    if (myArray[i] > max_lum) {
      max_lum = myArray[i];
      max_lum_id = i;
      }  
    delay(1000); // delay for X seconds
  }
  // turn the drive to the desired position
  servo.write(max_lum_id);
  delay(60000); // delay for X seconds
}
