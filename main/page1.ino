#include "nextion_config.h"
#include <Wire.h>

void page1_main(){
    while(sensor.getRawValues(&ir, &red) && page1_Flag){
        /*
        int value_ir = ir;
        int value_red = red;
        
        //TODO: melhorar isso depois
        Serial.print(iIRFilter(detrend(value_ir)));
        Serial.print("\t");
        Serial.print(value_ir);
        Serial.print("\t");
        Serial.print(iREDFilter(red_detrend(value_red)));
        Serial.print("\t");
        Serial.print(value_red);
        Serial.println("");
        */

        
        int value = ir;
        //Serial.print(value);
        //Serial.print("\t");
        //Serial.print(detrend(value));
        //Serial.print("\t");
        Serial.println(30*iIRFilter(detrend(value)));
        /*
        if(value>1000){
            value = iIRFilter(detrend(value));
            if(value<500){
                value = map(value, 0, 500, 0, 700);
                //Serial.print("Valor filtrado: ");
                Serial.println(value);
                //s0.addValue(0,value);
            }
            else{
                Serial.println(700);
                //s0.addValue(0,700);
            }
        }
        else{
            Serial.println(0);
            //s0.addValue(0,0);
        }
        */
        
        /*
        int value_ir = ir;
        int value_red = red;
        Serial.print("red: ");
        Serial.println(value_red);
        Serial.print("ir: ");
        Serial.println(value_ir);
        float value = log((float)value_red)/(float)log(value_ir);
        Serial.println(value);
        */
        

    }
    return;
}