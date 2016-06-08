import processing.serial.*;

final int PORT_TO_OPEN   = 4;
final int PORT_BAUD_RATE = 115200;
final int PORT_LINE_FEED = 10;      //new line character 

int values[];

Serial myPort;       

void setup() {
  size(200, 200);
  
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[PORT_TO_OPEN], PORT_BAUD_RATE);
  myPort.bufferUntil(PORT_LINE_FEED);
}

void draw() { 
  background(0); 
} 

void serialEvent(Serial myPort) {
  String myString = myPort.readStringUntil(PORT_LINE_FEED);

  if (myString != null) {
    myString = trim(myString);

    values = int(split(myString, ','));
    
    for (int i = 0; i < values.length; i++) {
      print("Sensor " + i + ": " + values[i] + "\t");
    }
    
    println();
  }
}


