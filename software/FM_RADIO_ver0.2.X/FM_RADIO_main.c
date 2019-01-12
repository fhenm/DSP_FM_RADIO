/*
 * File:   newmain.c
 * Author: msk319882
 *
 * Created on 2017/01/18, 22:51
 * 修正 2017/1/24, 20:35
 *
 * 適当に書いてるので間違いやバグがあるかもしれない（）
 *
 * 詰め込めるだけ詰め込んだFMラジオ
 * 
 * 機能：ソフトウェアサーチ、RSSI&SNR閾値設定とカウンタ、BUSSBOOSTセレクト、受信周波数を32個まで記憶できる周波数メモ機能
 * 充電機能（4.1v監視はハードで実装）、I2C関数実装、EEPROMの前回の起動設定を書き込むことにより次回移動時に前回起動時の状態になる
 * 小型LCDによる表示、小型LCDの電源制御
 *
 * PIC16f887を使ったためPIN数がめちゃくちゃ余っていてもったいないのが難点
 *
 * 待機電流 0.1mA 起動中電流 23~27mA
 */

//HI-TECHのヘッダをインクルード
#include <pic.h>
#include <pic16f887.h>

/*ディレイ定義*/
#define _XTAL_FREQ 4000000
#define  _XTAL_FREQ_LOW 32000
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define __low_delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ_LOW/4000.0)))
#define __low_delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ_LOW/4000000.0)))


/*外部PIN定義*/
#define LED1 RB3    //LEDへ
#define LIP_UP RC1  //リチュウムが充電終了したらlow
#define LIP_PW RA6  //リチュウム充電回路が動作中HI
#define SDA RC4     //SDA
#define SCL RC3     //SCL
#define LCD_RS_PIN RC5  //LCDリセットピンへ
#define FMIC_EN_PIN RC6     //FMICのENピンへ
#define TARCON RD4	//レギュレータコントロール
#define UP_PB RA1   //アップボタン
#define PW_PB RA2   //パワーボタン
#define DW_PB RA3   //ダウンボタン
#define LCDADD 0x7C     //LCDアドレス
#define FMADD 0b01101010    //FMICアドレス
int TMR1ON_Flag = 0; //タイマーフラグ
int fcont_pp = 765;//周波数カウンタ


//EEPROM初期データ
//ここにはプリセットとして高山で受信できそうな周波数を入れておく
//また、前回起動時のボリューム、周波数、周波数メモカウンタ値、BUSSBOOST設定値、RSSI、SNR閾値が入る
//電源を切っても残るようにEEPROMに入れる

//ボリュームや初期周波数が入る
__EEPROM_DATA(15, 15, 76, 5, 0, 0, 0, 0);
//ここからが周波数メモ　周波数値
__EEPROM_DATA(76, 80, 86, 80, 83, 78, 78, 81);//ここからが上2桁
__EEPROM_DATA(79, 82, 84, 85, 80, 77, 79, 78);
__EEPROM_DATA(0, 0, 0, 0, 0, 0, 0, 0);
__EEPROM_DATA(0, 0, 0, 0, 0, 0, 0, 0);
__EEPROM_DATA(5, 0, 1, 3, 6, 5, 3, 5);//ここからがした1桁
__EEPROM_DATA(3, 1, 3, 6, 7, 8, 5, 9);
__EEPROM_DATA(0, 0, 0, 0, 0, 0, 0, 0);
__EEPROM_DATA(0, 0, 0, 0, 0, 0, 0, 0);
__EEPROM_DATA(0, 0, 0, 0, 0, 0, 0, 0);
//RSSI、SNR、BUSSBOOST設定値が入る
__EEPROM_DATA(0, 0, 0, 48, 9, 46, 1, 0);


//コンフィグ設定
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

void interrupt InterTimer(){//割り込み関数
    if (TMR1IF == 1) {  // タイマー1のフラグがあるか？
        TMR1ON_Flag = 0 ;// タイマーフラグリセット
        TMR1IF = 0 ;
    }
}

