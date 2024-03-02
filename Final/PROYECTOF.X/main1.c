#define _XTAL_FREQ 20000000
#include <xc.h>
#include <libpic30.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "i2c.h"                        // Libreria del protocolo I2C
#include "lcd.h"                    // Libreria de la pantalla LCD con modulo I2C

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
#pragma config IESO = OFF

#define TRIG _LATA4https://chat.openai.com/c/d982e0ae-1ad5-4bda-b975-bc7095325d46
#define ECHO _RB4

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

//INTERRUPCIONES
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
int main()
{
    AD1PCFGL = 0xFFFF;             // Coloca todos los pines como digitales
    config_clk_HSPLL();
    
    _TRISA4 = 0; //TRIG    TRISAbits.TRISA4
    _TRISB4 = 1; //ECHO    TRISBbits.TRISB4
    I2C_Init_Master(I2C_100KHZ);        // Inicializa el protocolo i2c
    Lcd_Init();  // Inicializa la pantalla LCD
    configuracion_INT(); //sensor ultrasonico
    configuration_uart(); // Configuracion comunicacion serial
    Configuracion_timer1(); //sensor ultrasonico
    _TRISB2 = 1; //entrada del PIR
    _TRISC5 = 0; // salida del led
    _TRISC8 = 0; //salida del led
    _LATC5 = 0;
    _LATC8 = 0;
    while(1)
    {
        /******************sensor ultrasonico************************/
        TRIG = 1;
        delay_us(10);
        TRIG = 0;
        /*************++++++**** PIR +++****************************/
            if (PORTBbits.RB2 == 1) {
                _LATC5 = 0;
                _LATC8 = 1;
    //            delay_ms(100);

                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("Distancia:");
                Lcd_Set_Cursor(2,1);
                Lcd_Write_Float(Distancia);
                Lcd_Set_Cursor(2,15);
                Lcd_Blink();          
                delay_ms(2000);
                Lcd_NoBlink();
                Lcd_Clear();
                delay_ms(500);
                    U1TXREG='A';
                    delay_ms(100);
            } else {
                _LATC5 = 1;
                _LATC8 = 0;
    //            delay_ms(400);
            }
        
        
    }
    return 0;
}
