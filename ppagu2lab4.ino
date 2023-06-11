/*
   1- Author: Francis Pagulayan - 665900279
   2- Lab: Lab 4 - Multiple Inputs and Outputs 
   3- Description: In your own words, what is this code supposed to do?
    This code takes an analog joystick and photoresistor as input and has a piezo speaker, and 4 LEDs arranged along the axis of the joystick as output. 
    The code can be split into two systems. The first involves the LEDs and joystick. Each LED corresponds to each direction of the joystick and lights up according to the movement of the joystick.
    The second system plays the Pink Panther theme and speeds up if the photo resistor detects an elevated light level and slows down if the light level is lower than the recorded ambient level. 
   4-Lab Questions:
What does the map() function do?
  The map function allows you to convert a value from one range to another.
How did you change the speed of the melody in your code?
  I changed the speed by taking the light level for each note and if it is above a certain threshehold of the ambient light, the note duration is increased and vice versa if the light is below a certain level. 
What was the hardest part of the lab to implement?
  The hardest part of the lab was figuring out how to change the speed of the melody. 
   5- References: https://docs.google.com/document/d/18xOdmDwbZgGrmGrn0Z0zrDADfP3N7C5fX_sHvYOq3fE/edit#
   https://lastminuteengineers.com/joystick-interfacing-arduino-processing/
   https://www.arduino.cc/reference/en/language/functions/math/map/
   https://www.circuitbasics.com/how-to-use-photoresistors-to-detect-light-on-an-arduino/
   https://github.com/robsoncouto/arduino-songs
   6- Demo: Demonstration uploaded to VoiceThread
*/


// Arduino pin numbers
const int SW_pin = 8; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
int photoPin = A2;
int x_base;
int y_base;
int ambientLight;

#define LEFT_LED 2
#define UP_LED 3
#define RIGHT_LED 4
#define DOWN_LED 5

// MELODY
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

// change this to make the song slower or faster
int tempo = 120;

// change this to whichever pin you want to use
int buzzer = 11;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Pink Panther theme
  // Score available at https://musescore.com/benedictsong/the-pink-panther
  // Theme by Masato Nakamura, arranged by Teddy Mason

  REST,2, REST,4, REST,8, NOTE_DS4,8, 
  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
  NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, 
  NOTE_E4,2, REST,4, REST,8, NOTE_DS4,4,

  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_G4,8, NOTE_B4,-8, NOTE_E5,8,
  NOTE_DS5,1,   
  NOTE_D5,2, REST,4, REST,8, NOTE_DS4,8, 
  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
  
  NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, 
  NOTE_E4,-4, REST,4,
  REST,4, NOTE_E5,-8, NOTE_D5,8, NOTE_B4,-8, NOTE_A4,8, NOTE_G4,-8, NOTE_E4,-8,
  NOTE_AS4,16, NOTE_A4,-8, NOTE_AS4,16, NOTE_A4,-8, NOTE_AS4,16, NOTE_A4,-8, NOTE_AS4,16, NOTE_A4,-8,   
  NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, NOTE_E4,16, NOTE_E4,16, NOTE_E4,2,
 
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;


void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
  pinMode(LEFT_LED, OUTPUT);
  digitalWrite(LEFT_LED, LOW);
  pinMode((UP_LED), OUTPUT);
  digitalWrite(UP_LED, LOW);
  pinMode(RIGHT_LED, OUTPUT);
  digitalWrite(RIGHT_LED, LOW);
  pinMode(DOWN_LED, OUTPUT);
  digitalWrite(DOWN_LED, LOW);
  x_base = analogRead(X_pin);
  y_base = analogRead(Y_pin);
}

void loop() {
  // Recalibrates ambient light level in the room
  ambientLight = analogRead(photoPin);
  // Loops through each note of the melody
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    //Joystick Left
    if (analogRead(X_pin) < x_base-100) {
      digitalWrite(LEFT_LED, HIGH);
      digitalWrite(RIGHT_LED, LOW);
    } else if (analogRead(X_pin) > x_base+100) { //Joystick Right
      digitalWrite(LEFT_LED, LOW);
      digitalWrite(RIGHT_LED, HIGH);
    } else { // Reset joystick
      digitalWrite(LEFT_LED, LOW);
      digitalWrite(RIGHT_LED, LOW);
    }
    // Joystick Up
    if (analogRead(Y_pin) < y_base-100) {
      digitalWrite(UP_LED, HIGH);
      digitalWrite(DOWN_LED, LOW);
    } else if (analogRead(Y_pin) > y_base+100) { //Joystick Down
    digitalWrite(UP_LED, LOW);
    digitalWrite(DOWN_LED, HIGH);
    } else { //Reset Joystick
    digitalWrite(UP_LED, LOW);
    digitalWrite(DOWN_LED, LOW);
    }
  
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    int light = analogRead(photoPin);
    if (light > ambientLight + 10) { 
      noteDuration = noteDuration / 2;
    } else if (light < ambientLight - 10) {
      noteDuration = noteDuration * 2;
    }

    Serial.println(light);    
    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}