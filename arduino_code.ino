int val;
int availableBytesToRead;
int prevstate = 1;

int midC = 60; // MIDI note value for middle C on a standard keyboard
byte noteON = 144; // the function of the button, 144 = Channel 1 Note on
byte note = 63;
byte velocity = 127;

byte buf[256];

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(34, OUTPUT); // The LED on GND and PIN 34
  pinMode(30, INPUT_PULLUP); // The switch on VCC (+5v) and PIN 30
  
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {

  // Read incoming MIDI from USB and control the LED

  availableBytesToRead = Serial.available();
  if (availableBytesToRead > 0) {
    Serial.readBytes(buf, availableBytesToRead);
    
    if (buf[0] == 0x90) {
      digitalWrite(34, HIGH);
    }

    if (buf[0] == 0x80) {
      digitalWrite(34, LOW);
    }
  }

  // Read the button and transmit MIDI
  val = digitalRead(30);

  if (prevstate == 1 && val == 0) {
    //Serial.println("Button pushed");
    MIDImessage(noteON + 7, note, velocity);
  }

  if (prevstate == 0 && val == 1) {
    //Serial.println("Button released");

     // The note off signal is not needed for changing stops in Hauptwerk; This would cause unnecesary data traffic on the MIDI connection.
    //MIDImessage(noteON + 7, note, 0);
  }

  prevstate = val;

  delay(10);
}

void MIDImessage(byte status, byte data1, byte data2)
{
  Serial.write(status);
  Serial.write(data1);
  Serial.write(data2);
}
