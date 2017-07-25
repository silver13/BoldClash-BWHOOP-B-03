
#include "defines.h"
#include "hardware.h"

// pids in pid.c

// rate in deg/sec
// for acro mode
#define MAX_RATE 360.0
#define MAX_RATEYAW 360.0

// max angle for level mode
#define MAX_ANGLE_HI 70.0f

#define LOW_RATES_MULTI 0.5f

// disable inbuilt expo functions
#define DISABLE_EXPO

// use if your tx has no expo function
// also comment out DISABLE_EXPO to use
// -1 to 1 , 0 = no exp
// positive = less sensitive near center 
#define EXPO_XY 0.3
#define EXPO_YAW 0.0



// battery saver
// do not start software if battery is too low
// flashes 2 times repeatedly at startup
//#define STOP_LOWBATTERY

// under this voltage the software will not start 
// if STOP_LOWBATTERY is defined above
#define STOP_LOWBATTERY_TRESH 3.3

// voltage to start warning
// volts
#define VBATTLOW 3.5

// lvc starts flashing under this raw value regardless of throttle
#define VBATTLOW_MIN 2.7

// compensation for battery voltage vs throttle drop
#define VDROP_FACTOR 0.7
// calculate above factor automatically
#define AUTO_VDROP_FACTOR

// voltage hysteresys
// in volts
#define HYST 0.10



// lower throttle when battery below treshold
//#define LVC_LOWER_THROTTLE
#define LVC_LOWER_THROTTLE_VOLTAGE 3.30
#define LVC_LOWER_THROTTLE_VOLTAGE_RAW 2.70
#define LVC_LOWER_THROTTLE_KP 3.0



// Gyro LPF filter frequency
// gyro filter 0 = 250hz delay 0.97mS
// gyro filter 1 = 184hz delay 2.9mS
// gyro filter 2 = 92hz delay 3.9mS
// gyro filter 3 = 41hz delay 5.9mS (Default)
// gyro filter 4 = 20hz
// gyro filter 5 = 10hz
// gyro filter 6 = 5hz
// gyro filter 7 = 3600hz delay 0.17mS
#define GYRO_LOW_PASS_FILTER 3


// software gyro lpf ( iir )
// set only one below
//#define SOFT_LPF_1ST_023HZ
//#define SOFT_LPF_1ST_043HZ
//#define SOFT_LPF_1ST_100HZ
//#define SOFT_LPF_2ND_043HZ
#define SOFT_LPF_2ND_088HZ
//#define SOFT_LPF_4TH_088HZ
//#define SOFT_LPF_4TH_160HZ
//#define SOFT_LPF_4TH_250HZ
//#define SOFT_LPF_NONE




// switch function selection

// H8 protocol channels
// CH_FLIP - flip,  CH_HEADFREE - headfree, CH_RTH - headingreturn
// CH_EXPERT , CH_INV (inv h101 tx)
// CH_RLL_TRIM , CH_PIT_TRIM - trim buttons pitch, roll

// cg023 protocol chanels
// CH_CG023_FLIP , CH_CG023_VIDEO , CH_CG023_STILL , CH_CG023_LED

// H7 channels
// CH_H7_FLIP , CH_H7_VIDEO , CH_H7_FS

// CX10
// CH_CX10_CH0  (unknown) , CH_CX10_CH2 ( rates mid)

// DEVO channels (bayang protocol)
// DEVO_CHAN_5 - DEVO_CHAN_10

// Multiprotocol can use MULTI_CHAN_5 - MULTI_CHAN_10  (bayang protocol)

// CH_ON - on always ( all protocols)
// CH_OFF - off always ( all protocols)

#define HEADLESSMODE CH_OFF
// rates / expert mode
#define RATES CH_EXPERT

#define LEVELMODE CH_AUX1

#define STARTFLIP CH_OFF

#define LEDS_ON CH_ON

// switch for fpv / other, requires fet
// comment out to disable
//#define FPV_ON CH_ON

// aux1 channel starts on if this is defined, otherwise off.
#define AUX1_START_ON

// improves reception and enables trims if used
// trims are incompatible with DEVO TX when used
//#define USE_STOCK_TX

// automatically remove center bias ( needs throttle off for 1 second )
//#define STOCK_TX_AUTOCENTER

// Gestures enable ( gestures 1 = acc only)
//#define GESTURES1_ENABLE
#define GESTURES2_ENABLE

// enable motor filter
// hanning 3 sample fir filter
#define MOTOR_FILTER

// clip feedforward attempts to resolve issues that occur near full throttle
//#define CLIP_FF

// pwm frequency for motor control
// a higher frequency makes the motors more linear
// in Hz
#define PWMFREQ 24000

