/*
 * File:   newmain.c
 * Author: msk319882
 *
 * Created on 2017/01/18, 22:51
 * �C�� 2017/1/24, 20:35
 *
 * �K���ɏ����Ă�̂ŊԈႢ��o�O�����邩������Ȃ��i�j
 *
 * �l�ߍ��߂邾���l�ߍ���FM���W�I
 * 
 * �@�\�F�\�t�g�E�F�A�T�[�`�ARSSI&SNR臒l�ݒ�ƃJ�E���^�ABUSSBOOST�Z���N�g�A��M���g����32�܂ŋL���ł�����g�������@�\
 * �[�d�@�\�i4.1v�Ď��̓n�[�h�Ŏ����j�AI2C�֐������AEEPROM�̑O��̋N���ݒ���������ނ��Ƃɂ�莟��ړ����ɑO��N�����̏�ԂɂȂ�
 * ���^LCD�ɂ��\���A���^LCD�̓d������
 *
 * PIC16f887���g��������PIN�����߂��Ⴍ����]���Ă��Ă��������Ȃ��̂���_
 *
 * �ҋ@�d�� 0.1mA �N�����d�� 23~27mA
 */

//HI-TECH�̃w�b�_���C���N���[�h
#include <pic.h>
#include <pic16f887.h>

/*�f�B���C��`*/
#define _XTAL_FREQ 4000000
#define  _XTAL_FREQ_LOW 32000
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define __low_delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ_LOW/4000.0)))
#define __low_delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ_LOW/4000000.0)))


/*�O��PIN��`*/
#define LED1 RB3    //LED��
#define LIP_UP RC1  //���`���E�����[�d�I��������low
#define LIP_PW RA6  //���`���E���[�d��H�����쒆HI
#define SDA RC4     //SDA
#define SCL RC3     //SCL
#define LCD_RS_PIN RC5  //LCD���Z�b�g�s����
#define FMIC_EN_PIN RC6     //FMIC��EN�s����
#define TARCON RD4	//���M�����[�^�R���g���[��
#define UP_PB RA1   //�A�b�v�{�^��
#define PW_PB RA2   //�p���[�{�^��
#define DW_PB RA3   //�_�E���{�^��
#define LCDADD 0x7C     //LCD�A�h���X
#define FMADD 0b01101010    //FMIC�A�h���X
int TMR1ON_Flag = 0; //�^�C�}�[�t���O
int fcont_pp = 765;//���g���J�E���^


//EEPROM�����f�[�^
//�����ɂ̓v���Z�b�g�Ƃ��č��R�Ŏ�M�ł������Ȏ��g�������Ă���
//�܂��A�O��N�����̃{�����[���A���g���A���g�������J�E���^�l�ABUSSBOOST�ݒ�l�ARSSI�ASNR臒l������
//�d����؂��Ă��c��悤��EEPROM�ɓ����

//�{�����[���⏉�����g��������
__EEPROM_DATA(15, 15, 76, 5, 0, 0, 0, 0);
//�������炪���g�������@���g���l
__EEPROM_DATA(76, 80, 86, 80, 83, 78, 78, 81);//�������炪��2��
__EEPROM_DATA(79, 82, 84, 85, 80, 77, 79, 78);
__EEPROM_DATA(0, 0, 0, 0, 0, 0, 0, 0);
__EEPROM_DATA(0, 0, 0, 0, 0, 0, 0, 0);
__EEPROM_DATA(5, 0, 1, 3, 6, 5, 3, 5);//�������炪����1��
__EEPROM_DATA(3, 1, 3, 6, 7, 8, 5, 9);
__EEPROM_DATA(0, 0, 0, 0, 0, 0, 0, 0);
__EEPROM_DATA(0, 0, 0, 0, 0, 0, 0, 0);
__EEPROM_DATA(0, 0, 0, 0, 0, 0, 0, 0);
//RSSI�ASNR�ABUSSBOOST�ݒ�l������
__EEPROM_DATA(0, 0, 0, 48, 9, 46, 1, 0);


//�R���t�B�O�ݒ�
__CONFIG(
        BOREN_OFF
        & WRT_OFF
        & LVP_OFF
        & FCMEN_OFF
        & IESO_OFF
        & CPD_OFF
        & CP_OFF
        & MCLRE_OFF
        & PWRTE_ON
        & FOSC_INTRC_NOCLKOUT
        & WDTE_OFF
        );

void interrupt InterTimer(){//���荞�݊֐�
    if (TMR1IF == 1) {  // �^�C�}�[1�̃t���O�����邩�H
        TMR1ON_Flag = 0 ;// �^�C�}�[�t���O���Z�b�g
        TMR1IF = 0 ;
    }
}

