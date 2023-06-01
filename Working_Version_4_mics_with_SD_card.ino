/* GI Stethoscope Fall 2022- Winter 2023.
 
 *  Developed by Luke Haws and Kailey Fitzgerald.
 *  Note this uses the Audio Design GUItool which autogenerates the first main block of code
 *  https://www.pjrc.com/teensy/gui/
 *  
 *  Code status: trial version, unknown functionality. 
 *  This version of the code opens the SD card and writes to it on every iteration of loop()
 *  It may 'work' but is highly inefficient and needs considerable reworking
 *  
 */
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code

//This portion of the code was generated using the teensy audio design tool 
//We selected the i2s_quad, amps, and peaks and the tool generates code that builds the 
//association between each object
AudioInputI2SQuad        i2s_quad1;      //xy=111,94
AudioAmplifier           amp1;           //xy=259,74
AudioAmplifier           amp3;           //xy=270,170
AudioAmplifier           amp2;           //xy=275,123
AudioAmplifier           amp4;           //xy=281,221
AudioAnalyzePeak         peak2;          //xy=417,125
AudioAnalyzePeak         peak4;          //xy=419,248
AudioAnalyzePeak         peak1;          //xy=422,73
AudioAnalyzePeak         peak3;          //xy=429,189
AudioConnection          patchCord1(i2s_quad1, 0, amp1, 0);
AudioConnection          patchCord2(i2s_quad1, 1, amp2, 0);
AudioConnection          patchCord3(i2s_quad1, 2, amp3, 0);
AudioConnection          patchCord4(i2s_quad1, 3, amp4, 0);
AudioConnection          patchCord5(amp1, peak1);
AudioConnection          patchCord6(amp3, peak3);
AudioConnection          patchCord7(amp2, peak2);
AudioConnection          patchCord8(amp4, peak4);
// GUItool: end automatically generated code

//Here we initialize necessary components for the SD card
// SD card chip select
const int SD_CS_PIN = BUILTIN_SDCARD;

File myFile;

unsigned long myTime;

String dataString;


//In setup, we applied a gain of 5, but played around with this number
// while experimenting, we recommend adjusting this number to see what provides 
//optimal results 
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  AudioMemory(50);
  amp1.gain(5);
  amp2.gain(5);
  amp3.gain(5);
  amp4.gain(5);
  Serial.begin(115200);


//adding a statement that prints to indicate that the SD card was initialized properly
  Serial.print("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

}

void loop() {
  // put your main code here, to run repeatedly:
  myTime = millis();
  //Check to see that there are peaks available for each microphone
  if (peak1.available() && peak2.available() && peak3.available() && peak4.available()) {
    //establish peak as a variable float value and print it to the monitor for each peak
    float p1 = peak1.read();
    Serial.print(p1);
    Serial.print(", ");
    
    float p2 = peak2.read();
    Serial.print(p2);
    Serial.print(", ");
    
    float p3 = peak3.read();
    Serial.print(p3);
    Serial.print(", ");

    float p4 = peak4.read();
    Serial.print(p4);
    Serial.print("\n");

    //put the vaules in string format for writing to the SD card
    dataString = String(myTime) + "," + String(p1) + "," + String(p2) + "," + String(p3) + "," + String(p4);
    Serial.println(dataString);


  //write to SD card and print whether it worked or failed
    myFile = SD.open("data.csv", FILE_WRITE);
    if (myFile) {
      myFile.println(dataString);
      myFile.close();
    } else {
      Serial.println("error opening data.csv");
    }
  }
}
