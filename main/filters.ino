int detrend(int sample){
    int detrended = 0;
    mFilteredSamples[mFilteredSamples_n] = sample;
    if(mFilteredSamples_size < 100){
        mFilteredSamples_size++;
        return detrended;
    }
    for(int i=0; i<100; i++){
        detrended += mFilteredSamples[i];
    }
    mFilteredSamples_n++;
    mFilteredSamples_n = mFilteredSamples_n%100;
    detrended = detrended/100;
    return abs(sample - detrended);
}

int red_detrend(int sample){
    int detrended = 0;
    red_mFilteredSamples[mFilteredSamples_n] = sample;
    if(red_mFilteredSamples_size < 100){
        red_mFilteredSamples_size++;
        return detrended;
    }
    for(int i=0; i<100; i++){
        detrended += red_mFilteredSamples[i];
    }
    red_mFilteredSamples_n++;
    red_mFilteredSamples_n = red_mFilteredSamples_n%100;
    detrended = detrended/100;
    return abs(sample - detrended);
}

int iIRFilter(int sample){
    int filtered = 0;
    mSamples[mSamples_n] = sample;
    mSamples_size++;
    if(mSamples_size < 31){
        mSamples_n++;
        return filtered;
    }
    for(int i = 0; i <31; i++){
        filtered += mSamples[i];
    }
    mSamples_n = mSamples_n%31;
    mSamples_size--;
    return filtered/31;
}

int iREDFilter(int sample){
    int filtered = 0;
    red_mSamples[red_mSamples_n] = sample;
    if(red_mSamples_size < 31){
        red_mSamples_size++;
        return filtered;
    }
    for(int i = 0; i <31; i++){
        filtered += red_mSamples[i];
    }
    red_mSamples_n++;
    red_mSamples_n = red_mSamples_n%30;
    return filtered/31;
}