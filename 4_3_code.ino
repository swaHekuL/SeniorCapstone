#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
// Sets up variables and does other various Audio related things
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

// SD card chip select - selects SD card chip
const int SD_CS_PIN = BUILTIN_SDCARD;

// declares timer for interrupt timing
IntervalTimer myTimer;

// declares file tp be written to
FsFile myFile;

// declares variable to track run time
unsigned long myTime;

// misc 
int rowCount = 0;


// string for data
String dataString;

// declares floats for audio recording
float p1 = 0;
float p2 = 0;
float p3 = 0;
float p4 = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT); // sets built in LED as output
  AudioMemory(50); // reserves # of "blocks" for all audio connections, each block holds 128 audio samples or roughly 2.9ms of sound. should implement AudioMemoryUsageMax() with this
  amp1.gain(5); // sets gain of each amp to amplify levels
  amp2.gain(5);
  amp3.gain(5);
  amp4.gain(5);
  Serial.begin(115200); // sets serial monitoring rate
  myTimer.begin(read, 50); // interrupt timer, put the function you want to run at each interrupt in the first parameter. second declares interval in ms

  // initialize sd card
  Serial.print("Initializing SD card...");
  // if init fails
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // create a new file. this makes it so it makes a new file everytime without me having to do anything. copy and pasted from internet
  char filename[] = "loggedData00.CSV"; 
  for (uint8_t i = 0; i < 100; i++) {
    filename[10] = i/10 + '0';
    filename[11] = i%10 + '0';
    if (! SD.sdfs.exists(filename)) {
      // only open a new file if it doesn't exist
      myFile = SD.sdfs.open(filename, O_WRITE | O_CREAT);
      myFile.preAllocate(524288000);
      Serial.println("File opened or created.");
      break;  // leave the loop!
    }
  }  

}

// read function, grabs a reading from each mic. currently reads peaks, there are definitely better ways than just peak reading.
void read() {
  myTime = micros(); // gets the time
  if (peak1.available()){
  p1 = peak1.read();
  }
  
  if (peak2.available()){
  p2 = peak2.read();
  }

  if (peak3.available()){  
  p3 = peak3.read();
  }

  if (peak4.available()){
  p4 = peak4.read();
  }
  // builds a string of time and each reading to write. byte type data would be more efficient, just couldnt ever figure it out.
  dataString += String(myTime) + "," + String(p1) + "," + String(p2) + "," + String(p3) + "," + String(p4) + "\n";
  // row tracking so that the code only writes after 1000 lines. then clears row count after
  rowCount ++;
  if (rowCount >= 1000) {
    write();
    rowCount = 0;
  }
}

// writes data to sd card, clears data afterwards. 
void write() {
  for (int row = 0; row <= 256; row ++) { // loop iteration cutoff should matchup with rowcount cutoff above. could define sep variable for this.
    myFile.println(dataString); // prints datastring to file and then clears it
    dataString = "";
  }
  myFile.flush(); // these are necessary. basically refreshes the sd card each time, makes sure it knows what files are there without having to close it repeatedly
  myFile.sync();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
  
