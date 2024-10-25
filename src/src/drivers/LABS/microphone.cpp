//Switch.cpp
#include "microphone.h"
#include "hardware/adc.h"
#include "arm_math.h"  // Include ARM math library for q15_t type
#include <stdio.h>
#include "pico/stdlib.h"


// Define FFT size and other constants
#define FFT_SIZE 1024  // Number of samples to capture
#define FFT_OUTPUT_SIZE (FFT_SIZE / 2)  // FFT output size is half of the FFT size

// Create an instance of the RFFT structure
static arm_rfft_instance_q15 rfft_instance;

// You can add your own Hanning window values here.
static const q15_t hanning_window[FFT_SIZE] = {
0,0,1,3,5,8,11,15,20,25,31,37,44,52,60,69,79,89,100,111,123,136,149,163,177,192,208,224,241,259,277,296,315,335,355,376,398,420,443,467,491,516,541,567,593,621,648,677,705,735,765,796,827,859,891,924,958,992,1027,1062,1098,1134,1171,1209,1247,1286,1325,1365,1406,1447,1488,1530,1573,1616,1660,1704,1749,1795,1841,1887,1935,1982,2030,2079,2128,2178,2229,2280,2331,2383,2435,2488,2542,2596,2651,2706,2761,2817,2874,2931,2989,3047,3105,3165,3224,3284,3345,3406,3468,3530,3592,3655,3719,3783,3847,3912,3978,4044,4110,4177,4244,4312,4380,4449,4518,4587,4657,4728,4799,4870,4942,5014,5087,5160,5233,5307,5381,5456,5531,5606,5682,5759,5835,5913,5990,6068,6146,6225,6304,6383,6463,6543,6624,6705,6786,6868,6950,7032,7115,7198,7282,7365,7449,7534,7619,7704,7789,7875,7961,8047,8134,8221,8308,8396,8484,8572,8661,8749,8839,8928,9018,9108,9198,9288,9379,9470,9561,9653,9745,9837,9929,10021,10114,10207,10300,10394,10487,10581,10676,10770,10864,10959,11054,11149,11245,11340,11436,11532,11628,11724,11821,11917,12014,12111,12208,12306,12403,12501,12598,12696,12794,12892,12991,13089,13188,13286,13385,13484,13583,13682,13781,13881,13980,14079,14179,14279,14378,14478,14578,14678,14778,14878,14978,15078,15179,15279,15379,15480,15580,15680,15781,15881,15982,16082,16183,16283,16384,16485,16585,16686,16786,16887,16987,17088,17188,17288,17389,17489,17589,17690,17790,17890,17990,18090,18190,18290,18390,18489,18589,18689,18788,18887,18987,19086,19185,19284,19383,19482,19580,19679,19777,19876,19974,20072,20170,20267,20365,20462,20560,20657,20754,20851,20947,21044,21140,21236,21332,21428,21523,21619,21714,21809,21904,21998,22092,22187,22281,22374,22468,22561,22654,22747,22839,22931,23023,23115,23207,23298,23389,23480,23570,23660,23750,23840,23929,24019,24107,24196,24284,24372,24460,24547,24634,24721,24807,24893,24979,25064,25149,25234,25319,25403,25486,25570,25653,25736,25818,25900,25982,26063,26144,26225,26305,26385,26464,26543,26622,26700,26778,26855,26933,27009,27086,27162,27237,27312,27387,27461,27535,27608,27681,27754,27826,27898,27969,28040,28111,28181,28250,28319,28388,28456,28524,28591,28658,28724,28790,28856,28921,28985,29049,29113,29176,29238,29300,29362,29423,29484,29544,29603,29663,29721,29779,29837,29894,29951,30007,30062,30117,30172,30226,30280,30333,30385,30437,30488,30539,30590,30640,30689,30738,30786,30833,30881,30927,30973,31019,31064,31108,31152,31195,31238,31280,31321,31362,31403,31443,31482,31521,31559,31597,31634,31670,31706,31741,31776,31810,31844,31877,31909,31941,31972,32003,32033,32063,32091,32120,32147,32175,32201,32227,32252,32277,32301,32325,32348,32370,32392,32413,32433,32453,32472,32491,32509,32527,32544,32560,32576,32591,32605,32619,32632,32645,32657,32668,32679,32689,32699,32708,32716,32724,32731,32737,32743,32748,32753,32757,32760,32763,32765,32767,32767,32767,32767,32767,32765,32763,32760,32757,32753,32748,32743,32737,32731,32724,32716,32708,32699,32689,32679,32668,32657,32645,32632,32619,32605,32591,32576,32560,32544,32527,32509,32491,32472,32453,32433,32413,32392,32370,32348,32325,32301,32277,32252,32227,32201,32175,32147,32120,32091,32063,32033,32003,31972,31941,31909,31877,31844,31810,31776,31741,31706,31670,31634,31597,31559,31521,31482,31443,31403,31362,31321,31280,31238,31195,31152,31108,31064,31019,30973,30927,30881,30833,30786,30738,30689,30640,30590,30539,30488,30437,30385,30333,30280,30226,30172,30117,30062,30007,29951,29894,29837,29779,29721,29663,29603,29544,29484,29423,29362,29300,29238,29176,29113,29049,28985,28921,28856,28790,28724,28658,28591,28524,28456,28388,28319,28250,28181,28111,28040,27969,27898,27826,27754,27681,27608,27535,27461,27387,27312,27237,27162,27086,27009,26933,26855,26778,26700,26622,26543,26464,26385,26305,26225,26144,26063,25982,25900,25818,25736,25653,25570,25486,25403,25319,25234,25149,25064,24979,24893,24807,24721,24634,24547,24460,24372,24284,24196,24107,24019,23929,23840,23750,23660,23570,23480,23389,23298,23207,23115,23023,22931,22839,22747,22654,22561,22468,22374,22281,22187,22092,21998,21904,21809,21714,21619,21523,21428,21332,21236,21140,21044,20947,20851,20754,20657,20560,20462,20365,20267,20170,20072,19974,19876,19777,19679,19580,19482,19383,19284,19185,19086,18987,18887,18788,18689,18589,18489,18390,18290,18190,18090,17990,17890,17790,17690,17589,17489,17389,17288,17188,17088,16987,16887,16786,16686,16585,16485,16384,16283,16183,16082,15982,15881,15781,15680,15580,15480,15379,15279,15179,15078,14978,14878,14778,14678,14578,14478,14378,14279,14179,14079,13980,13881,13781,13682,13583,13484,13385,13286,13188,13089,12991,12892,12794,12696,12598,12501,12403,12306,12208,12111,12014,11917,11821,11724,11628,11532,11436,11340,11245,11149,11054,10959,10864,10770,10676,10581,10487,10394,10300,10207,10114,10021,9929,9837,9745,9653,9561,9470,9379,9288,9198,9108,9018,8928,8839,8749,8661,8572,8484,8396,8308,8221,8134,8047,7961,7875,7789,7704,7619,7534,7449,7365,7282,7198,7115,7032,6950,6868,6786,6705,6624,6543,6463,6383,6304,6225,6146,6068,5990,5913,5835,5759,5682,5606,5531,5456,5381,5307,5233,5160,5087,5014,4942,4870,4799,4728,4657,4587,4518,4449,4380,4312,4244,4177,4110,4044,3978,3912,3847,3783,3719,3655,3592,3530,3468,3406,3345,3284,3224,3165,3105,3047,2989,2931,2874,2817,2761,2706,2651,2596,2542,2488,2435,2383,2331,2280,2229,2178,2128,2079,2030,1982,1935,1887,1841,1795,1749,1704,1660,1616,1573,1530,1488,1447,1406,1365,1325,1286,1247,1209,1171,1134,1098,1062,1027,992,958,924,891,859,827,796,765,735,705,677,648,621,593,567,541,516,491,467,443,420,398,376,355,335,315,296,277,259,241,224,208,192,177,163,149,136,123,111,100,89,79,69,60,52,44,37,31,25,20,15,11,8,5,3,1,0,};

