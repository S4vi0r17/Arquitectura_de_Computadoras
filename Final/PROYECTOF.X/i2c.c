#include <xc.h>
#include "i2c.h"

#ifdef I2C_MASTER_MODE
void I2C_Init_Master(unsigned char sp_i2c)
{
    TRISBbits.TRISB8 = 1;
    TRISBbits.TRISB9 = 1;
//    SSPSTAT = sp_i2c;
//	SSPCON1 = 0x28;
//    SSPCON2 = 0x00;
 /*   if(sp_i2c == I2C_100KHZ){
        SSPADD = 119;
    }
    else if(sp_i2c == I2C_400KHZ){
        SSPADD = 29;
    } */
    // Configuraci?n de velocidad I2C
    I2C1BRG = 0x0188; // Frecuencia de reloj de 100 kHz
    I2C1CON = 0xC220; // Habilitar I2C y configurar como maestro

}

void I2C_Start(void)
{
/*    SSPCON2bits.SEN = 1;
    while(PIR1bits.SSPIF == 0);
    PIR1bits.SSPIF = 0; */
    while(I2C1CONbits.SEN  || I2C1CONbits.PEN ||I2C1CONbits.RCEN || I2C1CONbits.ACKEN   || I2C1STATbits.TRSTAT){}; // Iniciar condici?n de inicio
    I2C1CONbits.SEN=1;
    while (I2C1CONbits.SEN); // Esperar hasta que se complete el inicio
    IFS1bits.MI2C1IF=0;
}

void I2C_Stop(void)
{   while(I2C1CONbits.SEN || I2C1CONbits.PEN ||I2C1CONbits.RCEN || I2C1CONbits.ACKEN   || I2C1STATbits.TRSTAT){}; // Iniciar condici?n de inicio  
    I2C1CONbits.PEN = 1; // Iniciar condici?n de parada
    while (I2C1CONbits.PEN); // Esperar hasta que se complete la parada
//    
//    while (I2C1CONbits.PEN);
}
/*
void I2C_Restart(void)
{
    SSPCON2bits.RSEN = 1;
    while(PIR1bits.SSPIF == 0);
    PIR1bits.SSPIF = 0;
}

void I2C_Ack(void)
{
    SSPCON2bits.ACKDT = 0;
	SSPCON2bits.ACKEN = 1;
	while(PIR1bits.SSPIF == 0);
    PIR1bits.SSPIF = 0;
}

void I2C_Nack(void)
{
    SSPCON2bits.ACKDT = 1;
	SSPCON2bits.ACKEN = 1;
	while(PIR1bits.SSPIF == 0);
    PIR1bits.SSPIF = 0;
}
*/
short  I2C_Write(char data)
{
   /* SSPBUF = data;
    while(PIR1bits.SSPIF == 0);
    PIR1bits.SSPIF = 0;
    short b_ok = SSPCON2bits.ACKSTAT;
    return b_ok; */
    I2C1TRN = data; // Cargar datos en el registro de transmisi?n
    while (I2C1STATbits.TBF); 
    while (!IFS1bits.MI2C1IF); 
    IFS1bits.MI2C1IF =0;
    while (I2C1STATbits.ACKSTAT ); 
    short b_ok = I2C1STATbits.ACKSTAT;
    return  b_ok;
}

unsigned char I2C_Read(void)
{
    I2C1CONbits.RSEN = 1; // Habilitar la recepci?n
    while (I2C1CONbits.RSEN); 
    IFS1bits.MI2C1IF=0;
    while (I2C1STATbits.TBF); 
    IFS1bits.MI2C1IF=0;
    while (I2C1STATbits.ACKSTAT); 
    I2C1CONbits.RCEN=1;
    while (I2C1CONbits.RCEN);
    I2C1STATbits.I2COV=0;
    return (I2C1RCV);
}
#endif

#ifdef I2C_SLAVE_MODE
void I2C_Init_Slave(unsigned char add_slave)
{
    TRIS_SCL = 1;
    TRIS_SDA = 1;
    SSPSTAT = 0x80;
    SSPADD = add_slave;
    SSPCON1 = 0x36;
    SSPCON2 = 0x01;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 1;
}

short I2C_Error_Read(void)
{
    SSPCON1bits.CKP = 0;
    return ((SSPCON1bits.SSPOV) || (SSPCON1bits.WCOL)) ? 1 : 0;
}

short I2C_Write_Mode(void)
{
    return(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) ? 1 : 0;
}
short I2C_Read_Mode(void)
{
    return (!SSPSTATbits.D_nA && SSPSTATbits.R_nW) ? 1: 0;
}

void I2C_Error_Data(void)
{
    short z;
    SSPCON1bits.CKP = 0;
    z = SSPBUF;
    SSPCON1bits.SSPOV = 0;
    SSPCON1bits.WCOL = 0;
    SSPCON1bits.CKP = 1;
    SSPCON1bits.SSPM3 = 0;
}

unsigned char I2C_Read_Slave(void)
{
    short z;
    unsigned char dato_i2c;
    z = SSPBUF;
    while(!BF);
    dato_i2c = SSPBUF;
    SSPCON1bits.CKP = 1;
    SSPCON1bits.SSPM3 = 0;
    return dato_i2c;
}

void I2C_Write_Slave(char dato_i2c)
{
    short z;
    z = SSPBUF;
    BF = 0;
    SSPBUF = dato_i2c;
    SSPCON1bits.CKP = 1;
    while(SSPSTATbits.BF == 1);
}
#endif