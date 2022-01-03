int val;
int availableBytesToRead;
int prevstate = 1;

int midC = 60; // MIDI note value for middle C on a standard keyboard
byte noteON = 144; // the function of the button, 144 = Channel 1 Note on
byte note = 63;
byte velocity = 127;

byte buf[256];

int noteStates[256];

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.

  // Set 11 datapins to output per row the columns (x coordinate)
  for (int i=30;i<30+11;i++) {
    pinMode(i, OUTPUT);
  }

  // The IO pins for the rows, 2 for now.
  for (int i=50;i<52;i++) {
    pinMode(i, OUTPUT);
  }
  
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {

  // Read incoming MIDI from USB and control the LED

  availableBytesToRead = Serial.available();
  
  while (availableBytesToRead > 0) {
    Serial.readBytes(buf, 3);

    val = buf[1];
    val -= 60; // Shift the first note (middle c) to position 0

    if (buf[0] == 0x90) { // Note on
      noteStates[val] = HIGH;
        //digitalWrite(val, HIGH);
    }

    if (buf[0] == 0x80) { // Note off
      noteStates[val] = LOW;
      //digitalWrite(val, LOW);
    }

    // Keep looping until all MIDI messages are read.
    availableBytesToRead = Serial.available();
  }

  updateLEDMatrix();

/*
 * No buttons atm

  // Read the button and transmit MIDI
  val = digitalRead(30);

  if (prevstate == 1 && val == 0) {
    //Serial.println("Button pushed");
    MIDImessage(noteON + 7, note, velocity);
  }

  if (prevstate == 0 && val == 1) {
    //Serial.println("Button released");

    MIDImessage(noteON + 7, note, 0);
  }

  prevstate = val;
 */
  //delay(10);
}

void updateLEDMatrix() {

  // Run through all the rows and columns and switch them on and off after each other.

  // There are 23 lights currently. Don't turn on any more after that.
  
  // There are 12 lights per row.

  int currentRow = 0;
  int currentColumn = 0;

  // Turn the first row selector on.
  digitalWrite(currentRow + 50, HIGH); // Start with the first row at IO pin 50.
  
  for (int note = 0;note <= 23; note++) {

    digitalWrite(currentColumn + 30, noteStates[note]); // Start with the first column at IO pin 30.

    currentColumn++;

    if (currentColumn > 11) { // 0 is also a column, so once this becomes 12, we would be moving onto column 13 while there are just 12.
      // Let the current row shine for a little bit.
      delay(5);
      
      currentColumn = 0; // Back to column 0.

      // Turn the previous row off.
      digitalWrite(currentRow + 50, LOW);
      
      currentRow++; // Next row.

      // Turn the new row on.
      digitalWrite(currentRow + 50, HIGH);
    }
  }

  // Let the last row shine for a little bit.
  delay(5);
  digitalWrite(currentRow + 50, LOW);
  
}

void MIDImessage(byte status, byte data1, byte data2)
{
  Serial.write(status);
  Serial.write(data1);
  Serial.write(data2);
}
