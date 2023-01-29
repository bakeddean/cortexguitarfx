#include "audio/reverb2.h"
#include "audio/delay.h"


//const uint16_t ap_delays[]={431,433,439,443,449,457,461,463};
//const uint32_t delay_lengths[]={537,683,827,971};

const uint16_t ap_delays[]={107,127,149,283,71,241,311,199};
const uint32_t delay_lengths[]={1523,1847,2131,2447};

void initReverb2(Reverb2Type*reverbData)
{
    int16_t * delayMemoryPointer = getDelayMemoryPointer();
    for (uint8_t c=0;c<8;c++)
    {
        reverbData->aps[c].delayLine = delayMemoryPointer + c*512;
        reverbData->aps[c].coefficient = 16383;
        reverbData->aps[c].delayPtr = 0;
        reverbData->aps[c].oldValues = 0;
        reverbData->aps[c].delayInSamples=ap_delays[c];
        reverbData->aps[c].bufferSize = 0x1FF;
        reverbData->outs[c>>1]=0;
    }
    for (uint8_t c=0;c<4;c++)
    {
        initDelay(reverbData->delaylines+c,delayMemoryPointer + 8*512 +c*4096,4096);
        (reverbData->delaylines+c)->delayInSamples = delay_lengths[c];
    }
    reverbData->mix=0;
}

int16_t reverb2ProcessSample(int16_t sampleIn,Reverb2Type*reverbData)
{
    int16_t sampleOut;
    int16_t reverbSignal=0;
    int32_t processSignal = (sampleIn >> 1) + ((reverbData->decay*reverbData->outs[3])>> 16);
    if (processSignal > 32767)
    {
        processSignal = 32767;
    }
    else if (processSignal < -32768)
    {
        processSignal = -32768;
    }
    processSignal = allpassProcessSample(processSignal,&reverbData->aps[0]);
    processSignal = allpassProcessSample(processSignal,&reverbData->aps[1]);
    addSampleToDelayline(processSignal,reverbData->delaylines+0);
    reverbData->outs[0] = getDelayedSample(reverbData->delaylines+0);
    reverbSignal +=  reverbData->outs[0] >> 2;

    processSignal = (sampleIn >> 1) + ((reverbData->decay*reverbData->outs[0])>> 16);
    if (processSignal > 32767)
    {
        processSignal = 32767;
    }
    else if (processSignal < -32768)
    {
        processSignal = -32768;
    }
    processSignal = allpassProcessSample(processSignal,&reverbData->aps[2]);
    processSignal = allpassProcessSample(processSignal,&reverbData->aps[3]);
    addSampleToDelayline(processSignal,reverbData->delaylines+1);
    reverbData->outs[1] = getDelayedSample(reverbData->delaylines+1);
    reverbSignal += reverbData->outs[1] >> 2;

    processSignal = (sampleIn >> 1) + ((reverbData->decay*reverbData->outs[1])>> 16);
    if (processSignal > 32767)
    {
        processSignal = 32767;
    }
    else if (processSignal < -32768)
    {
        processSignal = -32768;
    }
    processSignal = allpassProcessSample(processSignal,&reverbData->aps[4]);
    processSignal = allpassProcessSample(processSignal,&reverbData->aps[5]);
    addSampleToDelayline(processSignal,reverbData->delaylines+2);
    reverbData->outs[2] = getDelayedSample(reverbData->delaylines+2);
    reverbSignal += reverbData->outs[2] >> 2;

    processSignal = (sampleIn >> 1) + ((reverbData->decay*reverbData->outs[2])>> 16);
    if (processSignal > 32767)
    {
        processSignal = 32767;
    }
    else if (processSignal < -32768)
    {
        processSignal = -32768;
    }
    processSignal = allpassProcessSample(processSignal,&reverbData->aps[6]);
    processSignal = allpassProcessSample(processSignal,&reverbData->aps[7]);
    addSampleToDelayline(processSignal,reverbData->delaylines+3);
    reverbData->outs[3] = getDelayedSample(reverbData->delaylines+3);
    reverbSignal += reverbData->outs[3] >> 2;  

    sampleOut = ((((1 << 15) - reverbData->mix)*sampleIn) >> 15) + ((reverbData->mix*reverbSignal) >> 15);
    return sampleOut;  
}