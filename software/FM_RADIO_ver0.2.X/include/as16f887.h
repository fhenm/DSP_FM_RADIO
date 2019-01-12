#ifndef _ASPIC_H_
#warning Header file as16f887.h included directly. Including <aspic.h> instead
#include <aspic.h>
#else
#ifndef _AS16F887_H_
#define _AS16F887_H_

INDF                                   equ 0000h
TMR0                                   equ 0001h
PCL                                    equ 0002h
STATUS                                 equ 0003h
#define CARRY                          BANKMASK(STATUS), 0
#define DC                             BANKMASK(STATUS), 1
#define ZERO                           BANKMASK(STATUS), 2
#define nPD                            BANKMASK(STATUS), 3
#define nTO                            BANKMASK(STATUS), 4
#define IRP                            BANKMASK(STATUS), 7
#define RP0                            BANKMASK(STATUS), 5
#define RP1                            BANKMASK(STATUS), 6
#ifndef _LIB_BUILD
#endif
FSR                                    equ 0004h
PORTA                                  equ 0005h
#define RA0                            BANKMASK(PORTA), 0
#define RA1                            BANKMASK(PORTA), 1
#define RA2                            BANKMASK(PORTA), 2
#define RA3                            BANKMASK(PORTA), 3
#define RA4                            BANKMASK(PORTA), 4
#define RA5                            BANKMASK(PORTA), 5
#define RA6                            BANKMASK(PORTA), 6
#define RA7                            BANKMASK(PORTA), 7
#ifndef _LIB_BUILD
#endif
PORTB                                  equ 0006h
#define RB0                            BANKMASK(PORTB), 0
#define RB1                            BANKMASK(PORTB), 1
#define RB2                            BANKMASK(PORTB), 2
#define RB3                            BANKMASK(PORTB), 3
#define RB4                            BANKMASK(PORTB), 4
#define RB5                            BANKMASK(PORTB), 5
#define RB6                            BANKMASK(PORTB), 6
#define RB7                            BANKMASK(PORTB), 7
#ifndef _LIB_BUILD
#endif
PORTC                                  equ 0007h
#define RC0                            BANKMASK(PORTC), 0
#define RC1                            BANKMASK(PORTC), 1
#define RC2                            BANKMASK(PORTC), 2
#define RC3                            BANKMASK(PORTC), 3
#define RC4                            BANKMASK(PORTC), 4
#define RC5                            BANKMASK(PORTC), 5
#define RC6                            BANKMASK(PORTC), 6
#define RC7                            BANKMASK(PORTC), 7
#ifndef _LIB_BUILD
#endif
PORTD                                  equ 0008h
#define RD0                            BANKMASK(PORTD), 0
#define RD1                            BANKMASK(PORTD), 1
#define RD2                            BANKMASK(PORTD), 2
#define RD3                            BANKMASK(PORTD), 3
#define RD4                            BANKMASK(PORTD), 4
#define RD5                            BANKMASK(PORTD), 5
#define RD6                            BANKMASK(PORTD), 6
#define RD7                            BANKMASK(PORTD), 7
#ifndef _LIB_BUILD
#endif
PORTE                                  equ 0009h
#define RE0                            BANKMASK(PORTE), 0
#define RE1                            BANKMASK(PORTE), 1
#define RE2                            BANKMASK(PORTE), 2
#define RE3                            BANKMASK(PORTE), 3
#ifndef _LIB_BUILD
#endif
PCLATH                                 equ 000Ah
#ifndef _LIB_BUILD
#endif
INTCON                                 equ 000Bh
#define RBIF                           BANKMASK(INTCON), 0
#define INTF                           BANKMASK(INTCON), 1
#define T0IF                           BANKMASK(INTCON), 2
#define RBIE                           BANKMASK(INTCON), 3
#define INTE                           BANKMASK(INTCON), 4
#define T0IE                           BANKMASK(INTCON), 5
#define PEIE                           BANKMASK(INTCON), 6
#define GIE                            BANKMASK(INTCON), 7
#define TMR0IF                         BANKMASK(INTCON), 2
#define TMR0IE                         BANKMASK(INTCON), 5
#ifndef _LIB_BUILD
#endif
PIR1                                   equ 000Ch
#define TMR1IF                         BANKMASK(PIR1), 0
#define TMR2IF                         BANKMASK(PIR1), 1
#define CCP1IF                         BANKMASK(PIR1), 2
#define SSPIF                          BANKMASK(PIR1), 3
#define TXIF                           BANKMASK(PIR1), 4
#define RCIF                           BANKMASK(PIR1), 5
#define ADIF                           BANKMASK(PIR1), 6
#define PSPIF                          BANKMASK(PIR1), 7
#ifndef _LIB_BUILD
#endif
PIR2                                   equ 000Dh
#define CCP2IF                         BANKMASK(PIR2), 0
#define ULPWUIF                        BANKMASK(PIR2), 2
#define BCLIF                          BANKMASK(PIR2), 3
#define EEIF                           BANKMASK(PIR2), 4
#define C1IF                           BANKMASK(PIR2), 5
#define C2IF                           BANKMASK(PIR2), 6
#define OSFIF                          BANKMASK(PIR2), 7
#ifndef _LIB_BUILD
#endif
TMR1L                                  equ 000Eh
TMR1H                                  equ 000Fh
T1CON                                  equ 0010h
#define TMR1ON                         BANKMASK(T1CON), 0
#define TMR1CS                         BANKMASK(T1CON), 1
#define nT1SYNC                        BANKMASK(T1CON), 2
#define T1OSCEN                        BANKMASK(T1CON), 3
#define TMR1GE                         BANKMASK(T1CON), 6
#define T1GINV                         BANKMASK(T1CON), 7
#define T1INSYNC                       BANKMASK(T1CON), 2
#define T1CKPS0                        BANKMASK(T1CON), 4
#define T1CKPS1                        BANKMASK(T1CON), 5
#define T1GIV                          BANKMASK(T1CON), 7
#define T1SYNC                         BANKMASK(T1CON), 2
#ifndef _LIB_BUILD
#endif
TMR2                                   equ 0011h
T2CON                                  equ 0012h
#define TMR2ON                         BANKMASK(T2CON), 2
#define T2CKPS0                        BANKMASK(T2CON), 0
#define T2CKPS1                        BANKMASK(T2CON), 1
#define TOUTPS0                        BANKMASK(T2CON), 3
#define TOUTPS1                        BANKMASK(T2CON), 4
#define TOUTPS2                        BANKMASK(T2CON), 5
#define TOUTPS3                        BANKMASK(T2CON), 6
#ifndef _LIB_BUILD
#endif
SSPBUF                                 equ 0013h
SSPCON                                 equ 0014h
#define CKP                            BANKMASK(SSPCON), 4
#define SSPEN                          BANKMASK(SSPCON), 5
#define SSPOV                          BANKMASK(SSPCON), 6
#define WCOL                           BANKMASK(SSPCON), 7
#define SSPM0                          BANKMASK(SSPCON), 0
#define SSPM1                          BANKMASK(SSPCON), 1
#define SSPM2                          BANKMASK(SSPCON), 2
#define SSPM3                          BANKMASK(SSPCON), 3
#ifndef _LIB_BUILD
#endif
CCPR1L                                 equ 0015h
CCPR1H                                 equ 0016h
CCP1CON                                equ 0017h
#define CCP1M0                         BANKMASK(CCP1CON), 0
#define CCP1M1                         BANKMASK(CCP1CON), 1
#define CCP1M2                         BANKMASK(CCP1CON), 2
#define CCP1M3                         BANKMASK(CCP1CON), 3
#define DC1B0                          BANKMASK(CCP1CON), 4
#define DC1B1                          BANKMASK(CCP1CON), 5
#define P1M0                           BANKMASK(CCP1CON), 6
#define P1M1                           BANKMASK(CCP1CON), 7
#define CCP1Y                          BANKMASK(CCP1CON), 4
#define CCP1X                          BANKMASK(CCP1CON), 5
#ifndef _LIB_BUILD
#endif
RCSTA                                  equ 0018h
#define RX9D                           BANKMASK(RCSTA), 0
#define OERR                           BANKMASK(RCSTA), 1
#define FERR                           BANKMASK(RCSTA), 2
#define ADDEN                          BANKMASK(RCSTA), 3
#define CREN                           BANKMASK(RCSTA), 4
#define SREN                           BANKMASK(RCSTA), 5
#define RX9                            BANKMASK(RCSTA), 6
#define SPEN                           BANKMASK(RCSTA), 7
#define RCD8                           BANKMASK(RCSTA), 0
#define RC9                            BANKMASK(RCSTA), 6
#define nRC8                           BANKMASK(RCSTA), 6
#define RC8_9                          BANKMASK(RCSTA), 6
#ifndef _LIB_BUILD
#endif
TXREG                                  equ 0019h
RCREG                                  equ 001Ah
CCPR2L                                 equ 001Bh
CCPR2H                                 equ 001Ch
CCP2CON                                equ 001Dh
#define DC2B0                          BANKMASK(CCP2CON), 4
#define DC2B1                          BANKMASK(CCP2CON), 5
#define CCP2M0                         BANKMASK(CCP2CON), 0
#define CCP2M1                         BANKMASK(CCP2CON), 1
#define CCP2M2                         BANKMASK(CCP2CON), 2
#define CCP2M3                         BANKMASK(CCP2CON), 3
#define CCP2Y                          BANKMASK(CCP2CON), 4
#define CCP2X                          BANKMASK(CCP2CON), 5
#ifndef _LIB_BUILD
#endif
ADRESH                                 equ 001Eh
ADCON0                                 equ 001Fh
#define ADON                           BANKMASK(ADCON0), 0
#define GO_nDONE                       BANKMASK(ADCON0), 1
#define GO                             BANKMASK(ADCON0), 1
#define CHS0                           BANKMASK(ADCON0), 2
#define CHS1                           BANKMASK(ADCON0), 3
#define CHS2                           BANKMASK(ADCON0), 4
#define CHS3                           BANKMASK(ADCON0), 5
#define ADCS0                          BANKMASK(ADCON0), 6
#define ADCS1                          BANKMASK(ADCON0), 7
#define nDONE                          BANKMASK(ADCON0), 1
#define GO_DONE                        BANKMASK(ADCON0), 1
#ifndef _LIB_BUILD
#endif
OPTION_REG                             equ 0081h
#define PSA                            BANKMASK(OPTION_REG), 3
#define T0SE                           BANKMASK(OPTION_REG), 4
#define T0CS                           BANKMASK(OPTION_REG), 5
#define INTEDG                         BANKMASK(OPTION_REG), 6
#define nRBPU                          BANKMASK(OPTION_REG), 7
#define PS0                            BANKMASK(OPTION_REG), 0
#define PS1                            BANKMASK(OPTION_REG), 1
#define PS2                            BANKMASK(OPTION_REG), 2
#ifndef _LIB_BUILD
#endif
TRISA                                  equ 0085h
#define TRISA0                         BANKMASK(TRISA), 0
#define TRISA1                         BANKMASK(TRISA), 1
#define TRISA2                         BANKMASK(TRISA), 2
#define TRISA3                         BANKMASK(TRISA), 3
#define TRISA4                         BANKMASK(TRISA), 4
#define TRISA5                         BANKMASK(TRISA), 5
#define TRISA6                         BANKMASK(TRISA), 6
#define TRISA7                         BANKMASK(TRISA), 7
#ifndef _LIB_BUILD
#endif
TRISB                                  equ 0086h
#define TRISB0                         BANKMASK(TRISB), 0
#define TRISB1                         BANKMASK(TRISB), 1
#define TRISB2                         BANKMASK(TRISB), 2
#define TRISB3                         BANKMASK(TRISB), 3
#define TRISB4                         BANKMASK(TRISB), 4
#define TRISB5                         BANKMASK(TRISB), 5
#define TRISB6                         BANKMASK(TRISB), 6
#define TRISB7                         BANKMASK(TRISB), 7
#ifndef _LIB_BUILD
#endif
TRISC                                  equ 0087h
#define TRISC0                         BANKMASK(TRISC), 0
#define TRISC1                         BANKMASK(TRISC), 1
#define TRISC2                         BANKMASK(TRISC), 2
#define TRISC3                         BANKMASK(TRISC), 3
#define TRISC4                         BANKMASK(TRISC), 4
#define TRISC5                         BANKMASK(TRISC), 5
#define TRISC6                         BANKMASK(TRISC), 6
#define TRISC7                         BANKMASK(TRISC), 7
#ifndef _LIB_BUILD
#endif
TRISD                                  equ 0088h
#define TRISD0                         BANKMASK(TRISD), 0
#define TRISD1                         BANKMASK(TRISD), 1
#define TRISD2                         BANKMASK(TRISD), 2
#define TRISD3                         BANKMASK(TRISD), 3
#define TRISD4                         BANKMASK(TRISD), 4
#define TRISD5                         BANKMASK(TRISD), 5
#define TRISD6                         BANKMASK(TRISD), 6
#define TRISD7                         BANKMASK(TRISD), 7
#ifndef _LIB_BUILD
#endif
TRISE                                  equ 0089h
#define TRISE0                         BANKMASK(TRISE), 0
#define TRISE1                         BANKMASK(TRISE), 1
#define TRISE2                         BANKMASK(TRISE), 2
#define TRISE3                         BANKMASK(TRISE), 3
#ifndef _LIB_BUILD
#endif
PIE1                                   equ 008Ch
#define TMR1IE                         BANKMASK(PIE1), 0
#define TMR2IE                         BANKMASK(PIE1), 1
#define CCP1IE                         BANKMASK(PIE1), 2
#define SSPIE                          BANKMASK(PIE1), 3
#define TXIE                           BANKMASK(PIE1), 4
#define RCIE                           BANKMASK(PIE1), 5
#define ADIE                           BANKMASK(PIE1), 6
#ifndef _LIB_BUILD
#endif
PIE2                                   equ 008Dh
#define CCP2IE                         BANKMASK(PIE2), 0
#define ULPWUIE                        BANKMASK(PIE2), 2
#define BCLIE                          BANKMASK(PIE2), 3
#define EEIE                           BANKMASK(PIE2), 4
#define C1IE                           BANKMASK(PIE2), 5
#define C2IE                           BANKMASK(PIE2), 6
#define OSFIE                          BANKMASK(PIE2), 7
#ifndef _LIB_BUILD
#endif
PCON                                   equ 008Eh
#define nBOR                           BANKMASK(PCON), 0
#define nPOR                           BANKMASK(PCON), 1
#define SBOREN                         BANKMASK(PCON), 4
#define ULPWUE                         BANKMASK(PCON), 5
#define nBO                            BANKMASK(PCON), 0
#ifndef _LIB_BUILD
#endif
OSCCON                                 equ 008Fh
#define SCS                            BANKMASK(OSCCON), 0
#define LTS                            BANKMASK(OSCCON), 1
#define HTS                            BANKMASK(OSCCON), 2
#define OSTS                           BANKMASK(OSCCON), 3
#define IRCF0                          BANKMASK(OSCCON), 4
#define IRCF1                          BANKMASK(OSCCON), 5
#define IRCF2                          BANKMASK(OSCCON), 6
#ifndef _LIB_BUILD
#endif
OSCTUNE                                equ 0090h
#define TUN0                           BANKMASK(OSCTUNE), 0
#define TUN1                           BANKMASK(OSCTUNE), 1
#define TUN2                           BANKMASK(OSCTUNE), 2
#define TUN3                           BANKMASK(OSCTUNE), 3
#define TUN4                           BANKMASK(OSCTUNE), 4
#ifndef _LIB_BUILD
#endif
SSPCON2                                equ 0091h
#define SEN                            BANKMASK(SSPCON2), 0
#define RSEN                           BANKMASK(SSPCON2), 1
#define PEN                            BANKMASK(SSPCON2), 2
#define RCEN                           BANKMASK(SSPCON2), 3
#define ACKEN                          BANKMASK(SSPCON2), 4
#define ACKDT                          BANKMASK(SSPCON2), 5
#define ACKSTAT                        BANKMASK(SSPCON2), 6
#define GCEN                           BANKMASK(SSPCON2), 7
#ifndef _LIB_BUILD
#endif
PR2                                    equ 0092h
SSPADD                                 equ 0093h
SSPMSK                                 equ 0093h
MSK                                    equ 0093h
#define MSK0                           BANKMASK(SSPMSK), 0
#define MSK1                           BANKMASK(SSPMSK), 1
#define MSK2                           BANKMASK(SSPMSK), 2
#define MSK3                           BANKMASK(SSPMSK), 3
#define MSK4                           BANKMASK(SSPMSK), 4
#define MSK5                           BANKMASK(SSPMSK), 5
#define MSK6                           BANKMASK(SSPMSK), 6
#define MSK7                           BANKMASK(SSPMSK), 7
#ifndef _LIB_BUILD
#endif
SSPSTAT                                equ 0094h
#define BF                             BANKMASK(SSPSTAT), 0
#define UA                             BANKMASK(SSPSTAT), 1
#define R_nW                           BANKMASK(SSPSTAT), 2
#define S                              BANKMASK(SSPSTAT), 3
#define P                              BANKMASK(SSPSTAT), 4
#define D_nA                           BANKMASK(SSPSTAT), 5
#define CKE                            BANKMASK(SSPSTAT), 6
#define SMP                            BANKMASK(SSPSTAT), 7
#define R                              BANKMASK(SSPSTAT), 2
#define D                              BANKMASK(SSPSTAT), 5
#define I2C_READ                       BANKMASK(SSPSTAT), 2
#define I2C_START                      BANKMASK(SSPSTAT), 3
#define I2C_STOP                       BANKMASK(SSPSTAT), 4
#define I2C_DATA                       BANKMASK(SSPSTAT), 5
#define nW                             BANKMASK(SSPSTAT), 2
#define nA                             BANKMASK(SSPSTAT), 5
#define nWRITE                         BANKMASK(SSPSTAT), 2
#define nADDRESS                       BANKMASK(SSPSTAT), 5
#define R_W                            BANKMASK(SSPSTAT), 2
#define D_A                            BANKMASK(SSPSTAT), 5
#define READ_WRITE                     BANKMASK(SSPSTAT), 2
#define DATA_ADDRESS                   BANKMASK(SSPSTAT), 5
#ifndef _LIB_BUILD
#endif
WPUB                                   equ 0095h
#define WPUB0                          BANKMASK(WPUB), 0
#define WPUB1                          BANKMASK(WPUB), 1
#define WPUB2                          BANKMASK(WPUB), 2
#define WPUB3                          BANKMASK(WPUB), 3
#define WPUB4                          BANKMASK(WPUB), 4
#define WPUB5                          BANKMASK(WPUB), 5
#define WPUB6                          BANKMASK(WPUB), 6
#define WPUB7                          BANKMASK(WPUB), 7
#ifndef _LIB_BUILD
#endif
IOCB                                   equ 0096h
#define IOCB0                          BANKMASK(IOCB), 0
#define IOCB1                          BANKMASK(IOCB), 1
#define IOCB2                          BANKMASK(IOCB), 2
#define IOCB3                          BANKMASK(IOCB), 3
#define IOCB4                          BANKMASK(IOCB), 4
#define IOCB5                          BANKMASK(IOCB), 5
#define IOCB6                          BANKMASK(IOCB), 6
#define IOCB7                          BANKMASK(IOCB), 7
#ifndef _LIB_BUILD
#endif
VRCON                                  equ 0097h
#define VRSS                           BANKMASK(VRCON), 4
#define VRR                            BANKMASK(VRCON), 5
#define VROE                           BANKMASK(VRCON), 6
#define VREN                           BANKMASK(VRCON), 7
#define VR0                            BANKMASK(VRCON), 0
#define VR1                            BANKMASK(VRCON), 1
#define VR2                            BANKMASK(VRCON), 2
#define VR3                            BANKMASK(VRCON), 3
#ifndef _LIB_BUILD
#endif
TXSTA                                  equ 0098h
#define TX9D                           BANKMASK(TXSTA), 0
#define TRMT                           BANKMASK(TXSTA), 1
#define BRGH                           BANKMASK(TXSTA), 2
#define SENDB                          BANKMASK(TXSTA), 3
#define SYNC                           BANKMASK(TXSTA), 4
#define TXEN                           BANKMASK(TXSTA), 5
#define TX9                            BANKMASK(TXSTA), 6
#define CSRC                           BANKMASK(TXSTA), 7
#define TXD8                           BANKMASK(TXSTA), 0
#define nTX8                           BANKMASK(TXSTA), 6
#define TX8_9                          BANKMASK(TXSTA), 6
#ifndef _LIB_BUILD
#endif
SPBRG                                  equ 0099h
#define BRG0                           BANKMASK(SPBRG), 0
#define BRG1                           BANKMASK(SPBRG), 1
#define BRG2                           BANKMASK(SPBRG), 2
#define BRG3                           BANKMASK(SPBRG), 3
#define BRG4                           BANKMASK(SPBRG), 4
#define BRG5                           BANKMASK(SPBRG), 5
#define BRG6                           BANKMASK(SPBRG), 6
#define BRG7                           BANKMASK(SPBRG), 7
#ifndef _LIB_BUILD
#endif
SPBRGH                                 equ 009Ah
#define BRG8                           BANKMASK(SPBRGH), 0
#define BRG9                           BANKMASK(SPBRGH), 1
#define BRG10                          BANKMASK(SPBRGH), 2
#define BRG11                          BANKMASK(SPBRGH), 3
#define BRG12                          BANKMASK(SPBRGH), 4
#define BRG13                          BANKMASK(SPBRGH), 5
#define BRG14                          BANKMASK(SPBRGH), 6
#define BRG15                          BANKMASK(SPBRGH), 7
#ifndef _LIB_BUILD
#endif
PWM1CON                                equ 009Bh
#define PRSEN                          BANKMASK(PWM1CON), 7
#define PDC0                           BANKMASK(PWM1CON), 0
#define PDC1                           BANKMASK(PWM1CON), 1
#define PDC2                           BANKMASK(PWM1CON), 2
#define PDC3                           BANKMASK(PWM1CON), 3
#define PDC4                           BANKMASK(PWM1CON), 4
#define PDC5                           BANKMASK(PWM1CON), 5
#define PDC6                           BANKMASK(PWM1CON), 6
#ifndef _LIB_BUILD
#endif
ECCPAS                                 equ 009Ch
#define ECCPASE                        BANKMASK(ECCPAS), 7
#define PSSBD0                         BANKMASK(ECCPAS), 0
#define PSSBD1                         BANKMASK(ECCPAS), 1
#define PSSAC0                         BANKMASK(ECCPAS), 2
#define PSSAC1                         BANKMASK(ECCPAS), 3
#define ECCPAS0                        BANKMASK(ECCPAS), 4
#define ECCPAS1                        BANKMASK(ECCPAS), 5
#define ECCPAS2                        BANKMASK(ECCPAS), 6
#ifndef _LIB_BUILD
#endif
PSTRCON                                equ 009Dh
#define STRA                           BANKMASK(PSTRCON), 0
#define STRB                           BANKMASK(PSTRCON), 1
#define STRC                           BANKMASK(PSTRCON), 2
#define STRD                           BANKMASK(PSTRCON), 3
#define STRSYNC                        BANKMASK(PSTRCON), 4
#ifndef _LIB_BUILD
#endif
ADRESL                                 equ 009Eh
ADCON1                                 equ 009Fh
#define VCFG0                          BANKMASK(ADCON1), 4
#define VCFG1                          BANKMASK(ADCON1), 5
#define ADFM                           BANKMASK(ADCON1), 7
#ifndef _LIB_BUILD
#endif
WDTCON                                 equ 0105h
#define SWDTEN                         BANKMASK(WDTCON), 0
#define WDTPS0                         BANKMASK(WDTCON), 1
#define WDTPS1                         BANKMASK(WDTCON), 2
#define WDTPS2                         BANKMASK(WDTCON), 3
#define WDTPS3                         BANKMASK(WDTCON), 4
#ifndef _LIB_BUILD
#endif
CM1CON0                                equ 0107h
#define C1R                            BANKMASK(CM1CON0), 2
#define C1POL                          BANKMASK(CM1CON0), 4
#define C1OE                           BANKMASK(CM1CON0), 5
#define C1OUT                          BANKMASK(CM1CON0), 6
#define C1ON                           BANKMASK(CM1CON0), 7
#define C1CH0                          BANKMASK(CM1CON0), 0
#define C1CH1                          BANKMASK(CM1CON0), 1
#ifndef _LIB_BUILD
#endif
CM2CON0                                equ 0108h
#define C2R                            BANKMASK(CM2CON0), 2
#define C2POL                          BANKMASK(CM2CON0), 4
#define C2OE                           BANKMASK(CM2CON0), 5
#define C2OUT                          BANKMASK(CM2CON0), 6
#define C2ON                           BANKMASK(CM2CON0), 7
#define C2CH0                          BANKMASK(CM2CON0), 0
#define C2CH1                          BANKMASK(CM2CON0), 1
#ifndef _LIB_BUILD
#endif
CM2CON1                                equ 0109h
#define C2SYNC                         BANKMASK(CM2CON1), 0
#define T1GSS                          BANKMASK(CM2CON1), 1
#define C2RSEL                         BANKMASK(CM2CON1), 4
#define C1RSEL                         BANKMASK(CM2CON1), 5
#define MC2OUT                         BANKMASK(CM2CON1), 6
#define MC1OUT                         BANKMASK(CM2CON1), 7
#ifndef _LIB_BUILD
#endif
EEDATA                                 equ 010Ch
EEDAT                                  equ 010Ch
EEADR                                  equ 010Dh
EEDATH                                 equ 010Eh
#ifndef _LIB_BUILD
#endif
EEADRH                                 equ 010Fh
#ifndef _LIB_BUILD
#endif
SRCON                                  equ 0185h
#define FVREN                          BANKMASK(SRCON), 0
#define PULSR                          BANKMASK(SRCON), 2
#define PULSS                          BANKMASK(SRCON), 3
#define C2REN                          BANKMASK(SRCON), 4
#define C1SEN                          BANKMASK(SRCON), 5
#define SR0                            BANKMASK(SRCON), 6
#define SR1                            BANKMASK(SRCON), 7
#ifndef _LIB_BUILD
#endif
BAUDCTL                                equ 0187h
#define ABDEN                          BANKMASK(BAUDCTL), 0
#define WUE                            BANKMASK(BAUDCTL), 1
#define BRG16                          BANKMASK(BAUDCTL), 3
#define SCKP                           BANKMASK(BAUDCTL), 4
#define RCIDL                          BANKMASK(BAUDCTL), 6
#define ABDOVF                         BANKMASK(BAUDCTL), 7
#ifndef _LIB_BUILD
#endif
ANSEL                                  equ 0188h
#define ANS0                           BANKMASK(ANSEL), 0
#define ANS1                           BANKMASK(ANSEL), 1
#define ANS2                           BANKMASK(ANSEL), 2
#define ANS3                           BANKMASK(ANSEL), 3
#define ANS4                           BANKMASK(ANSEL), 4
#define ANS5                           BANKMASK(ANSEL), 5
#define ANS6                           BANKMASK(ANSEL), 6
#define ANS7                           BANKMASK(ANSEL), 7
#ifndef _LIB_BUILD
#endif
ANSELH                                 equ 0189h
#define ANS8                           BANKMASK(ANSELH), 0
#define ANS9                           BANKMASK(ANSELH), 1
#define ANS10                          BANKMASK(ANSELH), 2
#define ANS11                          BANKMASK(ANSELH), 3
#define ANS12                          BANKMASK(ANSELH), 4
#define ANS13                          BANKMASK(ANSELH), 5
#ifndef _LIB_BUILD
#endif
EECON1                                 equ 018Ch
#define RD                             BANKMASK(EECON1), 0
#define WR                             BANKMASK(EECON1), 1
#define WREN                           BANKMASK(EECON1), 2
#define WRERR                          BANKMASK(EECON1), 3
#define EEPGD                          BANKMASK(EECON1), 7
#ifndef _LIB_BUILD
#endif
EECON2                                 equ 018Dh

#endif
#endif
