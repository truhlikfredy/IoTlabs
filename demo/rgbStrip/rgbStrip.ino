#define SERIAL_SPEED     115200

#define PORT_MICROPHONE  A0
#define PORT_LIGHT       A1
#define PORT_TEMPERATURE A2

#define MAX_INT          65535


void setup() {
  Serial.begin(SERIAL_SPEED);
}

void loop() {
  static unsigned int valueMicrophone  = 0;
  static unsigned int minMicrophone    = MAX_INT;
  static unsigned int maxMicrophone    = 0;
  static unsigned int valueLight       = 0;
  static unsigned int valueTemperature = 0;
  static unsigned int tick             = 0;
  
  valueMicrophone  = analogRead(PORT_MICROPHONE );
  valueLight       = analogRead(PORT_LIGHT      );
  valueTemperature = analogRead(PORT_TEMPERATURE);

  if (valueMicrophone < minMicrophone) {
    minMicrophone = valueMicrophone;
  }

  if (valueMicrophone > maxMicrophone) {
    maxMicrophone = valueMicrophone;
  }

  if ((tick % 255) == 0 ) {
    //output data for the processing GUI once in while
    Serial.print(maxMicrophone - minMicrophone);
    Serial.print(',');
    Serial.print(  map(valueLight,         0, 700, 0, 100));
    Serial.print(',');
    Serial.println(map(valueTemperature, 400, 500, 0, 100));  

    minMicrophone    = MAX_INT;
    maxMicrophone    = 0;
  }

  tick++;
}

