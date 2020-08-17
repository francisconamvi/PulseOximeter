#include "nextion_config.h"
#include <Wire.h>

void page0_main(){
    pox.update();
    heartRate = pox.getHeartRate();
    if(heartRate > value_hr_max){
        //batendo muito rapido
        t6.setText("Coracao acelerado");
        dispararAlarme();
        mudancahr = 1;
    }
    else if(heartRate<value_hr_min && heartRate != 0){
        //batendo muito devagar
        t6.setText("Coracao muito lento");
        dispararAlarme();
        mudancahr = 1;
    }
    else{
        if(mudancahr){
        t6.setText("Analisando");
        mudancahr = 0;
        }
    }

    spo2 = pox.getSpO2();
    if(spo2<value_spo2_min && spo2 != 0){
        //taxa de spo2 muito baixa
        t6.setText("Taxa spo2 muito baixa");
        dispararAlarme();
        mudancaspo2 = 1;
    }
    else{
        if(mudancaspo2){
        t6.setText("Analisando");
        mudancaspo2 = 0;
        }
    }

    if(alt){
        Serial.println(heartRate);
        n1.setValue(heartRate);
        alt=0;
    }
    else{
        if(spo2 < 100){
        Serial.println(spo2);
        n0.setValue(spo2);
        }
        alt=1;
    }
    digitalWrite(ledPin, LOW);
    tsLastReport = millis();
    return;
}