#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=842,537
AudioAnalyzePeak         peak1;          //xy=1030,522
AudioAnalyzePeak         peak2;          //xy=1030,557
AudioSynthWaveform       waveform4;      //xy=1209,623
AudioSynthWaveform       waveform3;      //xy=1211,582
AudioSynthWaveform       waveform1;      //xy=1221,514
AudioSynthWaveform       waveform2;      //xy=1222,556
AudioSynthWaveform       waveform5;      //xy=1317,665
AudioSynthWaveform       waveform7;      //xy=1323,739
AudioSynthWaveform       waveform6;      //xy=1337,697
AudioSynthWaveform       waveform8;      //xy=1340,793
AudioMixer4              mixer1;         //xy=1394,404
AudioMixer4              mixer2;         //xy=1546,650
AudioMixer4              mixer3;         //xy=1597,422
AudioOutputI2S           i2s2;           //xy=1677,528
AudioConnection          patchCord1(i2s1, 0, peak1, 0);
AudioConnection          patchCord2(i2s1, 1, peak2, 0);
AudioConnection          patchCord3(waveform4, 0, mixer1, 3);
AudioConnection          patchCord4(waveform3, 0, mixer1, 2);
AudioConnection          patchCord5(waveform1, 0, mixer1, 0);
AudioConnection          patchCord6(waveform2, 0, mixer1, 1);
AudioConnection          patchCord7(waveform5, 0, mixer2, 0);
AudioConnection          patchCord8(waveform7, 0, mixer2, 2);
AudioConnection          patchCord9(waveform6, 0, mixer2, 1);
AudioConnection          patchCord10(waveform8, 0, mixer2, 3);
AudioConnection          patchCord11(mixer1, 0, mixer3, 0);
AudioConnection          patchCord12(mixer2, 0, mixer3, 1);
AudioConnection          patchCord13(mixer3, 0, i2s2, 0);
AudioConnection          patchCord14(mixer3, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=909,681
// GUItool: end automatically generated code

int lastBtnState = HIGH;
int upperPotInput = 20;
int lowerPotInput = 21;
int currWaveform = 0;



#define kMaxPotInput  1024
#define detuneCentsMax 1200
#define pitchOffsetCentsMax 4800

// Use the 1010VCOCalibration sketch to learn how these values were measured
#define kPeakValue1V_IN1   0.2088
#define kPeakValue4V_IN1   0.7627
#define kPeakValue1V_IN2   0.2092
#define kPeakValue4V_IN2   0.7649
#define kC1VFrequency      32.703

void setup()
{
  pinMode(2, INPUT);
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);

  // Enable the audio shield, select input, and enable output
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.volume(0.82);
  sgtl5000_1.adcHighPassFilterDisable();
  sgtl5000_1.lineInLevel(0, 0);
  sgtl5000_1.unmuteHeadphone();

  waveform1.frequency(1000);
  waveform1.amplitude(0.25);
  waveform1.begin(WAVEFORM_SINE);

  waveform2.frequency(1000);
  waveform2.amplitude(0.25);
  waveform2.begin(WAVEFORM_SINE);

  waveform3.frequency(1000);
  waveform3.amplitude(0.25);
  waveform3.begin(WAVEFORM_SINE);

  waveform4.frequency(1000);
  waveform4.amplitude(0.25);
  waveform4.begin(WAVEFORM_SINE);

  waveform5.frequency(1000);
  waveform5.amplitude(0.25);
  waveform5.begin(WAVEFORM_SINE);

  waveform6.frequency(1000);
  waveform6.amplitude(0.25);
  waveform6.begin(WAVEFORM_SINE);

  waveform7.frequency(1000);
  waveform7.amplitude(0.25);
  waveform7.begin(WAVEFORM_SINE);

  waveform8.frequency(1000);
  waveform8.amplitude(0.25);
  waveform8.begin(WAVEFORM_SINE);

  mixer3.gain(0,0.75);
  mixer3.gain(1,0.75);
}

float calcFreqFromCV(float cvValue)
{
  return pow(2, cvValue) * kC1VFrequency;
}

float calcFreqFromPeakValueIN1(float peakValue)
{
  float peakPerOctave = (kPeakValue4V_IN1 - kPeakValue1V_IN1) / 3;
  float cvValue = (peakValue - kPeakValue1V_IN1) / peakPerOctave;
  return calcFreqFromCV(cvValue);
}

