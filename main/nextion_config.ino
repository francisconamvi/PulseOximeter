#include "nextion_config.h"
#include <Wire.h>

void back_0_Press(void *ptr) {
  page2.show();
  page0_Flag = 0;
}

void back_1_Press(void *ptr) {
  page2.show();
  page1_Flag = 0;
}

void back_3_Press(void *ptr) {
  page2.show();
  page3_Flag = 0;
}

void save_3_Press(void *ptr) {
  //TODO: verificar se hrmin é menor que hrmax
  //se sim, salva
  //se nao, da erro 
  n0_3.getValue(&value_hr_min);
  n1_3.getValue(&value_hr_max);
  n2_3.getValue(&value_spo2_min);

}

void b0_Press(void *ptr) {
  page1.show();
  page1_Flag = 1;
}

void b1_Press(void *ptr) {
  page0.show();
  page0_Flag = 1;
}

void b2_Press(void *ptr) {
  page3.show();
  n0_3.setValue(value_hr_min);
  n1_3.setValue(value_hr_max);
  n2_3.setValue(value_spo2_min);
  hrmin.setValue(value_hr_min);
  hrmax.setValue(value_hr_max);
  spo2min.setValue(value_spo2_min);
  page3_Flag = 1;
}

void hrminPopCallback(void *ptr)
{
    uint32_t number = 0;
    hrmin.getValue(&number);
    n0_3.setValue(number);
}

void hrmaxPopCallback(void *ptr)
{
    uint32_t number = 0;
    hrmax.getValue(&number);
    n1_3.setValue(number);
}

void spo2minPopCallback(void *ptr)
{
    uint32_t number = 0;
    spo2min.getValue(&number);
    n2_3.setValue(number);
}