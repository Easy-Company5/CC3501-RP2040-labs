#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "drivers/LABS/Switch.h"
#include "drivers/LABS/Microphone.h"
#include "drivers/LABS/LED.h"
#include "drivers/LABS/Misc.h"
#include "drivers/LABS/LIS3DH.h"
#include "drivers/LABS/Init.h"

// Function declarations
void Case1();  // Microphone FFT Processing
void Case2();  // Accelerometer Tilt Detection
void Case3();
void Case4();

int main() {
    Initialise();
// Infinite loop to handle case switching based on the switch state
    while (true) {
        int switchState = get_switch_state();  // Get the current state of the switch

        // Use a switch statement to handle different cases
        switch (switchState) {
            case 0:
                
                Case1();  // Perform Case 1 operations
                break;

            case 1:
                Case2();  // Perform Case 2 operations
                break;
            case 2:
                Case3();
                break;
            case 3:
                Case4();
                break;
            default:
                clearScreen();
                printf("Invalid switch state: %d\n", switchState);
                break;
        }

    }

    return 0;
}
