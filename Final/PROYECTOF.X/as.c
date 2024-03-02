/*
 * File:   main_HC_SR04_PIR_33FJ.c
 * Author: JHON ORE
 * Created on 19 de febrero de 2024, 11:27 PM
 */

#include "xc.h"
#include <libpic30.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//#include "Headers_Ultrasonido.h"

//FPOR
#pragma config PWMPIN = ON               
#pragma config HPOL = ON      
#pragma config LPOL = ON               
#pragma config ALTI2C = OFF
#pragma config FPWRT = PWR128               

// FWDT
#pragma config WDTPOST = PS32        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FOSC
#pragma config POSCMD = HS              // Primary Oscillator Mode Select bits (HS Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = ON            // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)

// FOSCSEL
#pragma config FNOSC = PRIPLL           // Oscillator Source Selection (Primary Oscillator with PLL module (XT + PLL, HS + PLL, EC + PLL))
#pragma config IESO = OFF                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)


#define TRIG _LATA4
#define ECHO _RB4        

/***************************************************************************/
#define MIPS 40     //MIPS deseados (maximo 40 para el dspic33fj32mc204)
#define crystal 20 //valor del cristal en MHZ       
#define FCY MIPS*1000000
// Definciones para calculo de Baud Rate.-
#define BAUDRATE 9600
#define BRGHigh 1
#define BRGVAL ((FCY/BAUDRATE)/16)-1
#define NUM_CHS2SCAN   2		// Number of channels enabled for channel scan
#define delay_ms(x) __delay32((FCY/1000)*x) //delay en milisegundos
#define delay_us(x) __delay32(MIPS*x) //delay en microsegundos
void configuration_uart(void);
void Configuracion_timer1(void);
void config_clk_HSPLL(void);
void configuracion_INT(void);

void config_clk_HSPLL(void) {//con oscilador externo
    int M = MIPS * 8 / crystal;
    PLLFBD = M - 2; // M = 28
    CLKDIVbits.PLLPRE = 0; // N1 = 2
    CLKDIVbits.PLLPOST = 0; // N2 = 2
    while (_LOCK == 0);
}

void configuration_uart(void) {//maestro
    //Configurar  COMO TRASNMISION RP22
    RPOR11bits.RP22R = 3;
    //Configurar  COMO RECEPCION RP23
    RPINR18bits.U1RXR = 23; /// RP23
    ////******Configuracion del modulo uart
    U1MODEbits.STSEL = 0; // 1 Stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8 data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud Disabled
    U1MODEbits.BRGH = 0; // Low-Speed mode
    U1BRG = BRGVAL; // BAUD Rate Setting for 9600
    U1STAbits.URXISEL = 0; // Interrupt after one RX character is received
    IFS0bits.U1RXIF = 0; // Clear the Recieve Interrupt Flag
    IEC0bits.U1RXIE = 1; // Enable Recieve Interrupts
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX
}

void Configuracion_timer1(void) {
    //Habilito interrupcion del timer y lo coloco como prioridad 1

    _T1IF = 0x00; //Limpio el bit de interrupcion
    _T1IP = 0x01; //Asigno prioridad a la interrupcion
    _T1IE = 0x01; //Habilitacion de la interrupcion

    TMR1 = 0x00; //Reseteo al conteo del timer
    PR1 = 0xFFF0; //Asigno el periodo del timer
    //T1CON = 0xFFA9;     //Inicio el timer //Preescaler 64
    //Total maximo contar 419.328ms
    //Fcy=40Mhz=>6.4us -- 1---
    //			419.328ms	65520 este es el tiempo de desborde aprox maximo del timer.

    //T1CON = 0xFF30;     //Inicio el timer //Preescaler 256
    T1CONbits.TCKPS = 3; //preescaler al clock en (3:256)(1:8)(0:1)(2:64)
    T1CONbits.TON = 0; //aun no Inicio timer1
}

void configuracion_INT(void) {
    _INT1IF = 0;
    RPINR0bits.INT1R = 0x04; //Configuro interrupcion externa 1 RP4
    INTCON2bits.INT1EP = 0; //Configuro como flanco de subida
    _INT1IE = 1; //activo la interrupcion

}
/****************************************************************************/
void StartSignal();

double tiempo = 0;
float Distancia_Raw = 0.0;
float Distancia = 0.0;
char Recep;

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void) {

    Recep = U1RXREG;
    U1TXREG = Recep;
    _U1RXIF = 0; // Borramos flag.
}

void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void) {

    if (ECHO == 1) {
        INTCON2bits.INT1EP = 1; //Configuro como flanco de bajada
        T1CONbits.TON = 1; //activo conteo de timer
        TMR1 = 0x00;
    }
    if (ECHO == 0) {//
        tiempo = TMR1; //calcula el tiempo del timer
        tiempo = 6 * TMR1;
        TMR1 = 0x00;
        T1CONbits.TON = 0; //Desactivo timer
        INTCON2bits.INT1EP = 0; //Configuro como flanco de subida
        _INT1IE = 1; //activo interrupcion
        Distancia_Raw = tiempo / 58.0; //para convertir en cm segun datasheet
        /******************************************************/

        if (Distancia_Raw >= 2.0 && Distancia_Raw < 2000.0) {

            // printf("%.2f cm\n",distancia);
            Distancia = Distancia_Raw;
        } else {
            ////            printf("fuera de rango\n");
            RCONbits.SWDTEN = 1; // activar WATCH DOG (el tiempo se configura en los fuses WDTPOST_PS32)  

        }
    }
    _INT1IF = 0;
}

int main(void) {
    AD1PCFGL = 0xFFFF;
    config_clk_HSPLL();
    /*********Pines del HC-SR04********/
    _TRISA4 = 0; //TRIG
    _TRISB4 = 1; //ECHO
    /*********Pines del HC-SR04********/
    configuration_uart(); // Configuracion comunicacion serial
    configuracion_INT(); //sensor ultrasonico
    Configuracion_timer1(); //sensor ultrasonico
    _TRISB9 = 1; //entrada del PIR
    _TRISC7 = 0; // salida del led
    _TRISC8 = 0; //salida del led
    _LATC7 = 0;
    _LATC8 = 0;
    while (1) {
        /******************sensor ultrasonico************************/
        TRIG = 1;
        delay_us(10);
        TRIG = 0;
        /*************++++++**** PIR +++****************************/
        if (PORTBbits.RB9 == 1) {
            _LATC7 = 1;
            _LATC8 = 0;
            printf("%.2f\n\r", (double) Distancia);
        } else {
            _LATC7 = 0;
            _LATC8 = 1;
        }
        /**************************************************************/

        delay_ms(100);
    }
    return 0;
}