oscsel(int sel1) {//�����I�V���[�g�Z���N�g
    switch (sel1) {
        case 0:
            OSCCON = 0b01100000; //4MHz
            break;
        case 1:
            OSCCON = 0b00000000; //32KHz
            break;
        case 2:
            OSCCON = 0b01000000; //1MHz
            break;
        case 3:
            OSCCON = 0b00110000; //500KHz
            break;
    }
}

memory_fcont(int sel, int add, int fcont) { //���g��EEPROM�������݁@�ǂݍ��݁@�֐�
    int fcon;
    switch (sel) {
        case 0:
            fcon = (eeprom_read(add + 7 + 1) * 10 + eeprom_read(add + 7 + 32 + 1));
            break;
        case 1:
            eeprom_write(add + 7 + 1, (fcont / 10));
            eeprom_write((add + 7 + 32 + 1), (fcont % 10));
            break;
        default:
            break;
    }
    return (fcon);
}

i2c_Tx(char date) {//I2C1byte��������
    LED1 = 1;
    SSPIF = 0;
    SSPBUF = date;
    while (BF);
    while (!SSPIF);
    LED1 = 0;
}

i2c_byte_write(int sel, char add, char cmd, char date1, char date2) {//I2C��������
    SEN = 1;
    while (SEN);
    i2c_Tx(add);
    i2c_Tx(cmd);
    i2c_Tx(date1);
    if (sel == 1)i2c_Tx(date2);
    SSPIF = 0;
    PEN = 1;
    while (PEN);
    __delay_us(27);
}

i2c_byte_read(unsigned int sel, char add, char regadd) {//I2C���[�h�֐�
    char read_data[2];
    SSPADD = 0x09; //I2C�ʐM���x�Ē�`
    SEN = 1; //�X�^�[�g�R���f�V�����Z�b�g
    while (SEN); //���M�����܂ő҂�
    i2c_Tx(add); //IC�A�h���X���M
    i2c_Tx(regadd); //���W�X�^�A�h���X���M
    RSEN = 1; //���X�^�[�g�R���f�B�V�����Z�b�g
    while (RSEN); //���M�����܂ő҂�
    i2c_Tx(add + 0x01); //IC�A�h���X�����[�h���[�h�ŏ�������
    RCEN = 1; //�}�X�^�����[�h���[�h��
    while (RCEN); //���M�����܂ő҂�
    read_data[0] = SSPBUF; //�o�b�t�@�̃f�[�^��Ҕ�
    ACKDT = 0; //ACK���Z�b�g
    ACKEN = 1; //ACK�𑗐M
    while (ACKEN); //���M�����܂ő҂�
    RCEN = 1; //�}�X�^�����[�h���[�h��
    while (RCEN); //���M�����܂ő҂�
    read_data[1] = SSPBUF; //�o�b�t�@�̃f�[�^��Ҕ�
    ACKDT = 1; //NO-ACK���Z�b�g
    ACKEN = 1; //NO-ACK�𑗐M
    while (ACKEN); //���M�����܂ő҂�
    PEN = 1; //�X�g�b�v�R���f�V�����Z�b�g
    while (PEN);//���M�����܂ő҂�
    SSPADD = 0x09; //I2C���x�Ē�`
    return (read_data[sel]);
}

lcd_RS() { //LCD�������֐�
    LCD_RS_PIN = 0;//LCD�̃n�[�h�E�F�A���Z�b�g
    __delay_ms(100);
    LCD_RS_PIN = 1;
    __delay_ms(100);
    i2c_byte_write(0, LCDADD, 0x00, 0x38, 0);//�����ݒ著�M
    i2c_byte_write(0, LCDADD, 0x00, 0x39, 0);
    i2c_byte_write(0, LCDADD, 0x00, 0x14, 0);
    i2c_byte_write(0, LCDADD, 0x00, 0x70, 0);
    i2c_byte_write(0, LCDADD, 0x00, 0x56, 0);
    i2c_byte_write(0, LCDADD, 0x00, 0x6C, 0);
    __delay_ms(200);
    i2c_byte_write(0, LCDADD, 0x00, 0x38, 0);
    i2c_byte_write(0, LCDADD, 0x00, 0x0C, 0);
    i2c_byte_write(0, LCDADD, 0x00, 0x01, 0);
    __delay_ms(10);
}

