#ifndef filters_h
#define filters_h

int detrend(int sample);
int red_detrend(int sample);
int iIRFilter(int sample);
int iREDFilter(int sample);

#endif