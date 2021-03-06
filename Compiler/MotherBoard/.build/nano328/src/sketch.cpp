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
#define BUS_PIN_0 14
#define BUS_PIN_7 21

TVout TV;

//Important: this one must not change his location, the compiler modifies this one
const uint16_t program[256] PROGMEM = {0x00,0x100,0x09,0x7d,0x0f,0x52,0x02,0x58,0x09,0x8a,0x09,0x7d,0x0f,0x01,0x04,0x7b,0x0f,0x59,0x02,0x64,0x09,0x8a,0x01,0x7b,0x16,0x01,0x04,0x7b,0x09,0xac,0x15,0x79,0x0d,0x28,0x02,0x7a,0x09,0xba,0x0b,0x16,0x0f,0x01,0x04,0x7b,0x09,0x7d,0x0f,0x65,0x02,0x70,0x09,0x8a,0x01,0x7b,0x16,0x01,0x04,0x7b,0x09,0xac,0x15,0x79,0x0d,0x48,0x18,0x0f,0x02,0x09,0xd2,0x08,0x0b,0x2c,0x09,0x7d,0x0f,0x71,0x02,0x78,0x09,0x8a,0x0b,0x02,0x53,0x61,0x6c,0x76,0x65,0x22,0x06,0x54,0x61,0x62,0x65,0x6c,0x6c,0x69,0x6e,0x61,0x3a,0x22,0x0b,0x50,0x6f,0x74,0x65,0x6e,0x7a,0x65,0x3a,0x32,0x5e,0x22,0x0b,0x47,0x72,0x61,0x7a,0x69,0x65,0x22,0x07,0x08,0x0a,0x01,0x02,0x01,0xb4,0x11,0x01,0xb5,0x11,0x01,0xb6,0x11,0x01,0xb7,0x11,0x0a,0x04,0xb3,0x05,0xb2,0x01,0xb2,0x17,0x01,0x04,0xb2,0x0d,0xa3,0x01,0xb3,0x1f,0x00,0x10,0x01,0xb3,0x16,0x01,0x04,0xb3,0x0b,0x8e,0x0a,0x01,0xb8,0x11,0x0a,0x01,0xb9,0x11,0x0a,0x16,0x30,0x10,0x17,0x30,0x0a,0x00,0x00,0x38,0x06,0x01,0x0f,0x80,0xc0,0x04,0xf5,0x04,0xf6,0x05,0xf7,0x01,0xf7,0x17,0x01,0x04,0xf7,0x01,0xf6,0x0d,0xd1,0x06,0xf5,0x04,0xf6,0x08,0x0b,0xc0,0x0a,0x04,0xf8,0x05,0x00,0x01,0x00,0x17,0x01,0x04,0x00,0x01,0xf8,0x0d,0xe6,0x06,0xf8,0x04,0xf8,0x0b,0xd6,0x0a,0xf6,0xf5,0x00,0xf7,0xf6,0xf5,0xf6,0x00,0x00,0xf7,0x01,0xf7,0x00,0xf7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};

unsigned char x, y;
unsigned char bus;
boolean hasProgrammed = false;
boolean currentClock = false;

//for the matrix
int LCDMatrixData[3];
uint8_t LCDMatrixPointer = 0;

char readFromBus(){
  char toOut = 0;
  for(int busPin = BUS_PIN_7; busPin >= BUS_PIN_7;  busPin -= 1){
    toOut = (toOut << 1) + digitalRead(busPin);
  }
  TV.println(toOut, BIN);
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
      delay(10);
      digitalWrite(PROGRAM_DONE, LOW);
      delay(50);
      digitalWrite(PROGRAM_DONE, HIGH);  
      delay(10);
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
    //screen writing
    if(digitalRead(LM)){
    	switch(LCDMatrixPointer){
    		case 0:
    			LCDMatrixData[0] = readFromBus();
    			LCDMatrixPointer = 1;
    			break;
    		case 1:
    			LCDMatrixData[1] = readFromBus();
    			LCDMatrixPointer = 2;
    			break;
    		case 2:
    			LCDMatrixData[2] = readFromBus();
    			//print
    			TV.print_char(LCDMatrixData[0], LCDMatrixData[1], LCDMatrixData[2]);
    			LCDMatrixPointer = 0;
    			break;
    		default:
    			TV.print("FATAL ERROR");
    			break;
    	}
    }
    else if(digitalRead(LI)){
     	TV.print(readFromBus());
    } 
  }
}

