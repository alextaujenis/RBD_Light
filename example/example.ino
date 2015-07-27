// include files
#include <Light.h>

// static values
int BAUD = 11500;

// variables
String serial_buffer = "";
Light LED(13);
int on_time   = 0;
int off_time  = 0;
int times     = 0;

void setup() {
  // start the serial connection
  Serial.begin(BAUD);
}

void loop() {
  // keep the light going in real-time
  LED.update();

  // capture incoming serial data
  if (Serial.available()) {
    serial_buffer += char(Serial.read());

    // API Command blink: on_time,off_time,times;
    if (serial_buffer.indexOf(";") > -1) {
      // parse out the values
      on_time  = split(serial_buffer,',',0).toInt();
      off_time = split(serial_buffer,',',1).toInt();
      times    = split(serial_buffer,',',2).toInt();

      // build a response string
      Serial.print("Blinking: ");
      Serial.print(on_time);
      Serial.print(" ");
      Serial.print(off_time);
      Serial.print(" ");
      Serial.println(times);

      // start the blinking
      LED.blink(on_time, off_time, times);

      // clear the buffer
      serial_buffer = "";
    }
  }
}

// helper method to split a string
String split(String data, char separator, int index)
{
  int max_index = data.length()-1;
  String chunk = "";
  int x = 0;
  for(int i = 0; i <= max_index && x <= index; i++) {
    chunk.concat(data[i]);
    if(data[i] == separator) {
      x++;
      if(x > index) {
        chunk.trim();
        return chunk;
      }
      chunk = "";
    }
  }
}
