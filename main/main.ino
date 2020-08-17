#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "MAX30100.h"

#include "Nextion.h"
#include "nextion_config.h"
#include "filters.h"
#include "page0.h"
#include "page1.h"
#include "page3.h"

#include <Math.h>

#define REPORTING_PERIOD_MS     200
#define ledPin 2

// Sampling is tightly related to the dynamic range of the ADC.
// refer to the datasheet for further info
#define SAMPLING_RATE                       MAX30100_SAMPRATE_100HZ

// The LEDs currents must be set to a level that avoids clipping and maximises the
// dynamic range
#define IR_LED_CURRENT                      MAX30100_LED_CURR_50MA
#define RED_LED_CURRENT                     MAX30100_LED_CURR_27_1MA

// The pulse width of the LEDs driving determines the resolution of
// the ADC (which is a Sigma-Delta).
// set HIGHRES_MODE to true only when setting PULSE_WIDTH to MAX30100_SPC_PW_1600US_16BITS
#define PULSE_WIDTH                         MAX30100_SPC_PW_1600US_16BITS
#define HIGHRES_MODE                        true

PulseOximeter pox;
MAX30100 sensor;

uint32_t tsLastReport = 0, spo2=0, heartRate=0;
uint16_t red, ir;
char str[12];
int mFilteredSamples[100], mFilteredSamples_n=0, mFilteredSamples_size=0;
int mSamples[31], mSamples_n=0, mSamples_size=0;  
int red_mFilteredSamples[100], red_mFilteredSamples_n=0, red_mFilteredSamples_size=0;
int red_mSamples[30], red_mSamples_n=0, red_mSamples_size=0;  

//variaveis alarmes
uint32_t value_spo2_min = 80, value_hr_min = 50, value_hr_max = 150;

int page0_Flag = 0, page1_Flag, page3_Flag = 0, alt=0, i=0, mudancahr=0, mudancaspo2=0;
NexPage page0 = NexPage(0,0,"page0");

NexText t0 = NexText(0,1,"t0");
NexText t1 = NexText(0,2,"t1");
NexText t2 = NexText(0,4,"t2");
NexText t3 = NexText(0,6,"t3");
NexText t5 = NexText(0,8,"t5");
NexText t6 = NexText(0,7,"t6");

NexNumber n0 = NexNumber(0,9,"n0");
NexNumber n1 = NexNumber(0,10,"n1");

NexPicture p0 = NexPicture(0,3,"p0");
NexPicture p1 = NexPicture(0,5,"p1");

NexButton back_0 = NexButton(0,11,"back_0");

NexPage page1 = NexPage(1,0,"page1");

NexWaveform s0 = NexWaveform(1,1,"s0");

NexButton back_1 = NexButton(1,2,"back_1");

NexPage page2 = NexPage(2,0,"page2");

NexButton b0 = NexButton(2,1,"b0");
NexButton b1 = NexButton(2,2,"b1");
NexButton b2 = NexButton(2,3,"b2");

NexPage page3 = NexPage(3,0,"page3");

NexButton back_3 = NexButton(3,1,"back_3");
NexButton save_3 = NexButton(3,14,"save_3");

NexSlider hrmin = NexSlider(3, 2, "h0");
NexSlider hrmax = NexSlider(3, 3, "h1");
NexSlider spo2min = NexSlider(3, 4, "h2");

NexText t0_3 = NexText(3,5,"t0");
NexText t1_3 = NexText(3,6,"t1");
NexText t2_3 = NexText(3,11,"t2");
NexText t3_3 = NexText(3,7,"t3");
NexText t4_3 = NexText(3,12,"t4");
NexText t5_3 = NexText(3,13,"t5");

NexNumber n0_3 = NexNumber(3,8,"n0");
NexNumber n1_3 = NexNumber(3,9,"n1");
NexNumber n2_3 = NexNumber(3,10,"n2");


NexTouch *nex_listen_list[] = {
  &b0, &b1, &b2, &back_0, &back_1, &back_3,
  &hrmin, &hrmax, &spo2min, &save_3, NULL
};


// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    //Serial.println("Beat!");
    digitalWrite(ledPin, HIGH);
    delay(1);
    digitalWrite(ledPin, LOW);
}

void dispararAlarme(){
  return;
}

void setup()
{
    nexInit();
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    back_0.attachPush(back_0_Press, &back_0);
    back_1.attachPush(back_1_Press, &back_1);
    back_3.attachPush(back_3_Press, &back_3);
    save_3.attachPush(save_3_Press, &save_3);

    b0.attachPush(b0_Press, &b0);
    b1.attachPush(b1_Press, &b1);
    b2.attachPush(b2_Press, &b2);

    hrmin.attachPop(hrminPopCallback, &hrmin);
    hrmax.attachPop(hrmaxPopCallback, &hrmax);
    spo2min.attachPop(spo2minPopCallback, &spo2min);

    page2.show();

    Serial.begin(115200);
    Serial.println("");
    Serial.print("Initializing pulse oximeter..");

    // Iniciando dados brutos
    if (!sensor.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    // Iniciando oximetro spo2 e hr
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);

    // Set up the wanted parameters
    sensor.setMode(MAX30100_MODE_SPO2_HR);
    sensor.setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT);
    sensor.setLedsPulseWidth(PULSE_WIDTH);
    sensor.setSamplingRate(SAMPLING_RATE);
    sensor.setHighresModeEnabled(HIGHRES_MODE);
}

void loop()
{
    nexLoop(nex_listen_list);
    pox.update();
    if(page1_Flag){
        sensor.update();
    }
    if ((millis() - tsLastReport > REPORTING_PERIOD_MS && page0_Flag)){
        page0_main();
    }
    else if(page1_Flag){
        page1_main();
    }
    else if(page3_Flag){
        page3_main();
    }
}
