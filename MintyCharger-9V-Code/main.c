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
#pragma config PWRTE = OFF, FOSC = INTOSCIO

#define _XTAL_FREQ 4000000
#define HIGH 1
#define LOW  0
#define DEADCELL 1
#define _8V4 0
#define _9V6 1

void set_voltage(uint8_t type);
void battery_indicator(unsigned int voltage);

void main() {
    TRISIO = 0b000000;
    ANSEL = 0;

    uint8_t sGPIO = 0;

    // Setup the pins.
    TRISIO = 0b001001;

    // Setup ADC.
    ANSELbits.ADCS = 0b001;  // Fosc / 8 = 2.0us (4MHz clock).
    ANSELbits.ANS = 0b0001;  // Enable AN0.
    ADCON0bits.ADFM = 1;     // LSB of result in ADRESH<7>
    ADCON0bits.VCFG = 0;     // Voltage reference set to Vdd.
    ADCON0bits.CHS = 0b00;   // Select channel AN0.
    ADCON0bits.ADON = 1;     // Turn ADC ON.

    // Initialize the PWM module.
    T2CONbits.T2CKPS = 0b00;     // Timer2 Prescaler = 1.
    T2CONbits.TMR2ON = 1;        // Enable Timer2.
                                 // Timer2 will increment every 1us with 4MHz.
    PR2 = 49;                    // Period = 50 * 1us = 50us or 20kHz.
    CCP1CONbits.CCP1M = 0b1100;  // Single output active high PWM.
    set_voltage(GPIObits.GP3);

    unsigned int res = 0;
    unsigned int res2 = 0;

    while (1) {
        // Check if the switch changed.
        /*if (GPIObits.GP3 != sGP3) {
            sGP3 = GPIObits.GP3;
            set_voltage(sGP3);
        }*/

        __delay_us(10);  // Acquisition delay.
        ADCON0bits.GO = 1;
        while (ADCON0bits.nDONE)
            ;
        res = (ADRESH << 8) | ADRESL;
        res2 = res / 5;//(res - 70) / 2;
        CCP1CONbits.DC1B = res2 & 0b0000000011;
        CCPR1L = res2 >> 2;

        battery_indicator(res);
    }
}

/**
 * Sets the PWM duty cycle according to the voltage selection switch.
 *
 * @param type Voltage selection switch position.
 */
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

void calibrate() {
    // TODO: Implement a calibration mode to fix the imperfections in the feedback
    //       resistors. Enter the mode by shorting the battery terminals (ADC will
    //       read 0), then flash the LEDs in a pattern so the user can unshort,
    //       then read the voltages.
}

void battery_indicator(unsigned int voltage) {
    // Vbatt < 8.2V
    if (voltage < 689) {
        if (voltage <= 614) {
            // Vbatt < 7.3V
            GPIO = 0b000010;
            __delay_ms(600);
            GPIO = 0b000000;
            __delay_ms(600);
        } else if (voltage <= 639) {
            // Vbatt < 7.6V
            GPIO = 0b000010;
            __delay_ms(400);
            GPIO = 0b000000;
            __delay_ms(400);
        } else if (voltage <= 664) {
            // Vbatt < 7.9V
            GPIO = 0b000010;
            __delay_ms(200);
            GPIO = 0b000000;
            __delay_ms(200);
        } else {
            // Vbatt < 8.2V
            GPIO = 0b000010;
            __delay_ms(100);
            GPIO = 0b000000;
            __delay_ms(100);
        }
    }

    // Vbatt < 9.4V
    if (voltage < 790 && voltage > 689) {
        if (voltage <= 715) {
            // Vbatt < 8.5V
            GPIO = 0b000010;
            __delay_ms(600);
            GPIO = 0b000010;
            __delay_ms(600);
        } else if (voltage <= 740) {
            // Vbatt < 8.8V
            GPIO = 0b010010;
            __delay_ms(400);
            GPIO = 0b000010;
            __delay_ms(400);
        } else if (voltage <= 765) {
            // Vbatt < 9.1V
            GPIO = 0b010010;
            __delay_ms(200);
            GPIO = 0b000010;
            __delay_ms(200);
        } else {
            // Vbatt < 9.4V
            GPIO = 0b010010;
            __delay_ms(100);
            GPIO = 0b000010;
            __delay_ms(100);
        }
    }

    // Vbatt < 10.5V
    if (voltage > 790) {
        if (voltage <= 816) {
            // Vbatt < 9.7V
            GPIO = 0b110010;
            __delay_ms(600);
            GPIO = 0b010010;
            __delay_ms(600);
        } else if (voltage <= 841) {
            // Vbatt < 10.0V
            GPIO = 0b110010;
            __delay_ms(400);
            GPIO = 0b010010;
            __delay_ms(400);
        } else if (voltage <= 866) {
            // Vbatt < 10.3V
            GPIO = 0b110010;
            __delay_ms(200);
            GPIO = 0b010010;
            __delay_ms(200);
        } else if (voltage <= 880) {
            // Vbatt < 10.5V
            GPIO = 0b110010;
            __delay_ms(100);
            GPIO = 0b010010;
            __delay_ms(100);
        } else {
            GPIO = 0b110010;
        }
    }

    // TODO: Implement 9.6V battery.
}
