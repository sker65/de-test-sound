
#define BUTTON 52
#define DATAEAST 1
#define WPC 2

void setup() {
  // set gpio mode
  DDRC = 255; // port c komplett ausgang
  DDRA = 255; // port a komplett auf ausgang
  PORTA = 255; // high
  PORTC = 255; // high
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}


int data[] = { 
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // main music
  0x40, 0x41, 0x42, 0x43, 0x44,       // effects / callout
  0,0
  };

int lenData = 8*4 + 2;

/*
 * sound address is 0x1C = port C bit 0-4 ---> pin 30 - 37  PC0:37 - PC7:30 
 * port c bit 5 = RW -> low
 * port c bit 6 = WDEN -> pluse to low
 */

// PA0:22 -> CD0 de3
// PA1:23 -> CD1 de4
// PA2:24 -> CD2 de5
// PA3:25 -> CD3 de6
// PA4:26 -> CD4 de7
// PA5:27 -> CD5 de8
// PA6:28 -> CD6 de9
// PA7:29 -> CD7 de10



// PC6:31 -> de13 = STROBE
 
int ledState = 0;
int mode = DATAEAST;
unsigned int t = 0;

void sendCmd(int c ) {
    delayMicroseconds(500);
    if( mode == WPC ) { // for wpc always use 2 byte commands
      PORTA = 0;
      PORTC = 0b10111111;      // strobe low
      delayMicroseconds(20);
      PORTC = 0b11111111;      // strobe high  
      delayMicroseconds(80);
    }
    PORTA = c;
    PORTC = 0b10111111;      // strobe low
    delayMicroseconds(20);
    PORTC = 0b11111111;      // strobe high  
}

void play(int start, int end, int d ) {
  for( int i = start; i <= end; i++ ) {
   
    sendCmd(data[i]);
    delay(d);
  }
}

void toggleLed() {
  digitalWrite(LED_BUILTIN, ledState);
  ledState = !ledState;
}

int c = 0;

void toggleMode() {
  mode++;
  if( mode > WPC ) mode = DATAEAST;
}

void loop() {
  t = millis();
  if( digitalRead( BUTTON ) == LOW ) {
    toggleMode();  
  }
  
  sendCmd(0);
  play(0,5,5000);

  sendCmd(0);

  sendCmd(1);

  play(6,10,1000);

  delay(5000);

  c++;
  
}