// Buffer to hold ADC samples
static q15_t sample_buffer[FFT_SIZE];

// FFT result buffer
static q15_t fft_output[FFT_SIZE];


// Method to initialize the microphone, ADC, and FFT instance
void Microphone::init() {
    // Initialize the ADC
    adc_init();

    // Select GPIO pin for ADC input (assuming GPIO26 is used, corresponding to ADC input 0)
    adc_gpio_init(26);
    adc_select_input(0);

    // Enable the ADC FIFO and configure settings
    adc_fifo_setup(
        true,    // Write each completed conversion to the FIFO
        false,   // Disable DMA data request (DREQ)
        1,       // DREQ (Data Request) Level
        false,   // Don't overwrite data in the FIFO if it is full
        false    // FIFO disabled
    );

    // Calculate clkdiv to get a sample rate of 44.1 kHz
    float clkdiv = (48000000.0f / 44100.0f) - 1;
    adc_set_clkdiv(clkdiv);

    // Initialize the RFFT instance
    arm_status status;
    status = arm_rfft_init_q15(&rfft_instance, FFT_SIZE, 0, 1);  // Forward FFT, normal scaling
    if (status != ARM_MATH_SUCCESS) {
        // Handle error if initialization fails
        while (1);  // Stay here if FFT initialization fails
    }
}