// motor curve to use
// the pwm frequency has to be set independently
// 720motors - use 8khz and curve none.
//#define MOTOR_CURVE_NONE
//#define MOTOR_CURVE_6MM_490HZ
//#define MOTOR_CURVE_85MM_8KHZ
//#define MOTOR_CURVE_85MM_32KHZ
//#define BOLDCLASH_716MM_8K
#define BOLDCLASH_716MM_24K

// a filter which makes throttle feel faster
//#define THROTTLE_TRANSIENT_COMPENSATION

//throttle feedback from accelerometer
//#define THROTTLE_SMOOTH

// lost quad beeps using motors (30 sec timeout)
//#define MOTOR_BEEPS

// throttle angle compensation in level mode
// comment out to disable
//#define AUTO_THROTTLE

// enable auto lower throttle near max throttle to keep control
// comment out to disable
//#define MIX_LOWER_THROTTLE
//#define MIX_INCREASE_THROTTLE

// Radio protocol selection
// select only one
//#define RX_CG023_PROTOCOL
//#define RX_H7_PROTOCOL
//#define RX_BAYANG_PROTOCOL
#define RX_BAYANG_PROTOCOL_TELEMETRY
//#define RX_BAYANG_PROTOCOL_BLE_BEACON
//#define RX_BAYANG_BLE_APP
//#define RX_CX10BLUE_PROTOCOL

// 0 - 7 - power for telemetry
#define TX_POWER 7


// Flash saving features
#define DISABLE_HEADLESS
#define DISABLE_FLIP_SEQUENCER

// led brightness in-flight ( solid lights only)
// 0- 15 range
#define LED_BRIGHTNESS 15



//#define BUZZER_ENABLE



// Comment out to disable pid tuning gestures
#define PID_GESTURE_TUNING
#define COMBINE_PITCH_ROLL_PID_TUNING

// flash save method
// flash_save 1: pids + accel calibration
// flash_save 2: accel calibration to option bytes
#define FLASH_SAVE1
//#define FLASH_SAVE2


//##################################
// debug / other things
// this should not be usually changed



// enable serial driver ( pin SWCLK after calibration) 
// WILL DISABLE PROGRAMMING AFTER GYRO CALIBRATION - 2 - 3 seconds after powerup)
//#define SERIAL_ENABLE
// enable some serial info output
//#define SERIAL_INFO


// level mode "manual" trims ( in degrees)
// pitch positive forward
// roll positive right
#define TRIM_PITCH 0.0
#define TRIM_ROLL 0.0

// disable motors for testing
//#define NOMOTORS

// throttle direct to motors for thrust measure
// #define MOTORS_TO_THROTTLE

// loop time in uS
// this affects soft gyro lpf frequency if used
#define LOOPTIME 1000

// failsafe time in uS
#define FAILSAFETIME 1000000  // one second

// max rate used by level pid ( limit )
#define LEVEL_MAX_RATE 360

// invert yaw pid for hubsan motors
//#define INVERT_YAW_PID

// debug things ( debug struct and other)
//#define DEBUG

// rxdebug structure
//#define RXDEBUG

// enable motors if pitch / roll controls off center (at zero throttle)
// possible values: 0 / 1
// use in acro build only
#define ENABLESTIX 0
#define ENABLESTIX_TRESHOLD 0.3
#define ENABLESTIX_TIMEOUT 1e6

// overclock to 64Mhz
//#define ENABLE_OVERCLOCK


// limit minimum motor output to a value (0.0 - 1.0)
//#define MOTOR_MIN_ENABLE
#define MOTOR_MIN_VALUE 0.05

// limit max motor output to a value (0.0 - 1.0)
//#define MOTOR_MAX_ENABLE
#define MOTOR_MAX_VALUE 1.0




#pragma diag_warning 1035 , 177 , 4017
#pragma diag_error 260

//--fpmode=fast





// define logic - do not change
///////////////

// used for pwm calculations
#ifdef ENABLE_OVERCLOCK
#define SYS_CLOCK_FREQ_HZ 64000000
#else
#define SYS_CLOCK_FREQ_HZ 48000000
#endif



#ifdef MOTOR_BEEPS
#ifdef USE_ESC_DRIVER
#warning "MOTOR BEEPS_WORKS WITH BRUSHED MOTORS ONLY"
#endif
#endif


//needed for rssi
//#ifdef OSD_LTM_PROTOCOL
//#define RXDEBUG
//#endif


// for the ble beacon to work after in-flight reset
#ifdef RX_BAYANG_PROTOCOL_BLE_BEACON
#undef STOP_LOWBATTERY
#endif
