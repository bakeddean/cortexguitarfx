
// ***************
// * pin numbers *
// ***************

// A  B  C  D  E  F  G  H  
// 0  1  2  3  4  5  6  7 
#define ENCODER_1 (1*16+4) //PB4 since TIM3 with channel 1 and channel 2 is used
#define ENCODER_2 (1*16+5) //PB5 since TIM3 with channel 1 and channel 2 is used
#define ENTER_SWITCH (3*16+14) // PD14
#define EXIT_SWITCH (3*16+15) // PD15

// adc pins used by the three potentiometer, refer to the datasheet for a list of possible options
// 
#define POT1 (2*16+0) // PC0 
#define POT2 (0*16+3) // PA3
#define POT3 (1*16+1) // PB1



// i2s interface
#define I2S_BCLK (4*16+5) // PE5  SCK
#define I2S_LRCLK (4*16+4) // PE4 LRCLK
#define I2S_DOUT (4*16+3) // PC1 sai 1: transmitter
#define I2S_DIN (4*16+6) // PE6 sai 1: receiver
#define I2S_MCLK (4*16+2) // PE2 MCLK
//#define I2S_BCLK2 (1*16+10) // PB10 I2S2 SCK
//#define I2S_LRCLK2 (1*16+12) // PB12 I2S2 LRCLK
// i2c interface
#define I2C_SDA (1*16+11) // PB11
#define I2C_SCL (7*16+4) // PH4

//cs4270 reset line
#define CS4270_RESET (0*16+0)

//ssd1306 display interfaced using spi
#define SSD1306_SCK (2*16+10) // PC10
#define SSD1306_MOSI (1*16+2) // PB2
#define SSD1306_MISO (2*16+11) // PC11
#define SSD1306_CD (0*16+6) // PA6
#define SSD1306_RESET (2*16+6) //PC6
#define SSD1306_CS (2*16+8) //PC8



// ***************************************
// * additional device-specific settings *
// ***************************************

#define POT1_CHANNEL 10
#define POT2_CHANNEL 15
#define POT3_CHANNEL 5
