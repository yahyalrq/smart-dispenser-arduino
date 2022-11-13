// Include the library code
// 

#include <LiquidCrystal_I2C.h> // Downloaded library to manage the lcd with I2C
#include <Wire.h> // This library allows Arduino to communicate with I2C devices
LiquidCrystal_I2C lcd(0x27, 16, 2); //Create the object lcd with lcd_Addr 0x27 (0x3F)


// Sensor pins

#define sensorPower 7
#define sensorPin A1
#define trigPin 9
#define echoPin 10
#define pump_switch 3

// Value for storing variables
int waterval = 0;
int ml = 0;
long ultra_duration;
int ultra_distance;

void setup() {
  Serial.begin(9600);
  
  pinMode(sensorPower, OUTPUT); // Set D7 as an OUTPUT
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(pump_switch,OUTPUT); // Sets the pumpswitch as an Output
  
  // Set to LOW so no power flows through the sensor
  digitalWrite(sensorPower, LOW);
  digitalWrite(pump_switch, LOW); //safety speed reset of the motor

  startlcd(); // Print constant text
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(A0); //read the input on analog pin 0
   
  //get the reading from the function below and print it
  int level = readWaterSensor();
  int days = sensorValue/16.7;
  int presence = readUltraSensor();
  
  Serial.print("Water level:");
  Serial.println(level); //print out the value you read
  
  lcd.setCursor(6, 1); 
  lcd.print(days); // print  days on lcd 
  lcd.setCursor(13, 3); 
  lcd.print(level); // print water level variable on lcd

  if (presence <= 5) {
    lcd.setCursor(6, 2); 
    lcd.print("PUMP ON "); // print  water level
    digitalWrite(pump_switch, HIGH);  // Turn the switch ON 
  }

  else {
    lcd.setCursor(6, 2); 
    lcd.print("PUMP OFF"); // print  water level
    digitalWrite(pump_switch, LOW);   // Turn the switch OFF
  }
  
  delay(200); //delay in between reads for stability

}

//This is a function used to get the reading
int readWaterSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  waterval = analogRead(sensorPin); // Read the analog value form sensor
  waterval = waterval/2.2;
  digitalWrite(sensorPower, LOW); // Turn the sensor OFF
  return waterval;  // send current reading
}

int readUltraSensor() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  ultra_duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  ultra_distance= ultra_duration*0.034/2;

  return ultra_distance;
}

void startlcd() {
  lcd.init(); // Initiate LCD
  lcd.backlight(); // Turn on backlight
  lcd.begin(20, 4); // set up the LCD's number of columns and rows:  
  lcd.setCursor(4, 0); // set the cursor to column 4, line 2 
  lcd.print("INSERT TIME"); // Print a message to the LCD.
  lcd.setCursor(8, 1); 
  lcd.print(" DAYS");
  lcd.setCursor(0, 3); 
  lcd.print("Water Level:");
  lcd.setCursor(17, 3); 
  lcd.print("ml"); // print  water level
}