fmIC_RS() {//FMIC�������֐�
    FMIC_EN_PIN = 0;//FMIC�n�[�h�E�F�A���Z�b�g
    __delay_ms(100);
    FMIC_EN_PIN = 1;
    __delay_ms(500);
    i2c_byte_write(1, FMADD, 0x10, 0b01100000, 0x00);
    i2c_byte_write(1, FMADD, 0x3f, 0x00, 0b00010011);
    i2c_byte_write(1, FMADD, 0x02, 0x00, 0b00000111); //FM�~���[�g������
    i2c_byte_write(1, FMADD, 0x04, ((eeprom_read(86) & 0b00000011) | 0b11000000), 0b10000000); //BUSSBOOST�̒l��EEPROM����FMIC�ɏ�������
    i2c_byte_write(1, FMADD, 0x16, 0b00001001, 0b00000010);//38kHz�ɐݒ�
    //i2c_byte_write(1, FMADD, 0x16, 0b00000000, 0b00000010);//32.768kHz�ɐݒ�
    i2c_byte_write(1, FMADD, 0x0a, 0b00000100, 0x00); //AFC�ݒ�
    i2c_byte_write(1, FMADD, 0x0c, 0x00, 0b00101100); //�ш��64����32��
}

fmIC_VOL(char vol) {//�{�����[���֐� vol�ɓ��ꂽ�l��FMIC�Ƀ{�����[���ݒ�l�Ƃ��đ��M
    i2c_byte_write(1, FMADD, 0x0F, 0b00000000, (vol & 0b00011111));
}

lcd_int(int sel, unsigned int charadd, unsigned int cnt1) {//LCD�ɐ�����\������֐��@1��1��2��2�� DDRAM�A�h���X int�\������l
    int LCD_2, LCD_1;
    LCD_1 = cnt1 % 10;
    LCD_2 = cnt1 / 10;
    LCD_1 = (LCD_1 & 0x0f) | 0x30;
    LCD_2 = (LCD_2 & 0x0f) | 0x30;
    i2c_byte_write(0, LCDADD, 0x00, (0x80 + charadd), 0);
    i2c_byte_write(0, LCDADD, 0xC0, LCD_2, 0);
    if (sel == 2)i2c_byte_write(0, LCDADD, 0xC0, LCD_1, 0);
}

lcd_float(unsigned int charadd, unsigned int cnt1) {//3���̒l�����ꌅ�������_���ʂƂ��ĕ\������֐��@DDRAM�A�h���X int3�\�����鐔��
    int LCD_3, LCD_2, LCD_1;
    LCD_3 = cnt1 / 100;
    LCD_2 = (cnt1 - LCD_3 * 100) / 10;
    LCD_1 = cnt1 % 10;
    LCD_1 = (LCD_1 & 0x0f) | 0x30;
    LCD_2 = (LCD_2 & 0x0f) | 0x30;
    LCD_3 = (LCD_3 & 0x0f) | 0x30;
    i2c_byte_write(0, LCDADD, 0x00, (0x80 + charadd), 0);
    i2c_byte_write(0, LCDADD, 0xC0, LCD_3, 0);
    i2c_byte_write(0, LCDADD, 0xC0, LCD_2, 0);
    i2c_byte_write(0, LCDADD, 0xC0, '.', 0);
    i2c_byte_write(0, LCDADD, 0xC0, LCD_1, 0);
}

start_LCD_POINT() {//�N������LCD�ɕ\�����镶���Z�b�g�֐�
    i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x03), 0);
    i2c_byte_write(0, LCDADD, 0xC0, '.', 0);
    i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x05), 0);
    i2c_byte_write(0, LCDADD, 0xC0, 'M', 0);
    i2c_byte_write(0, LCDADD, 0xC0, 'H', 0);
    i2c_byte_write(0, LCDADD, 0xC0, 'z', 0);
    i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x40), 0);
    i2c_byte_write(0, LCDADD, 0xC0, 'N', 0);
    i2c_byte_write(0, LCDADD, 0xC0, 'o', 0);
    i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x45), 0);
    i2c_byte_write(0, LCDADD, 0xC0, 'v', 0);
}