oscsel(int sel1) {//内部オシレートセレクト
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

memory_fcont(int sel, int add, int fcont) { //周波数EEPROM書き込み　読み込み　関数
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

i2c_Tx(char date) {//I2C1byte書き込み
    LED1 = 1;
    SSPIF = 0;
    SSPBUF = date;
    while (BF);
    while (!SSPIF);
    LED1 = 0;
}

i2c_byte_write(int sel, char add, char cmd, char date1, char date2) {//I2C書き込み
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

i2c_byte_read(unsigned int sel, char add, char regadd) {//I2Cリード関数
    char read_data[2];
    SSPADD = 0x09; //I2C通信速度再定義
    SEN = 1; //スタートコンデションセット
    while (SEN); //送信完了まで待ち
    i2c_Tx(add); //ICアドレス送信
    i2c_Tx(regadd); //レジスタアドレス送信
    RSEN = 1; //リスタートコンディションセット
    while (RSEN); //送信完了まで待ち
    i2c_Tx(add + 0x01); //ICアドレスをリードモードで書き込み
    RCEN = 1; //マスタをリードモードへ
    while (RCEN); //送信完了まで待ち
    read_data[0] = SSPBUF; //バッファのデータを待避
    ACKDT = 0; //ACKをセット
    ACKEN = 1; //ACKを送信
    while (ACKEN); //送信完了まで待ち
    RCEN = 1; //マスタをリードモードへ
    while (RCEN); //送信完了まで待ち
    read_data[1] = SSPBUF; //バッファのデータを待避
    ACKDT = 1; //NO-ACKをセット
    ACKEN = 1; //NO-ACKを送信
    while (ACKEN); //送信完了まで待ち
    PEN = 1; //ストップコンデションセット
    while (PEN);//送信完了まで待ち
    SSPADD = 0x09; //I2C速度再定義
    return (read_data[sel]);
}

lcd_RS() { //LCD初期化関数
    LCD_RS_PIN = 0;//LCDのハードウェアリセット
    __delay_ms(100);
    LCD_RS_PIN = 1;
    __delay_ms(100);
    i2c_byte_write(0, LCDADD, 0x00, 0x38, 0);//初期設定送信
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

fmIC_RS() {//FMIC初期化関数
    FMIC_EN_PIN = 0;//FMICハードウェアリセット
    __delay_ms(100);
    FMIC_EN_PIN = 1;
    __delay_ms(500);
    i2c_byte_write(1, FMADD, 0x10, 0b01100000, 0x00);
    i2c_byte_write(1, FMADD, 0x3f, 0x00, 0b00010011);
    i2c_byte_write(1, FMADD, 0x02, 0x00, 0b00000111); //FMミュートを解除
    i2c_byte_write(1, FMADD, 0x04, ((eeprom_read(86) & 0b00000011) | 0b11000000), 0b10000000); //BUSSBOOSTの値をEEPROMからFMICに書き込み
    i2c_byte_write(1, FMADD, 0x16, 0b00001001, 0b00000010);//38kHzに設定
    //i2c_byte_write(1, FMADD, 0x16, 0b00000000, 0b00000010);//32.768kHzに設定
    i2c_byte_write(1, FMADD, 0x0a, 0b00000100, 0x00); //AFC設定
    i2c_byte_write(1, FMADD, 0x0c, 0x00, 0b00101100); //帯域を64から32へ
}

fmIC_VOL(char vol) {//ボリューム関数 volに入れた値をFMICにボリューム設定値として送信
    i2c_byte_write(1, FMADD, 0x0F, 0b00000000, (vol & 0b00011111));
}

lcd_int(int sel, unsigned int charadd, unsigned int cnt1) {//LCDに数字を表示する関数　1で1桁2で2桁 DDRAMアドレス int表示する値
    int LCD_2, LCD_1;
    LCD_1 = cnt1 % 10;
    LCD_2 = cnt1 / 10;
    LCD_1 = (LCD_1 & 0x0f) | 0x30;
    LCD_2 = (LCD_2 & 0x0f) | 0x30;
    i2c_byte_write(0, LCDADD, 0x00, (0x80 + charadd), 0);
    i2c_byte_write(0, LCDADD, 0xC0, LCD_2, 0);
    if (sel == 2)i2c_byte_write(0, LCDADD, 0xC0, LCD_1, 0);
}

lcd_float(unsigned int charadd, unsigned int cnt1) {//3桁の値を下一桁を少数点第一位として表示する関数　DDRAMアドレス int3表示する数字
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

start_LCD_POINT() {//起動時にLCDに表示する文字セット関数
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

fm_fcon_write(int fconv) {//3桁の数字を変換してFMICに周波数として書き込み　例：765 → 76.5Mhz
    i2c_byte_write(1, FMADD, 0x03, ((((fconv << 1) & 0xff00) + 0x0000) >> 8), (fconv << 1) & 0x00ff);
    i2c_byte_write(1, FMADD, 0x03, ((((fconv << 1) & 0xff00) + 0x8000) >> 8), (fconv << 1) & 0x00ff);
}
seek_RSSI() {//RSSI閾値設定関数
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

seek_SNR() {//SNR閾値設定関数
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

buss_boost() {//BUSSBOOST設定関数
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
            fm_fcon_write(fcont_pp); //FM周波数セット
            lcd_int(2, 0x05, boost);
            while (PW_PB);
        }
        __delay_ms(70);
        if (DW_PB == 1 && boost > 0x00) {
            boost--;
            i2c_byte_write(1, FMADD, 0x04, ((boost & 0x03) | 0b11000000), 0b10000000);
            fm_fcon_write(fcont_pp); //FM周波数セット
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

fmNo_in(int fmNo, int fcont) {//今聞いている周波数をメモする関数 全部で32個EEPROMに保存できる
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
        while ((!PW_PB || (UP_PB || DW_PB)) && !endf) {//ここの中では他のボタンが同時押されたときにRSSIなどの設定に飛ぶようにフラグを立てている
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

    if (endf == 0) {//メモ終了
        memory_fcont(1, fmNo, fcont);
        fcon = memory_fcont(0, fmNo, 0);
        lcd_int(2, 0x02, fmNo);
        lcd_float(0x40, fcon);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x44), 0);
        i2c_byte_write(0, LCDADD, 0xC0, ' ', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'O', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'K', 0);
    } else if (endf == 1) {//戻る
        i2c_byte_write(0, LCDADD, 0x00, 0x01, 0);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x02), 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'B', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'a', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'c', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'k', 0);
    } else if (endf == 2) {//BUSSBOOST設定へ
        i2c_byte_write(0, LCDADD, 0x00, 0x01, 0);
        i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x00), 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'B', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'A', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'S', 0);
        i2c_byte_write(0, LCDADD, 0xC0, 'S', 0);
        buss_boost();
    } else if (endf == 3) {//閾値設定へ
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

fm_serch(int sel, int fcon) {//ソフトウェアサーチ（ハードウェアにサーチ終了フラグがある時点でハードでできるが情報が少なすぎてわからなかったのでRSSIとSNRを2回読んで閾値以上なら設定）
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
        endf = ((i2c_byte_read(1, FMADD, 0x14) & 0b11000000) >> 6) + ((i2c_byte_read(0, FMADD, 0x14) & 0b00011111) << 2);//FMICから現在のSNR読み出し
        endrssi = ((i2c_byte_read(1, FMADD, 0x12) & 0b11111000) >> 3)*3;//FMICから現在のRSSI読み出し
    }
    i2c_byte_write(0, LCDADD, 0x00, (0x80 + 0x44), 0);
    i2c_byte_write(0, LCDADD, 0xC0, ' ', 0);
    while (PW_PB);
    return (fcon);
}

main() {
    unsigned int FMVOL = 0b00010010; //ICボリューム初期設定値
    int cnt1 = 18, fmNo = 0, c = 6000; //ICにかかわる設定値が入る変数　ｃはタイマーの初期セット値
    int VOL_STAT = 0, PW_STAT = 0, MAIN_CON = 0, UPf = 0, DWf = 0; //それぞれのフラグ
    int MAIN_STAT = 0, LIP = 0;//mainや充電に関するフラグ;
    oscsel(0); //4MHzへ
    ANSEL = 0x00; //I/Oデジタル
    ANSELH = 0x00;
    TRISA = 0xff; //Aポート入力
    TRISB = 0b11110111; //Aポート設定
    TRISC = 0b00011011; //CポートSADとSCLを入力に
    TRISD = 0x00; //Dポート設定
    SSPCON = 0b00101000; //I2Cの設定 Fosc/(4*(SSPADD+1))
    SSPSTAT = 0b10000000; //SSPSTAT設定
    SSPADD = 0x09; // I2C通信周波数100KHz for 4MHz
    T1CON = 0b01110000; //タイマー1設定
    TMR1IF = 0;
    TMR1IE = 1;
    TMR1ON = 0;
    PEIE = 1;
    GIE = 1;
    oscsel(1); //32khzへ
    RD0 = 0;

    //EEPROMの値を変数にセット
    FMVOL = eeprom_read(0); //ボリューム
    cnt1 = eeprom_read(1); //ボリュームカウンタ
    fcont_pp = (eeprom_read(2) * 10) + eeprom_read(3); //周波数カウンタ
    fmNo = eeprom_read(4); //メモナンバーカウンタ

    //main処理
    while (1) {
        if (PW_PB == 1) {//電源ボタンが押されたら電源ON
            while (PW_PB); //PW_PB
            TARCON = 1;
            oscsel(0); //4MHz?
            __delay_ms(100);
            MAIN_STAT = 1;
            lcd_RS(); //LCDリセット
            fmIC_RS(); //FMICリセット
            fmIC_VOL(FMVOL); //FMICボリューム表示
            lcd_int(2, 0x46, cnt1); //現在ボリューム値表示
            lcd_int(2, 0x42, fmNo); //周波数メモ値表示
            lcd_float(0x01, fcont_pp); //現在周波数表示
            start_LCD_POINT(); //LCDにその他の固定文字表示
            fm_fcon_write(fcont_pp); //FM周波数セット

            while(!MAIN_CON){//mainループ
                while(!(UP_PB || DW_PB || PW_PB)){//ボタンが押されるまで待ち
                    oscsel(3);//クロックを500KHzへ、I2Cも速度が落ちる
                    SSPADD = 0x00;//I2Cの速度を上げる
                    if(0b00000011 == (i2c_byte_read(0,FMADD,0x12) & 0b00000011)){//FMICからステータスリード
                        i2c_byte_write(0,LCDADD,0x00,(0x80 + 0x00),0);
                        i2c_byte_write(0,LCDADD,0xC0,'S',0);//ステレオならSを表示
                    }
                    else{
                        i2c_byte_write(0,LCDADD,0x00,(0x80 + 0x00),0);
                        i2c_byte_write(0,LCDADD,0xC0,'M',0);//モノラルならMを表示
                    }
                    oscsel(1);//32khz
                    SSPADD = 0x09;//I2C速度再定義
                    __low_delay_ms(30);//0.03秒ずつ監視
                }
                oscsel(2);//1MHzへ
                TMR1H = (c >> 8) ;
                TMR1L = (c & 0x00ff) ;
                TMR1ON_Flag = 1 ;
                TMR1ON = 1 ;
                UPf = 0;
                while((UP_PB = 1) && (UPf < 2)){//一定時間UPボタンが押されたかPWボタンと押されたか確認
                    if((UP_PB == 1) && (TMR1ON_Flag == 0)){//一定時間たった
                        oscsel(0);
                        i2c_byte_write(0,LCDADD,0x00,(0x80 + 0x44),0);
                        i2c_byte_write(0,LCDADD,0xC0,0b01111110,0);//周波数をインクリメント中の表示
                        UPf = 3;
                        VOL_STAT = 1;
                        fcont_pp = fm_serch(1,fcont_pp);//インクリメントサーチへ
                        oscsel(1);
                        while(UP_PB);
                    }
                    if((UP_PB == 1) && (TMR1ON_Flag == 1))UPf = 1;//時間計測中にボタンが押されていたらフラグセット
                    if((UP_PB == 1) && (PW_PB == 1))UPf = 2;//PWボタンと同時押しされた
                }
                TMR1ON = 0 ;
                oscsel(1);
                if((UPf > 0) && (UPf < 3) && (VOL_STAT == 0)){//PWボタンと同時押しされたので今選択している周波数メモからインクリメントしたナンバーの周波数を選択
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

                    if((FMVOL<0b00011111) && (VOL_STAT==0) && (UPf == 1)){//一定以上押されず、他のボタンと押されなかったのでボリュームアップ
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
                while((DW_PB = 1) && (DWf < 2)){//一定時間DWボタンが押されたかPWボタンと押されたか確認
                    if((DW_PB == 1) && (TMR1ON_Flag == 0)){//一定時間たった
                        oscsel(0);
                        i2c_byte_write(0,LCDADD,0x00,(0x80 + 0x44),0);
                        i2c_byte_write(0,LCDADD,0xC0,0b01111111,0);//周波数をデクリメント中の表示
                        DWf = 3;
                        VOL_STAT = 1;
                        fcont_pp = fm_serch(2,fcont_pp);//デクリメントサーチへ
                        oscsel(1);
                        while(DW_PB);
                    }
                    if((DW_PB == 1) && (TMR1ON_Flag == 1))DWf = 1;//時間計測中にボタンが押されていたらフラグセット
                    if((DW_PB == 1) && (PW_PB == 1))DWf = 2;//PWボタンと同時押しされた
                }
                TMR1ON = 0 ;
                oscsel(1);
                if((DWf > 0) && (DWf < 3) && (VOL_STAT == 0)){//PWボタンと同時押しされたので今選択している周波数メモからデクリメントしたナンバーの周波数を選択
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
                    if((FMVOL>0b00000000) && (VOL_STAT==0) && (DWf == 1)){//一定以上押されず、他のボタンと押されなかったのでボリュームダウン
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
                if(PW_PB == 1 && UP_PB == 0 && DW_PB == 0){//PWボタンだけ押されたら
                    oscsel(2);
                    TMR1H = (c >> 8);
                    TMR1L = (c & 0x00ff);
                    TMR1ON_Flag = 1 ;
                    TMR1ON = 1 ;
                    while(PW_PB){//一定時間PWボタンが押されたかPU,DWボタンと押されたか確認
                        if(PW_PB == 1 && TMR1ON_Flag == 0){//一定時間たった
                            oscsel(0);
                            PW_STAT = 1;
                            fmNo_in(fmNo,fcont_pp);//周波数メモ機能＆設定関数へ
                            lcd_int(2,0x46,cnt1);//それぞれ値が変わっているので更新
                            lcd_int(2,0x42,fmNo);
                            lcd_float(0x01,fcont_pp);
                            oscsel(1);
                        }
                        if(PW_PB == 1 && TMR1ON_Flag == 1)MAIN_CON = 1;//PWボタンがタイマーカウント中に押されたので電源OFFフラグを立てる
                        if((DW_PB == 1 || UP_PB == 1) && PW_PB == 1)break;//他のボタンと押されたので上の機能と認識、ループを出る
                    }
                    TMR1ON = 0 ;
                    oscsel(1);
                    if(DW_PB == 1 || UP_PB == 1 || PW_STAT == 1){//一応他のボタンと押されてないか確認
                        MAIN_CON = 0;
                        PW_STAT = 0;
                    }
                }
                oscsel(0);//4Mhz
                lcd_int(2,0x46,cnt1);//LCDボリューム値更新
                lcd_int(2,0x42,fmNo);//LCD周波数メモ値値更新
                lcd_float(0x01,fcont_pp);//LCD周波数カウント値更新
                oscsel(1);//32khz
            }
            /*main終了*/

            //周辺機器OFF　＆　フラグリセット
            oscsel(0); //4Mhz
            FMIC_EN_PIN = 0;
            LCD_RS_PIN = 0;
            MAIN_CON = 0;
            LIP = 0;
            TARCON = 0;


            if (MAIN_STAT == 1) {
                //EEPROMへ書き込み
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
        if (LIP_PW || LIP_UP) {//充電中か？
                if(LIP == 0){
		TARCON = 1;
                    oscsel(0); //4MHz
                    LIP = 1;
                    lcd_RS(); //LCDリセット
                    LED1 = 1;
                }
                if (LIP_UP == 1) {//充電中はメッセージを表示
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
        else {//何もなければフラグリセット　＆　ボタン確認へループ
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


