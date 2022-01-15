int val;
int availableBytesToRead;
int prevstate = 1;

int midC = 60; // MIDI note value for middle C on a standard keyboard
byte noteON = 144; // the function of the button, 144 = Channel 1 Note on
byte note = 63;
byte velocity = 127;

byte buf[256];

int noteStates[256];

int SER_pin = 28; // IC pin 14 (SER) = data
int SRCLK_pin = 29; // IC pin 11 (SRCLK)
int RCLK_pin = 34; // IC pin 12 (RCLK)
// Don't forget to connect the GND of the Arduino to the GND of the shift register!
// In total you need to have 4 wires going from the Arduino to the shift register.

// the setup function runs once when you press reset or power the board
void setup() {

  pinMode(SER_pin, OUTPUT);
  pinMode(SRCLK_pin, OUTPUT);
  pinMode(RCLK_pin, OUTPUT);

  for (int i = 0; i < 256; i++) {
    noteStates[i] = LOW;
  }

  writereg();
  
  Serial.begin(115200);
}

void writereg() {

  digitalWrite(RCLK_pin, LOW);

  for (int i = 256; i >= 0; i--) {
    digitalWrite(SRCLK_pin, LOW);
    digitalWrite(SER_pin, noteStates[i]);
    digitalWrite(SRCLK_pin, HIGH);
  }

  digitalWrite(RCLK_pin, HIGH);
}


// the loop function runs over and over again forever
void loop() {

  // Easy test to see if the electronic bit is connected correctly.
  /*for (int i = 0; i < 99; i++ ){
    writeNumberTo7SegmentLCD(0, 2, i);
    writereg();
    delay(100);
  }*/


  
  
  // Read incoming MIDI from USB and control the LED

  availableBytesToRead = Serial.available();
  
  while (availableBytesToRead > 0) {
    Serial.readBytes(buf, 3);

    val = buf[1];

    val -= 60;


    // Control Change op channel 1, laatste byte is de waarde
    if (buf[0] == 0xB0 && buf[1] == 0x0B) {
      val = buf[2];

      writeNumberTo7SegmentLCD(0, 2, (int)((float)val / 127 * 60));
      writereg();
    }

    // Keep looping until all MIDI messages are read.
    availableBytesToRead = Serial.available();
  }

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
}

byte displayLEDs[10][8] = { 
        { 0,1,1,1,1,1,1,0 },  // = 0
        { 0,0,0,1,0,0,1,0 },  // = 1
        { 1,0,1,1,1,1,0,0 },  // = 2
        { 1,0,1,1,0,1,1,0 },  // = 3
        { 1,1,0,1,0,0,1,0 },  // = 4
        { 1,1,1,0,0,1,1,0 },  // = 5
        { 1,1,1,0,1,1,1,0 },  // = 6
        { 0,0,1,1,0,0,1,0 },  // = 7
        { 1,1,1,1,1,1,1,0 },  // = 8
        { 1,1,1,1,0,1,1,0 }   // = 9     
        }; 
void writeNumberTo7SegmentLCD(int startPos, int amountDigits, int number) {

  int digits[10];

  //number = 55;
  
  // Convert the number to a string
  String input = String(number);

  // Prepend the string with zeroes until it reaches the length amountDigits.
  while (input.length() < amountDigits) {
    input = "0" + input;
  }

  // Put all the characters into the digits array.
  for (int i = 0; i < amountDigits; i++) {
    digits[i] = String(input.charAt(i)).toInt();
  }

  //digits[1] = digits[0];

  //digits[0] = 8;
  //digits[1] = 5;

/*
 * 0 to 7 are on the 10th's display
 * 8 to 15 are on the 1th's display
 * 
 */
  
  // Loop through the digits  
  for (int i = 0; i < amountDigits; i++) {
    
    // Loop through the pins for the current digit.
    for (int j = 0; j < 8; j++) {
      
      int number = digits[i];
      
      noteStates[startPos + (i*8) + j] = displayLEDs[number][j];
    }
  }
  
}

void MIDImessage(byte status, byte data1, byte data2)
{
  Serial.write(status);
  Serial.write(data1);
  Serial.write(data2);
}