fm_fcon_write(int fconv) {//3���̐�����ϊ�����FMIC�Ɏ��g���Ƃ��ď������݁@��F765 �� 76.5Mhz
    i2c_byte_write(1, FMADD, 0x03, ((((fconv << 1) & 0xff00) + 0x0000) >> 8), (fconv << 1) & 0x00ff);
    i2c_byte_write(1, FMADD, 0x03, ((((fconv << 1) & 0xff00) + 0x8000) >> 8), (fconv << 1) & 0x00ff);
}
seek_RSSI() {//RSSI臒l�ݒ�֐�
    int RSSI, endf;
    RSSI = eeprom_read(84);
    lcd_int(2, 0x05, RSSI);
    while (PW_PB || UP_PB || DW_PB);
    while (!PW_PB || (UP_PB || DW_PB)) {
        endf = ((i2c_byte_read(1, FMADD, 0x12) & 0b11111000) >> 3)*3;
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x00), 0);
        lcd_int(2, 0x40, endf);
        __delay_ms(70);
        if (UP_PB == 1 && (RSSI < 93)) {
            RSSI++;
            lcd_int(2, 0x05, RSSI);
            while (PW_PB);
        }
        __delay_ms(70);
        if (DW_PB == 1 && (RSSI > 0x00)) {
            RSSI--;
            lcd_int(2, 0x05, RSSI);
            while (PW_PB);
        }
    }
    eeprom_write(84, RSSI);
    i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x43), 0);
    i2c_byte_write(0, LCDADD, 0xC0, ' ', 0);
    i2c_byte_write(0, LCDADD, 0xC0, ' ', 0);
    i2c_byte_write(0, LCDADD, 0xC0, 'O', 0);
    i2c_byte_write(0, LCDADD, 0xC0, 'K', 0);
}

seek_SNR() {//SNR臒l�ݒ�֐�
    int SNR, endf;
    SNR = eeprom_read(85);
    lcd_int(2, 0x05, SNR);
    while (PW_PB || UP_PB || DW_PB);
    while (!PW_PB || (UP_PB || DW_PB)) {
        endf = ((i2c_byte_read(1, FMADD, 0x14) & 0b11000000) >> 6) + ((i2c_byte_read(0, FMADD, 0x14) & 0b00011111) << 2);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x00), 0);
        lcd_int(2, 0x40, endf);
        __delay_ms(70);
        if (UP_PB == 1 && (SNR < 127)) {
            SNR++;
            lcd_int(2, 0x05, SNR);
            while (PW_PB);
        }
        __delay_ms(70);
        if (DW_PB == 1 && (SNR > 0x00)) {
            SNR--;
            lcd_int(2, 0x05, SNR);
            while (PW_PB);
        }
    }
    eeprom_write(85, SNR);
    i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x44), 0);
    i2c_byte_write(0, LCDADD, 0xC0, ' ', 0);
    i2c_byte_write(0, LCDADD, 0xC0, 'O', 0);
    i2c_byte_write(0, LCDADD, 0xC0, 'K', 0);
}

buss_boost() {//BUSSBOOST�ݒ�֐�
    unsigned char boost;
    boost = ((i2c_byte_read(0, FMADD, 0x04) & 0b00000011));
    i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x46), 0);
    lcd_int(2, 0x40, boost);
    boost = (eeprom_read(86) & 0b00000011);
    lcd_int(2, 0x05, boost);
    while (PW_PB || UP_PB || DW_PB);
    while (!PW_PB || (UP_PB || DW_PB)) {
        __delay_ms(70);
        if (UP_PB == 1 && boost < 0x03) {
            boost++;
            i2c_byte_write(1, FMADD, 0x04, ((boost & 0x03) | 0b11000000), 0b10000000);
            fm_fcon_write(fcont_pp); //FM���g���Z�b�g
            lcd_int(2, 0x05, boost);
            while (PW_PB);
        }
        __delay_ms(70);
        if (DW_PB == 1 && boost > 0x00) {
            boost--;
            i2c_byte_write(1, FMADD, 0x04, ((boost & 0x03) | 0b11000000), 0b10000000);
            fm_fcon_write(fcont_pp); //FM���g���Z�b�g
            lcd_int(2, 0x05, boost);
            while (PW_PB);
        }
    }
    eeprom_write(86, boost);
    i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x44), 0);
    i2c_byte_write(0, LCDADD, 0xC0, ' ', 0);
    i2c_byte_write(0, LCDADD, 0xC0, 'O', 0);
    i2c_byte_write(0, LCDADD, 0xC0, 'K', 0);
}

