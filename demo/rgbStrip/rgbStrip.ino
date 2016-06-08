#define SERIAL_SPEED     115200

#define PORT_MICROPHONE  A0
#define PORT_LIGHT       A1
#define PORT_TEMPERATURE A2


void setup() {
  Serial.begin(SERIAL_SPEED);
}

void loop() {
  static unsigned int valueMicrophone;
  static unsigned int valueLight;
  static unsigned int valueTemperature;
  
  valueMicrophone  = analogRead(PORT_MICROPHONE );
  valueLight       = analogRead(PORT_LIGHT      );
  valueTemperature = analogRead(PORT_TEMPERATURE);
  
  //output data for the processing GUI
  Serial.print(valueMicrophone);
  Serial.print(',');
  Serial.print(  map(valueLight,         0, 700, 0, 100));
  Serial.print(',');
  Serial.println(map(valueTemperature, 400, 500, 0, 100));
}
