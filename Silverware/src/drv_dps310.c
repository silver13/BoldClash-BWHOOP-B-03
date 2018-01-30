/*
The MIT License (MIT)

Copyright (c) 2016 silverx

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "drv_dps310.h"
#include "drv_i2c.h"
#include "drv_time.h"
#include "config.h"
#include "hardware.h"
#include "binary.h"
#include "util.h"
#include "control.h"
#include <math.h>

#define DPS310_PSR 0x00
#define DPS310_TMP 0x03
#define DPS310_PSR_CFG 0x06
#define DPS310_TMP_CFG 0x07
#define DPS310_MEAS_CFG 0x08
#define DPS310_CFG_REG 0x09
#define DPS310_INT 0x0a
#define DPS310_FIFO 0x0b
#define DPS310_RESET 0x0c
#define DPS310_PRDID 0x0d
#define DPS310_COEF 0x10
#define DPS310_COEF_SRCE 0x28
#define DPS310_ID 0x10

#define T_RDY B00100000
#define P_RDY B00010000

// Select Temperature sensor oversampling rate
// #define TEMP_FACTOR  524288.0
// #define TEMP_FACTOR 1572864.0
// #define TEMP_FACTOR 3670016.0
// #define TEMP_FACTOR 7864320.0
#define TEMP_FACTOR  253952.0
// #define TEMP_FACTOR  516096.0
// #define TEMP_FACTOR 1040384.0
// #define TEMP_FACTOR 2088960.0


// Select Temperature sensor LPF rate
#define TEMP_LPF     0.75
// #define TEMP_LPF    0.875
// #define TEMP_LPF   0.9375
// #define TEMP_LPF  0.96875
// #define TEMP_LPF 0.984375

// Select Pressure sensor oversampling rate
// #define PRESS_FACTOR  524288.0
// #define PRESS_FACTOR 1572864.0
// #define PRESS_FACTOR 3670016.0
// #define PRESS_FACTOR 7864320.0
#define PRESS_FACTOR 253952.0
// #define PRESS_FACTOR  516096.0
// #define PRESS_FACTOR 1040384.0
// #define PRESS_FACTOR 2088960.0

// Select Pressure sensor LPF rate
#define PRESS_LPF     0.75
// #define PRESS_LPF    0.875
// #define PRESS_LPF   0.9375
// #define PRESS_LPF  0.96875
// #define PRESS_LPF 0.984375

//      199    100, -259 79830 -51103 -2647 1325 -7843 -102 -788
int32_t  c0, c0Half,  c1,  c00,   c10,  c01, c11,  c20, c21, c30;
float press_raw_sc, temp_raw_sc, press_fl, temp_fl;

void dps310_init(void)
{
    int press_raw, temp_raw = 0;
    int data[3];

    // send reset command
    i2c_writereg( DPS310_I2C_ADDRESS , DPS310_RESET , B10001001 );
    delay(60000); //60ms to warm up

    // blocking - wait for registers
    do
        delay(1000);
    while (!(i2c_readreg(DPS310_I2C_ADDRESS, DPS310_MEAS_CFG)&B01000000));

    // blocking wait for coefficients to be ready
    do
        delay(1000);
    while (!(i2c_readreg(DPS310_I2C_ADDRESS, DPS310_MEAS_CFG)&B10000000));

    dps310_readcoeffs();

    // Forcing temp_sensor_type to B10000000 results in realistic T readings,
    // int temp_sensor_type = B10000000;
    int temp_sensor_type = i2c_readreg(DPS310_I2C_ADDRESS, DPS310_COEF_SRCE)&B10000000;

    // temp config
    i2c_writereg(DPS310_I2C_ADDRESS, DPS310_TMP_CFG, B00000100|temp_sensor_type); // 0 meas/sec / 16x oversampling

    // pressure config
    i2c_writereg(DPS310_I2C_ADDRESS, DPS310_PSR_CFG, B00000100); // 0 meas/sec | 16x oversampling

    // set CFG_REG
//     i2c_writereg(DPS310_I2C_ADDRESS, DPS310_CFG_REG, B00000000); // Disable T_SHIFT, P_SHIFT (1,2,4,8)
    i2c_writereg(DPS310_I2C_ADDRESS, DPS310_CFG_REG, B00001100); // Ensable T_SHIFT, P_SHIFT (16,32,64,128)

    // Prime LPF with valid P&T values
    dps310_prime();
}

int dps310_check(void)
{
    // Get product ID
    int id = i2c_readreg(DPS310_I2C_ADDRESS, DPS310_PRDID);
    return (DPS310_ID==id);
}

void dps310_read_pressure(void)
{
    int32_t press_raw, temp_raw = 0;
    static float temp_raw_sc_new, press_raw_sc_new;
    int rdy = 0;
    int data[3];

    // Check if any sample is ready
    rdy = i2c_readreg(DPS310_I2C_ADDRESS, DPS310_MEAS_CFG);

    if (rdy & T_RDY) { // New T sample ready?
        // read out new temp_raw
        i2c_readdata(DPS310_I2C_ADDRESS, DPS310_TMP, data, 3);
        temp_raw = ((data[0]<<16 | data[1]<<8 | data[2])<<8)>>8;
        temp_raw_sc_new  = temp_raw / TEMP_FACTOR;

        // Request new P sample
        i2c_writereg(DPS310_I2C_ADDRESS, DPS310_MEAS_CFG, B00000001);

    } else if (rdy & P_RDY) { // New P sample ready?
        // read out new press_raw
        i2c_readdata(DPS310_I2C_ADDRESS, DPS310_PSR, data, 3);
        press_raw = ((data[0]<<16 | data[1]<<8 | data[2])<<8)>>8;
        press_raw_sc_new =  press_raw / PRESS_FACTOR;

        // Request new T sample
        i2c_writereg(DPS310_I2C_ADDRESS, DPS310_MEAS_CFG, B00000010);
    }

    // Low-pass filter (ease in) raw values, even if we don't have new measurement within looptime
//     lpf(&temp_raw_sc, temp_raw_sc_new, TEMP_LPF);
    temp_raw_sc = temp_raw_sc_new;
//     lpf(&press_raw_sc, press_raw_sc_new, PRESS_LPF);
    press_raw_sc = press_raw_sc_new;
}

void dps310_prime(void)
{
    int data[3];

    // Request next T sample
    i2c_writereg(DPS310_I2C_ADDRESS, DPS310_MEAS_CFG, B00000010); // New T sample

    while (!(i2c_readreg(DPS310_I2C_ADDRESS, DPS310_MEAS_CFG) & T_RDY)) {
        delay(1000);
    }

    // read out new temp_raw
    i2c_readdata(DPS310_I2C_ADDRESS, DPS310_TMP, data, 3);
    int32_t temp_raw = ((data[0]<<16 | data[1]<<8 | data[2])<<8)>>8;
    temp_raw_sc = temp_raw / TEMP_FACTOR;

    // Request new P sample
    i2c_writereg(DPS310_I2C_ADDRESS, DPS310_MEAS_CFG, B00000001); // New P sample

    while (!(i2c_readreg(DPS310_I2C_ADDRESS, DPS310_MEAS_CFG) & P_RDY)) {
        delay(1000);
    }

    // read out new press_raw
    i2c_readdata(DPS310_I2C_ADDRESS, DPS310_PSR, data, 3);
    int32_t press_raw = ((data[0]<<16 | data[1]<<8 | data[2])<<8)>>8;
    press_raw_sc = press_raw / PRESS_FACTOR;

    // Request next T sample
    i2c_writereg(DPS310_I2C_ADDRESS, DPS310_MEAS_CFG, B00000010); // New T sample
}

void dps310_tcomp_lpf(void) {
    temp_fl = c0Half + c1 * temp_raw_sc;
}

void dps310_pcomp_lpf(void) {
    press_fl = c00 + press_raw_sc * (c10 + press_raw_sc * (c20 + press_raw_sc * c30)) + (c01 * temp_raw_sc) + temp_raw_sc * press_raw_sc * (c11 + c21 * press_raw_sc);
}


void dps310_readcoeffs(void)
{
    int buffer[18];

    i2c_readdata(DPS310_I2C_ADDRESS, 0x10, buffer, 18);

    c0Half = ((buffer[0]<<8 | buffer[1] )<<12 )>>12; // sign extend by shifting to 32bits and back
    c0Half = c0Half / 2U;
    c1 = ((buffer[1]<<8 | buffer[2] )<<16 )>>16;
    c00 = ((buffer[3]<<16 | buffer[4]<<8 | buffer[5] )<<8 )>>12;
    c10 = ((buffer[5]<<16 | buffer[6]<<8 | buffer[7] )<<12 )>>12;
    c01 = ((buffer[8]<<8 | buffer[9] )<<16 )>>16;
    c11 = ((buffer[10]<<8 | buffer[11] )<<16 )>>16;
    c20 = ((buffer[12]<<8 | buffer[13] )<<16 )>>16;
    c21 = ((buffer[14]<<8 | buffer[15] )<<16 )>>16;
    c30 = ((buffer[16]<<8 | buffer[17] )<<16 )>>16;
}
