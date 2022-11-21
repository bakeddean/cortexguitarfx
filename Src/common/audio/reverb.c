#include "audio/reverb.h"
#include "audio/delay.h"

/**
 * @brief feedback interpolation values for
 * delay lines of 31, 67, 101, 113 samples
 * and tau values (time when signal is at 0.001) of 0.1, 0.73333333,1.36666667,2.        
 * 
 */
/*
const float feedback[4][4] = {
{
0.9563679f, 0.9939349f, 0.9967410f, 0.9977719f, 
},{
0.9080819f, 0.9869378f, 0.9929697f, 0.9951906f, 
},{
0.8647190f, 0.9803746f, 0.9894209f, 0.9927588f, 
},{
0.8499140f, 0.9780686f, 0.9881715f, 0.9919020f, 
}};*/
const float feedback[4][4] = {
{
0.0553509f, 0.6739194f, 0.8091584f, 0.8652792f, 
},{
0.0254463f, 0.6061571f, 0.7644312f, 0.8323026f, 
},{
0.0105469f, 0.5375590f, 0.7167218f, 0.7964458f, 
},{
0.0027661f, 0.4478832f, 0.6498605f, 0.7448928f, 
},};

const float taus[4] = {0.1, 0.733333333f,1.366666666f,2.0f}; 
const uint16_t delayInSamples[4] = {2011, 2551, 3163, 4093};//{31,67,101,113};
const uint16_t allpassDelays[3] = {1021,337,115};
const float phaseshifts[4]= {0.7f,0.7f,0.7f,0.7f};

float getFeedback(uint8_t delayLineIndex,float tau)
{
    float feedbackVal=0.0f;
    for(uint8_t c=0;c<3;c++)
    {
        if (taus[c] < tau && taus[c+1] > tau)
        {
            feedbackVal = feedback[delayLineIndex][c] + (feedback[delayLineIndex][c+1]-feedback[delayLineIndex][c])*(tau - taus[c])/(taus[c+1]-taus[c]);
        } 
    }
    return feedbackVal;
}

void setReverbTime(float reverbTime,ReverbType*reverbData)
{
    for(uint8_t c=0;c<4;c++)
    {
        reverbData->feedbackValues[c]=getFeedback(c,reverbTime);
    }
}

float allpassProcessSample(float sampleIn,AllpassType*allpass)
{
    float sampleOut;
    DelayDataType * delaySingleton = getDelayData();
    sampleOut = allpass->coefficient*sampleIn + *(allpass->delayLine + ((allpass->delayPtr - allpass->delayInSamples) & 0x3FF));
   // allpass->oldValues
    *(allpass->delayLine + allpass->delayPtr) = sampleIn - allpass->coefficient*sampleOut;
    allpass->delayPtr++;
    allpass->delayPtr &= 0x3FF;
    return sampleOut;
}

void initReverb(ReverbType*reverbData)
{
    DelayDataType * delaySingleton = getDelayData();
    initDelay(delaySingleton);
    for(uint8_t c=0;c<3;c++)
    {
        reverbData->allpasses[c].delayLine = (float*)(((float*)delaySingleton->delayLine)+8192+c*1024); 
        reverbData->allpasses[c].oldValues=0.0f;
        reverbData->allpasses[c].coefficient=phaseshifts[c];
        reverbData->allpasses[c].delayPtr=0;
        reverbData->allpasses[c].delayInSamples=allpassDelays[c];
    }
    reverbData->delayTime=0.3;
    setReverbTime(reverbData->delayTime,reverbData);
    reverbData->delayPointer=0;
}

float reverbProcessSample(float sampleIn,ReverbType*reverbData)
{
    float sampleOut;
    float reverbSignal;

    DelayDataType * delaySingleton = getDelayData();

    reverbSignal = 0.0f;

    
    for (uint8_t c=0;c<4;c++)
    {
        reverbSignal += delaySingleton->delayLine[(reverbData->delayPointer-delayInSamples[c]) & 0x1FFF]/4.0f;
    }

    for (uint8_t rc=0;rc < 4;rc++)
    {
        delaySingleton->delayLine[reverbData->delayPointer & 0x1FFF] = sampleIn + delaySingleton->delayLine[(reverbData->delayPointer-delayInSamples[rc]) & 0x1FFF]*(reverbData->feedbackValues[rc]);
    }
    reverbData->delayPointer++;
    

    for (uint8_t c=0;c<3;c++)
    {
        reverbSignal = allpassProcessSample(reverbSignal,reverbData->allpasses+c);
    }

    sampleOut = (1.0f-reverbData->mix)*sampleIn + reverbData->mix*reverbSignal;
    return sampleOut;
}