fmNo_in(int fmNo, int fcont) {//�������Ă�����g������������֐� �S����32��EEPROM�ɕۑ��ł���
    int fcon, endf = 0, okf = 0;
    fcon = memory_fcont(0, fmNo, 0);
    while(!okf){
        i2c_byte_write(0, LCDADD, 0x00, 0x01, 0);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x40), 0);
        i2c_byte_write(0, LCDADD, 0xC0, ' ', 0);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x43), 0);
        i2c_byte_write(0, LCDADD, 0xC0, '.', 0);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x45), 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'M', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'H', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'z', 0);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x00), 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'N', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'o', 0);
        lcd_int(2, 0x02, fmNo);
        lcd_float(0x41, fcon);
        while (PW_PB || UP_PB || DW_PB);
        while ((!PW_PB || (UP_PB || DW_PB)) && !endf) {//�����̒��ł͑��̃{�^�������������ꂽ�Ƃ���RSSI�Ȃǂ̐ݒ�ɔ�Ԃ悤�Ƀt���O�𗧂ĂĂ���
            __delay_ms(60);
            if (UP_PB == 1) {
                if (fmNo < 32) {
                    fcon = memory_fcont(0, ++fmNo, 0);
                }
                lcd_int(2, 0x02, fmNo);
                lcd_float(0x41, fcon);
                while (UP_PB && !endf) {
                    if (DW_PB == 1 && UP_PB == 1)endf = 1;
                    if (UP_PB == 1 && PW_PB == 1)endf = 2;
                }
            }

            if (DW_PB == 1) {
                if (fmNo > 0)fcon = memory_fcont(0, --fmNo, 0);
                lcd_int(2, 0x02, fmNo);
                lcd_float(0x41, fcon);
                while (DW_PB && !endf) {
                    if (DW_PB == 1 && UP_PB == 1)endf = 1;
                    if (DW_PB == 1 && PW_PB == 1)endf = 3;
                }
            }
        }
        if(endf == 0){
            i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x41), 0);
            i2c_byte_write(0, LCDADD, 0xC0, 'O', 0);
            i2c_byte_write(0, LCDADD, 0xC0, 'K', 0);
            i2c_byte_write(0, LCDADD, 0xC0, '?', 0);
            while(1){
                if(DW_PB == 1){
                    while(DW_PB);
                    break;
                }
                if(UP_PB == 1){
                    okf = 1;
                    while(UP_PB);
                    break;
                }
            }
        }
        else if(endf > 0){
            okf = 1;
        }
    }
    okf = 0;

    if (endf == 0) {//�����I��
        memory_fcont(1, fmNo, fcont);
        fcon = memory_fcont(0, fmNo, 0);
        lcd_int(2, 0x02, fmNo);
        lcd_float(0x40, fcon);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x44), 0);
        i2c_byte_write(0, LCDADD, 0xC0, ' ', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'O', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'K', 0);
    } else if (endf == 1) {//�߂�
        i2c_byte_write(0, LCDADD, 0x00, 0x01, 0);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x02), 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'B', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'a', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'c', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'k', 0);
    } else if (endf == 2) {//BUSSBOOST�ݒ��
        i2c_byte_write(0, LCDADD, 0x00, 0x01, 0);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x00), 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'B', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'A', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'S', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'S', 0);
        buss_boost();
    } else if (endf == 3) {//臒l�ݒ��
        i2c_byte_write(0, LCDADD, 0x00, 0x01, 0);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x01), 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'S', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'N', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'R', 0);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x42), 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'S', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'N', 0);
        seek_SNR();
        __delay_ms(1000);
        i2c_byte_write(0, LCDADD, 0x00, 0x01, 0);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x00), 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'R', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'S', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'S', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'I', 0);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x42), 0);
        i2c_byte_write(0, LCDADD, 0xC0, '-', 0);
        i2c_byte_write(0, LCDADD, 0xC0, '1', 0);
        i2c_byte_write(0, LCDADD, 0xC0, '0', 0);
        i2c_byte_write(0, LCDADD, 0xC0, '0', 0);
        seek_RSSI();
    }
    __delay_ms(1000);
    i2c_byte_write(0, LCDADD, 0x00, 0x01, 0);
    start_LCD_POINT();
}

fm_serch(int sel, int fcon) {//�\�t�g�E�F�A�T�[�`�i�n�[�h�E�F�A�ɃT�[�`�I���t���O�����鎞�_�Ńn�[�h�łł��邪��񂪏��Ȃ����Ă킩��Ȃ������̂�RSSI��SNR��2��ǂ��臒l�ȏ�Ȃ�ݒ�j
    int endf = 1, memSNR, RSSI, endrssi = 0, aSERC_1= 2;
    memSNR = eeprom_read(85);
    RSSI = eeprom_read(84);
    while (!((endf > memSNR) && (endrssi > RSSI)) && (PW_PB == 0)) {
        aSERC_1++;
        if(aSERC_1 >= 2){
            aSERC_1 = 0;
            if (sel == 1) {
                if (fcon > 1100)fcon = 320;
                ++fcon;
            }
            if (sel == 2) {
                if (fcon < 320)fcon = 1100;
                --fcon;
            }
            fm_fcon_write(fcon);
            lcd_float(0x01, fcon);
        }
        __delay_ms(56);
        endf = ((i2c_byte_read(1, FMADD, 0x14) & 0b11000000) >> 6) + ((i2c_byte_read(0, FMADD, 0x14) & 0b00011111) << 2);//FMIC���猻�݂�SNR�ǂݏo��
        endrssi = ((i2c_byte_read(1, FMADD, 0x12) & 0b11111000) >> 3)*3;//FMIC���猻�݂�RSSI�ǂݏo��
    }
    i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x44), 0);
    i2c_byte_write(0, LCDADD, 0xC0, ' ', 0);
    while (PW_PB);
    return (fcon);
}

