#include <CapacitiveSensor.h>
#include <Encoder.h>
#include <MIDI.h>


struct CustomBaudRate : public midi::DefaultSettings{

    static const long BaudRate = 115200;

};

MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, CustomBaudRate);

//threshold for capacitive sensing. Modify to suit your needs. 
int threshold = 110;
int midiNote = 40;
int velocity = 127;

// Rotary Encoder
#define CLK 10
#define DT 11
#define SW 12
long oldPosition  = -999;

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

//setting the capacitive sensors (1 sensor per country)
CapacitiveSensor   cs_4_5 = CapacitiveSensor(4,5);    //Paraguay     // 10M resistor between pins 4 & 5, pin 6 is sensor pin, add a wire and a pin to your desired country on your globe. 
CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);    //Australia       // 10M resistor between pins 4 & 6, pin 7 is sensor pin, add a wire and a pin to your desired country on your globe. 
CapacitiveSensor   cs_4_7 = CapacitiveSensor(4,7);    //Zimbabwe         // 10M resistor between pins 4 & 7, pin 8 is sensor pin, add a wire and a pin to your desired country on your globe. 
CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,8);    //China       // 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and a pin to your desired country on your globe. 
CapacitiveSensor   cs_4_9 = CapacitiveSensor(4,9);    //Belgium          // 10M resistor between pins 4 & 9, pin 9 is sensor pin, add a wire and a pin to your desired country on your globe. 

bool noteOn1 = false;
bool noteOn2 = false;
bool noteOn3 = false;
bool noteOn4 = false;
bool noteOn5 = false;


void setup()                    
{
    // Set encoder pins as inputs
    pinMode(CLK,INPUT);
    pinMode(DT,INPUT);
    pinMode(SW, INPUT_PULLUP);
    //begin serial communication
    Serial.begin(9600);

   	// Read the initial state of CLK
    lastStateCLK = digitalRead(CLK);

   //begin midi
    MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop()                    
{
    //update the encoder for the globe position.
    updateEncoder();
    //update capacitive sensors to find out if a country has been touched. 
    updateCapacitiveSensors();
}


void updateEncoder(){
        
	// Read the current state of CLK
	currentStateCLK = digitalRead(CLK);

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(DT) != currentStateCLK) {
			counter --;
			currentDir ="CCW";
      midiNote --;
		} else {
			// Encoder is rotating CW so increment
			counter ++;
			currentDir ="CW";
      midiNote ++;
		}

		// Serial.print("Direction: ");
		// Serial.print(currentDir);
		// Serial.print(" | Counter: ");
		// Serial.println(counter);
	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;
  delay(1);
 }

void updateCapacitiveSensors(){
    long total1 =  cs_4_5.capacitiveSensor(30);   //Madagascar
    long total2 =  cs_4_6.capacitiveSensor(30);   //Namibia
    long total3 =  cs_4_7.capacitiveSensor(30);   //France
    long total4 =  cs_4_8.capacitiveSensor(30);   //Ethiopia
    
    long total5 =  cs_4_9.capacitiveSensor(30);   //China
    // long total6 =  cs_4_10.capacitiveSensor(30);  //Vietnam
    // long total7 =  cs_4_11.capacitiveSensor(30);  //Japan
    // long total8 =  cs_4_12.capacitiveSensor(30);  //Philippines
    
    // Serial.print("1: ");
    // Serial.println(total1);                  // print sensor output 1
    // delay(1000);
    // Serial.print("2: ");
    // Serial.println(total2);                  // print sensor output 2
    // Serial.print("3: ");
    // Serial.println(total3);                  // print sensor output 3
    // Serial.print("4: ");
    // Serial.println(total4);                // print sensor output 4
    
    // Serial.print("5: ");
    // Serial.println(total5);                // print sensor output 5
    // Serial.print("\t");
    // Serial.print(total6);                // print sensor output 6
    // Serial.print("\t");
    // Serial.print(total7);                // print sensor output 7
    // Serial.print("\t");
    // Serial.println(total8);                // print sensor output 8
    
    if(total1 > threshold){
      if (!noteOn1) {
        MIDI.sendNoteOn(midiNote, sqrt(total1)*3.5, 1);  // Send a Note (pitch 42, velo 127 on channel 1)
        noteOn1 = true;
        delay(10);
      }
    } else {
      if (noteOn1) {
        MIDI.sendNoteOff(midiNote,0,1);   // Stop the note
        noteOn1 = false;
      }
    }

    if(total2>threshold){
      if (!noteOn2) {
        MIDI.sendNoteOn(midiNote, sqrt(total2)*3.5, 2);  // Send a Note (pitch 42, velo 127 on channel 1)
        noteOn2 = true;
        delay(10);
      }
    } else {
      if (noteOn2) {
        MIDI.sendNoteOff(midiNote,0,2);   // Stop the note
        noteOn2 = false;
      }
    }

    if(total3>threshold){
      if (!noteOn3) {
        MIDI.sendNoteOn(midiNote, sqrt(total3)*3.5, 3);  // Send a Note (pitch 42, velo 127 on channel 1)
        noteOn3 = true;
        delay(10);
      }
    } else {
      if (noteOn3) {
        MIDI.sendNoteOff(midiNote,0,3);   // Stop the note
        noteOn3 = false;
      }
    }

    if(total4>threshold){
      if (!noteOn4) {
        MIDI.sendNoteOn(midiNote, sqrt(total4)*3.5, 4);  // Send a Note (pitch 42, velo 127 on channel 1)
        noteOn4 = true;
        delay(10);
      }
    } else {
      if (noteOn4) {
        MIDI.sendNoteOff(midiNote,0,4);   // Stop the note
        noteOn4 = false;
      }
    }

    if(total5>threshold){
      if (!noteOn5) {
        MIDI.sendNoteOn(midiNote, sqrt(total5)*3.5, 5);  // Send a Note (pitch 42, velo 127 on channel 1)
        noteOn5 = true;
        delay(10);
      }
    } else {
      if (noteOn5) {
        MIDI.sendNoteOff(midiNote,0,5);   // Stop the note
        noteOn5 = false;
      }
    }
  }