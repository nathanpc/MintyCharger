/*
 * File:   main.c
 * Author: nathanpc
 *
 * Created on April 30, 2015, 6:51 PM
 */


#include <xc.h>
#include <pic12f683.h>
#include <stdint.h>

#pragma config MCLRE = OFF, CP = OFF, CPD = OFF, BOREN = OFF, WDTE = OFF
#pragma config PWRTE = OFF, FOSC = INTOSCCLK

// TODO: Implement a calibration mode to fix the imperfections in the feedback
//       resistors. Enter the mode by shorting the battery terminals (ADC will
//       read 0), then flash the LEDs in a pattern so the user can unshort,
//       then read the voltages.

#define _XTAL_FREQ 4000000
#define HIGH 1
#define LOW  0
#define DEADCELL 1
#define _8V4 0
#define _9V6 1

void set_voltage(uint8_t type);

void main() {
    uint8_t sGPIO = 0;
    uint8_t sGP3 = 0;

    // Setup the pins.
    TRISIO = 0b111011;
    ANSEL = 0;

    // Initialize the PWM module.
    T2CONbits.T2CKPS = 0b00;     // Timer2 Prescaler = 1.
    T2CONbits.TMR2ON = 1;        // Enable Timer2.
                                 // Timer2 will increment every 1us with 4MHz.
    PR2 = 49;                    // Period = 50 * 1us = 50us or 20kHz.
    CCP1CONbits.CCP1M = 0b1100;  // Single output active high PWM.
    sGP3 = GPIObits.GP3;
    set_voltage(sGP3);

    while (1) {
        if (GPIObits.GP3 != sGP3) {
            sGP3 = GPIObits.GP3;
            set_voltage(sGP3);
        }
    }
}

void set_voltage(uint8_t type) {
    if (type == _8V4) {
        // 75%
        CCP1CONbits.DC1B  = 0b11;
        CCPR1L = 0b100101;
    } else {
        // 86%
        CCP1CONbits.DC1B  = 0b10;
        CCPR1L = 0b101011;
    }
}

void battery_indicator(unsigned int voltage) {
    // TODO: Make LEDs blink as different rates depending on the percentage
    //       at that point.
}