main() {
    unsigned int FMVOL = 0b00010010; //IC�{�����[�������ݒ�l
    int cnt1 = 18, fmNo = 0, c = 6000; //IC�ɂ������ݒ�l������ϐ��@���̓^�C�}�[�̏����Z�b�g�l
    int VOL_STAT = 0, PW_STAT = 0, MAIN_CON = 0, UPf = 0, DWf = 0; //���ꂼ��̃t���O
    int MAIN_STAT = 0, LIP = 0;//main��[�d�Ɋւ���t���O;
    oscsel(0); //4MHz��
    ANSEL = 0x00; //I/O�f�W�^��
    ANSELH = 0x00;
    TRISA = 0xff; //A�|�[�g����
    TRISB = 0b11110111; //A�|�[�g�ݒ�
    TRISC = 0b00011011; //C�|�[�gSAD��SCL����͂�
    TRISD = 0x00; //D�|�[�g�ݒ�
    SSPCON = 0b00101000; //I2C�̐ݒ� Fosc/(4*(SSPADD+1))
    SSPSTAT = 0b10000000; //SSPSTAT�ݒ�
    SSPADD = 0x09; // I2C�ʐM���g��100KHz for 4MHz
    T1CON = 0b01110000; //�^�C�}�[1�ݒ�
    TMR1IF = 0;
    TMR1IE = 1;
    TMR1ON = 0;
    PEIE = 1;
    GIE = 1;
    oscsel(1); //32khz��
    RD0 = 0;

    //EEPROM�̒l��ϐ��ɃZ�b�g
    FMVOL = eeprom_read(0); //�{�����[��
    cnt1 = eeprom_read(1); //�{�����[���J�E���^
    fcont_pp = (eeprom_read(2) * 10) + eeprom_read(3); //���g���J�E���^
    fmNo = eeprom_read(4); //�����i���o�[�J�E���^

    //main����
    while (1) {
        if (PW_PB == 1) {//�d���{�^���������ꂽ��d��ON
            while (PW_PB); //PW_PB
            TARCON = 1;
            oscsel(0); //4MHz?
            __delay_ms(100);
            MAIN_STAT = 1;
            lcd_RS(); //LCD���Z�b�g
            fmIC_RS(); //FMIC���Z�b�g
            fmIC_VOL(FMVOL); //FMIC�{�����[���\��
            lcd_int(2, 0x46, cnt1); //���݃{�����[���l�\��
            lcd_int(2, 0x42, fmNo); //���g�������l�\��
            lcd_float(0x01, fcont_pp); //���ݎ��g���\��
            start_LCD_POINT(); //LCD�ɂ��̑��̌Œ蕶���\��
            fm_fcon_write(fcont_pp); //FM���g���Z�b�g

            while(!MAIN_CON){//main���[�v
                while(!(UP_PB || DW_PB || PW_PB)){//�{�^�����������܂ő҂�
                    oscsel(3);//�N���b�N��500KHz�ցAI2C�����x��������
                    SSPADD = 0x00;//I2C�̑��x���グ��
                    if(0b00000011 == (i2c_byte_read(0,FMADD,0x12) & 0b00000011)){//FMIC����X�e�[�^�X���[�h
                        i2c_byte_write(0,LCDADD,0x00,(0x80 + 0x00),0);
                        i2c_byte_write(0,LCDADD,0xC0,'S',0);//�X�e���I�Ȃ�S��\��
                    }
                    else{
                        i2c_byte_write(0,LCDADD,0x00,(0x80 + 0x00),0);
                        i2c_byte_write(0,LCDADD,0xC0,'M',0);//���m�����Ȃ�M��\��
                    }
                    oscsel(1);//32khz
                    SSPADD = 0x09;//I2C���x�Ē�`
                    __low_delay_ms(30);//0.03�b���Ď�
                }
                oscsel(2);//1MHz��
                TMR1H = (c >> 8) ;
                TMR1L = (c & 0x00ff) ;
                TMR1ON_Flag = 1 ;
                TMR1ON = 1 ;
                UPf = 0;
                while((UP_PB = 1) && (UPf < 2)){//��莞��UP�{�^���������ꂽ��PW�{�^���Ɖ����ꂽ���m�F
                    if((UP_PB == 1) && (TMR1ON_Flag == 0)){//��莞�Ԃ�����
                        oscsel(0);
                        i2c_byte_write(0,LCDADD,0x00,(0x80 + 0x44),0);
                        i2c_byte_write(0,LCDADD,0xC0,0b01111110,0);//���g�����C���N�������g���̕\��
                        UPf = 3;
                        VOL_STAT = 1;
                        fcont_pp = fm_serch(1,fcont_pp);//�C���N�������g�T�[�`��
                        oscsel(1);
                        while(UP_PB);
                    }
                    if((UP_PB == 1) && (TMR1ON_Flag == 1))UPf = 1;//���Ԍv�����Ƀ{�^����������Ă�����t���O�Z�b�g
                    if((UP_PB == 1) && (PW_PB == 1))UPf = 2;//PW�{�^���Ɠ����������ꂽ
                }
                TMR1ON = 0 ;
                oscsel(1);
                if((UPf > 0) && (UPf < 3) && (VOL_STAT == 0)){//PW�{�^���Ɠ����������ꂽ�̂ō��I�����Ă�����g����������C���N�������g�����i���o�[�̎��g����I��
                    if(UPf == 2){
                        if(fmNo < 32)fmNo++;
                        oscsel(0);
                        lcd_int(2,0x42,fmNo);
                        fcont_pp = memory_fcont(0,fmNo,0);
                        fm_fcon_write(memory_fcont(0,fmNo,0));
                        lcd_float(0x01,memory_fcont(0,fmNo,0));
                        VOL_STAT = 1;
                        oscsel(1);
                        while(UP_PB || PW_PB);
                    }

                    while(UP_PB);

                    if((FMVOL<0b00011111) && (VOL_STAT==0) && (UPf == 1)){//���ȏ㉟���ꂸ�A���̃{�^���Ɖ�����Ȃ������̂Ń{�����[���A�b�v
                        oscsel(0);
                        FMVOL = FMVOL + 0x01;
                        cnt1++;
                        fmIC_VOL(FMVOL);
                        lcd_int(2,0x46,cnt1);
                    }
                    oscsel(1);
                    VOL_STAT=0;
                }
                VOL_STAT=0;

                oscsel(2);
                TMR1H = (c >> 8) ;
                TMR1L = (c & 0x00ff) ;
                TMR1ON_Flag = 1 ;
                DWf = 0;
                TMR1ON = 1 ;
                while((DW_PB = 1) && (DWf < 2)){//��莞��DW�{�^���������ꂽ��PW�{�^���Ɖ����ꂽ���m�F
                    if((DW_PB == 1) && (TMR1ON_Flag == 0)){//��莞�Ԃ�����
                        oscsel(0);
                        i2c_byte_write(0,LCDADD,0x00,(0x80 + 0x44),0);
                        i2c_byte_write(0,LCDADD,0xC0,0b01111111,0);//���g�����f�N�������g���̕\��
                        DWf = 3;
                        VOL_STAT = 1;
                        fcont_pp = fm_serch(2,fcont_pp);//�f�N�������g�T�[�`��
                        oscsel(1);
                        while(DW_PB);
                    }
                    if((DW_PB == 1) && (TMR1ON_Flag == 1))DWf = 1;//���Ԍv�����Ƀ{�^����������Ă�����t���O�Z�b�g
                    if((DW_PB == 1) && (PW_PB == 1))DWf = 2;//PW�{�^���Ɠ����������ꂽ
                }
                TMR1ON = 0 ;
                oscsel(1);
                if((DWf > 0) && (DWf < 3) && (VOL_STAT == 0)){//PW�{�^���Ɠ����������ꂽ�̂ō��I�����Ă�����g����������f�N�������g�����i���o�[�̎��g����I��
                    if(DWf == 2){
                        if(fmNo > 0)fmNo--;
                        oscsel(0);
                        lcd_int(2,0x42,fmNo);
                        fcont_pp = memory_fcont(0,fmNo,0);
                        fm_fcon_write(memory_fcont(0,fmNo,0));
                        lcd_float(0x01,memory_fcont(0,fmNo,0));
                        oscsel(1);
                        VOL_STAT = 1;
                        while(DW_PB || PW_PB);
                    }
                    while(DW_PB);
                    if((FMVOL>0b00000000) && (VOL_STAT==0) && (DWf == 1)){//���ȏ㉟���ꂸ�A���̃{�^���Ɖ�����Ȃ������̂Ń{�����[���_�E��
                        oscsel(0);
                        FMVOL = FMVOL - 0x01;
                        cnt1--;
                        fmIC_VOL(FMVOL);
                        lcd_int(2,0x46,cnt1);
                    }
                    oscsel(1);
                    VOL_STAT=0;
                }
                VOL_STAT=0;
                oscsel(1);

                __low_delay_ms(100);
                if(PW_PB == 1 && UP_PB == 0 && DW_PB == 0){//PW�{�^�����������ꂽ��
                    oscsel(2);
                    TMR1H = (c >> 8);
                    TMR1L = (c & 0x00ff);
                    TMR1ON_Flag = 1 ;
                    TMR1ON = 1 ;
                    while(PW_PB){//��莞��PW�{�^���������ꂽ��PU,DW�{�^���Ɖ����ꂽ���m�F
                        if(PW_PB == 1 && TMR1ON_Flag == 0){//��莞�Ԃ�����
                            oscsel(0);
                            PW_STAT = 1;
                            fmNo_in(fmNo,fcont_pp);//���g�������@�\���ݒ�֐���
                            lcd_int(2,0x46,cnt1);//���ꂼ��l���ς���Ă���̂ōX�V
                            lcd_int(2,0x42,fmNo);
                            lcd_float(0x01,fcont_pp);
                            oscsel(1);
                        }
                        if(PW_PB == 1 && TMR1ON_Flag == 1)MAIN_CON = 1;//PW�{�^�����^�C�}�[�J�E���g���ɉ����ꂽ�̂œd��OFF�t���O�𗧂Ă�
                        if((DW_PB == 1 || UP_PB == 1) && PW_PB == 1)break;//���̃{�^���Ɖ����ꂽ�̂ŏ�̋@�\�ƔF���A���[�v���o��
                    }
                    TMR1ON = 0 ;
                    oscsel(1);
                    if(DW_PB == 1 || UP_PB == 1 || PW_STAT == 1){//�ꉞ���̃{�^���Ɖ�����ĂȂ����m�F
                        MAIN_CON = 0;
                        PW_STAT = 0;
                    }
                }
                oscsel(0);//4Mhz
                lcd_int(2,0x46,cnt1);//LCD�{�����[���l�X�V
                lcd_int(2,0x42,fmNo);//LCD���g�������l�l�X�V
                lcd_float(0x01,fcont_pp);//LCD���g���J�E���g�l�X�V
                oscsel(1);//32khz
            }
            /*main�I��*/

            //���Ӌ@��OFF�@���@�t���O���Z�b�g
            oscsel(0); //4Mhz
            FMIC_EN_PIN = 0;
            LCD_RS_PIN = 0;
            MAIN_CON = 0;
            LIP = 0;
            TARCON = 0;


            if (MAIN_STAT == 1) {
                //EEPROM�֏�������
                eeprom_write(0, FMVOL);
                eeprom_write(1, cnt1);
                eeprom_write(2, (fcont_pp / 10));
                eeprom_write(3, (fcont_pp % 10));
                eeprom_write(4, fmNo);
                MAIN_STAT = 0;
		TARCON = 0;
                while (PW_PB);
            }
            oscsel(1);
        }
        if (LIP_PW || LIP_UP) {//�[�d�����H
                if(LIP == 0){
		TARCON = 1;
                    oscsel(0); //4MHz
                    LIP = 1;
                    lcd_RS(); //LCD���Z�b�g
                    LED1 = 1;
                }
                if (LIP_UP == 1) {//�[�d���̓��b�Z�[�W��\��
                    oscsel(0);
                    i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x00), 0);
                    i2c_byte_write(0, LCDADD, 0xC0,'(', 0);
                    i2c_byte_write(0, LCDADD, 0xC0, 0b00111110, 0);
                    i2c_byte_write(0, LCDADD, 0xC0, 0b00111100, 0);
                    i2c_byte_write(0, LCDADD, 0xC0,')', 0);
                    i2c_byte_write(0, LCDADD, 0xC0,'/', 0);
                    i2c_byte_write(0, LCDADD, 0xC0,0b10110001, 0);
                    i2c_byte_write(0, LCDADD, 0xC0,0b10111001, 0);
                    i2c_byte_write(0, LCDADD, 0xC0,0b11011110, 0);
                    oscsel(1);
                }
            }
        else {//�����Ȃ���΃t���O���Z�b�g�@���@�{�^���m�F�փ��[�v
                oscsel(0);
                LED1 = 0;
                FMIC_EN_PIN = 0;
                LCD_RS_PIN = 0;
                LIP = 0;
		TARCON = 0;
                oscsel(1);
            }
    }
}