float calcFreqFromPeakValueIN2(float peakValue)
{
  float peakPerOctave = (kPeakValue4V_IN2 - kPeakValue1V_IN2) / 3;
  float cvValue = (peakValue - kPeakValue1V_IN2) / peakPerOctave;
  return calcFreqFromCV(cvValue);
}

float centsToRatio(float cents)
{
  return pow(2, cents / 1200);
}

void changeWaveform()
{
  switch(currWaveform)
  {
    case 0:
    waveform1.begin(WAVEFORM_SINE);
    waveform2.begin(WAVEFORM_SINE);
    waveform3.begin(WAVEFORM_SINE);
    waveform4.begin(WAVEFORM_SINE);
    waveform5.begin(WAVEFORM_SINE);
    waveform6.begin(WAVEFORM_SINE);
    waveform7.begin(WAVEFORM_SINE);
    waveform8.begin(WAVEFORM_SINE);
    break;

    case 1:
    waveform1.begin(WAVEFORM_SAWTOOTH);
    waveform2.begin(WAVEFORM_SAWTOOTH);
    waveform3.begin(WAVEFORM_SAWTOOTH);
    waveform4.begin(WAVEFORM_SAWTOOTH);
    waveform5.begin(WAVEFORM_SAWTOOTH);
    waveform6.begin(WAVEFORM_SAWTOOTH);
    waveform7.begin(WAVEFORM_SAWTOOTH);
    waveform8.begin(WAVEFORM_SAWTOOTH);
    break;

    case 2:
    waveform1.begin(WAVEFORM_TRIANGLE);
    waveform2.begin(WAVEFORM_TRIANGLE);
    waveform3.begin(WAVEFORM_TRIANGLE);
    waveform4.begin(WAVEFORM_TRIANGLE);
    waveform5.begin(WAVEFORM_TRIANGLE);
    waveform6.begin(WAVEFORM_TRIANGLE);
    waveform7.begin(WAVEFORM_TRIANGLE);
    waveform8.begin(WAVEFORM_TRIANGLE);
    break;

    case 3:
    waveform1.begin(WAVEFORM_PULSE);
    waveform2.begin(WAVEFORM_PULSE);
    waveform3.begin(WAVEFORM_PULSE);
    waveform4.begin(WAVEFORM_PULSE);
    waveform5.begin(WAVEFORM_PULSE);
    waveform6.begin(WAVEFORM_PULSE);
    waveform7.begin(WAVEFORM_PULSE);
    waveform8.begin(WAVEFORM_PULSE);
    break;
  }
  
}

void loop()
{
  if (peak1.available() && peak2.available())
  {
    int upperPotValue = analogRead(upperPotInput);
    int lowerPotValue = analogRead(lowerPotInput);
    float potDetune = float(upperPotValue) / float(kMaxPotInput);
    potDetune -=0.5f;
    
    float centsMult = (peak2.read() + potDetune) * detuneCentsMax;
    float ratio2 = centsToRatio(centsMult);
    float ratio3 = centsToRatio(-centsMult);
    float ratio4 = centsToRatio(centsMult*2);
    float ratio5 = centsToRatio(-centsMult*2);
    float ratio6 = centsToRatio(centsMult*3);
    float ratio7 = centsToRatio(-centsMult*3);
    float ratio8 = centsToRatio(centsMult*4);
    float peakValue1 = peak1.read();
    
    float baseFreq = calcFreqFromPeakValueIN1(peakValue1) * (centsToRatio((float(lowerPotValue) / float(kMaxPotInput)) * pitchOffsetCentsMax));
    waveform1.frequency(baseFreq);
    waveform2.frequency(baseFreq * ratio2);
    waveform3.frequency(baseFreq * ratio3);
    waveform4.frequency(baseFreq * ratio4);
    waveform5.frequency(baseFreq * ratio5);
    waveform6.frequency(baseFreq * ratio6);
    waveform7.frequency(baseFreq * ratio7);
    waveform8.frequency(baseFreq * ratio8);
  }

  int btnState = digitalRead(2);
  
  if (btnState != lastBtnState)
  {
    lastBtnState = btnState;
    delay(50);
    if (btnState == LOW)
    {
      currWaveform++;
      if (currWaveform > 3) currWaveform = 0;
      changeWaveform();
    }
  }
  delay(2);
}
