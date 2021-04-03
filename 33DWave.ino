// SIT210 3.3D

// define the analogue pin we'll be using
const pin_t LED = A5;
// the event to subscribe to
String event = "Deakin_RIOT_SIT210_Photon_Buddy";
// the possible data from the event
String waveData = "wave";
String patData = "pat";
// how long to pause between breaths
const int breathDelay = 200;
// how many flashes to do for a wave
const int waveFlashes = 3;
// the pause in between wave flashes
const int wavePause = 500;
// the max value an LED can be lit to
const int maxLEDBrightness = 255;
// the brightness incriment/decriment for breathing
const int breathIncriment = 5;
// the delay in breathing brightness incriments
const int breathIncrimentDelay = 19;
// the max numbr of flashes in party mode
const int maxPartyFlashes = 6;
// the max on/off time for party mode in millis
const int maxPartyFlashTime = 1000;

// handy little line I found that means your program will start running before the
// device is connected to the cloud. 
SYSTEM_THREAD(ENABLED);

// sets up the program
void setup() {
    // subscribe to the events we're interested in
    Particle.subscribe(event, handler);
    // start the serial port so we can see output there as well
    Serial.begin(9600);
    while(!Serial) 
    Serial.println("All OK...");
    // set the pin mode to output
    pinMode(LED, OUTPUT);
}

// this was boring when it didn't do anything while it waited. 
// So I made it 'breath' while it's waiting. Inspiration for this 
// came from dfrobot.com
void loop() {
    breathIn();
    breathOut();
    delay(breathDelay);
}

// the function called when the subscribed event happens
void handler(String event, String data) {
    // switch on the data value being recieved
    if (data == waveData) {
        Serial.println("waving");
        flashLED(waveFlashes);
    } else if (data == patData) {
        Serial.println("patting");
        partyLED();
    }
}

// flashes the LED a certain number of times at full brightness
void flashLED(int times) {
    for (int i = 0; i < times; i++) {
        analogWrite(LED, maxLEDBrightness);
        delay(wavePause);
        analogWrite(LED, 0);
        delay(wavePause);
    }
}

// fades the LED to full brightness
void breathIn() {
    for (int i = 0; i < maxLEDBrightness; i += breathIncriment) {
        analogWrite(LED, i);
        delay(breathIncrimentDelay);
    }
}

// fades the lED to off
void breathOut() {
    for (int i = maxLEDBrightness; i > 0; i -= breathIncriment) {
        analogWrite(LED, i);
        delay(breathIncrimentDelay);
    }
}

// this was supposed to look like a party, but instead all I can see is the
// Spongebob meme:
/*
 -----------------------------------------
|                                         |
| tHiS iS SuPpoSeD tO bE a PaRtY          |
|                                         |
 -----------------------------------------
  \\
   \\    *
          *
     ----//-------
     \..C/--..--/ \   `A
      (@ )  ( @) \  \// |w
       \          \  \---/
        HGGGGGGG    \    /`
        V `---------`--'
            <<    <<
           ###   ###

*/
// (thanks to pypi.org for the ascii art!)
// I still like it though, so in it stays.
void partyLED() {
    // flash a random number of times up to designated top barrier
    // + 1 added through here as the max number in random is exclusive, 
    // so we have to add one to actually possibly get the max number chosen
    // above.
    int flashes = random(maxPartyFlashes + 1);
    for (int i = 0; i < flashes; i++) {
        // use a random brightnes from 0 to 255 (the max)
        int brightness = random(maxLEDBrightness + 1);
        // stay on and off for random times up to 1 second
        int on = random(maxPartyFlashTime + 1);
        int off = random(maxPartyFlashTime + 1);
        
        // turn the LED on with the above random settings
        analogWrite(LED, brightness);
        // wait the random time
        delay(on);
        // turn it off
        analogWrite(LED, 0);
        // wait the random time before continuing
        delay(off);
    }
}