q15_t* Microphone::get_fft_output() {
    return fft_output;  // Return the pointer to the FFT output array
}

// Method to read samples from the microphone and store them in the provided buffer
void Microphone::read(uint16_t* buffer, size_t sample_count) {
    // Enable free-running sampling mode
    adc_run(true);

    int32_t dc_bias = 0;  // Variable to hold the sum of ADC values for DC bias calculation

    // Read samples directly into the provided buffer
    for (size_t i = 0; i < sample_count; i++) {
        buffer[i] = adc_fifo_get_blocking();  // Read one sample from the FIFO
        dc_bias += buffer[i];                 // Accumulate for DC bias calculation
    }

    // Calculate the DC bias
    dc_bias = 2024;

    // Subtract the DC bias to zero-center the signal
    for (size_t i = 0; i < sample_count; i++) {
        buffer[i] -= dc_bias;  // Subtract DC bias directly in the provided buffer
    }

    // Turn off the ADC to stop free-running mode
    adc_run(false);

    // Drain the FIFO to discard any remaining samples
    while (adc_fifo_get_level() > 0) {
        adc_fifo_drain();
    }
}


void Microphone::perform_fft() {
    // Read samples into the sample_buffer
    uint16_t adc_samples[FFT_SIZE];
    read(adc_samples, FFT_SIZE);

    // Convert uint16_t samples to Q15 format and apply the Hanning window
    for (int i = 0; i < FFT_SIZE; i++) {
        // Convert the uint16_t sample to Q15 format (scale and center)
        sample_buffer[i] = (q15_t)(((int32_t)adc_samples[i] - 32768) << 1);  // Convert to Q15 by subtracting 32768 and scaling

        // Apply Hanning window and amplify the signal
        sample_buffer[i] = (q15_t)(((int32_t)sample_buffer[i] * hanning_window[i]) >> 15);

        // Additional scaling factor to amplify sensitivity (adjust value as needed)
        // sample_buffer[i] = (q15_t)(sample_buffer[i] * 10);  // Increase sensitivity
    }

    // Perform FFT using the initialized RFFT instance
    arm_rfft_q15(&rfft_instance, sample_buffer, fft_output);
    
    // Shift the fft_output values by 10 bits to the right
    for (int i = 0; i < FFT_OUTPUT_SIZE; i++) {
        fft_output[i] = fft_output[i] << 10;  // Right shift each FFT output element by 10 bits
    }



    // At this point, fft_output contains the real and imaginary parts of the FFT result.
}
