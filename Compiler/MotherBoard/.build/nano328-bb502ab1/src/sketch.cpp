#include <Arduino.h>
#include <font4x6.h>
#include <TVout.h>
#include <video_gen.h>
char readFromBus();
void programMemory();
void setPinModes();
void setup();
void loop();
#line 1 "src/sketch.ino"
//this is the versiUIIUTYIOon without the keyboard for the final exam,
//once done create the keyboard module, for now keep the keyboard flag off in order to use conditional jumps
//thanks
//#include <font4x6.h>

//#include <TVout.h>
//#include <video_gen.h>

//tv
#define TV_OUT_SIGNAL 7
#define TV_OUT_CLK 9
//keyboard
#define KEYBOARD_SIGNAL 8
#define KEYBOARD_CLK 3
//programmer
#define SHIFT_DATA 2
#define SHIFT_LATCH 4
#define SHIFT_CLK 3
//controls
#define LI 6
#define LM 10
#define KO 11
//flags
#define KR 1 
//programming controls
#define PROGRAM_DONE 12
//clock
#define CLK 13
//bus pins
#define BUS_PIN_7 14
#define BUS_PIN_0 19

TVout TV;

const uint16_t program[256] PROGMEM = {0x00,0x01,0x2c,0x11,0x01,0x2d,0x11,0x01,0x2e,0x11,0x01,0x21,0x16,0x21,0x04,0x21,0x08,0x01,0x22,0x10,0x01,0x12,0x16,0x01,0x04,0x12,0x15,0x21,0x0d,0x20,0x0b,0x11,0x1e,0x04,0x47,0x72,0x65,0x65,0x74,0x69,0x6e,0x67,0x73,0x22,0x38,0x0f,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};

unsigned char x, y;
unsigned char bus;
boolean hasProgrammed = false;
boolean currentClock = false;

char readFromBus(){
  char toOut = 0;
  for(int busPin = BUS_PIN_0; busPin <= BUS_PIN_7;  busPin += 1){
    TV.print(digitalRead(busPin), BIN); 
    toOut = (toOut << 1) + digitalRead(busPin);
  }
  return toOut;
}

void programMemory(){
  hasProgrammed = true;
  TV.println("Ribo 101, Booting...");
   for(int i = 0; i < 256; i++){
     //TV.print("address: ");
     //TV.print(i);
     //TV.print(" data: ");
     //TV.println(pgm_read_byte(&(program[i])));
      shiftOut(SHIFT_DATA, SHIFT_CLK, LSBFIRST, pgm_read_byte(&(program[i])));
      shiftOut(SHIFT_DATA, SHIFT_CLK, LSBFIRST, i);
            
      delay(1);
      digitalWrite(SHIFT_LATCH, LOW);
      delay(1);
      digitalWrite(SHIFT_LATCH, HIGH);
      delay(1);
      digitalWrite(SHIFT_LATCH, LOW);
      delay(1);
      digitalWrite(PROGRAM_DONE, HIGH);
      delay(1);
      digitalWrite(PROGRAM_DONE, LOW);
      delay(20);
      digitalWrite(PROGRAM_DONE, HIGH);  
      delay(1);
   }
   TV.println("Done, turn the switch off to run the program!");
   delay(2000);
   TV.clear_screen();
}

void setPinModes(){
  pinMode(PROGRAM_DONE, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(CLK, INPUT);
  
  for(int i = BUS_PIN_0; i <= BUS_PIN_7; i++){
   pinMode(i, INPUT);
  }
}

void setup(){  
  TV.begin(NTSC);
  TV.select_font(font4x6);
  //setting up the pinmodes for everything
  setPinModes();
  if(!hasProgrammed) programMemory();
  delay(1000);
}

void loop(){
  if(digitalRead(CLK) != currentClock){
    currentClock = !currentClock;
    //do the stuffs for the clock sync
    if(digitalRead(LI)){
     TV.print("\nLICommandPush\n");
     TV.print(readFromBus(), BIN);
     TV.print(readFromBus());
    } 
  }
}

