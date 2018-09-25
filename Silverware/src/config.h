
#include "defines.h"
#include "hardware.h"

// pids in pid.c

// rate in deg/sec
// for acro mode
#define MAX_RATE 360.0
#define MAX_RATEYAW 360.0
// use if your tx has no expo function
// 0.00 to 1.00 , 0 = no exp
// positive = less sensitive near center 
#define ACRO_EXPO_XY 0.0
#define ACRO_EXPO_YAW 0.0



// max angle for level mode
#define LEVEL_MAX_ANGLE 70.0f
#define EXPO_XY 0.0
#define EXPO_YAW 0.0

#define LOW_RATES_MULTI 0.5f



// *******************************ANALOG AUX CHANNELS*****************************
// For some protocols, use Tx channels as auxiliary analog values
// Bayang with analog aux protocol (Tx optional mode enabled in modified Multimodule and DeviationTx) has two analog channels available:
//     Multimodule: channels 14 and 15
//     Deviation: channels 13 and 14
// Sbus and DSM can use analog values from any channel
// comment to disable
//#define USE_ANALOG_AUX
// Select analog feature for each channel
// comment to disable
//#define ANALOG_RATE_MULT CHAN_13
//#define ANALOG_MAX_ANGLE CHAN_14
//#define ANALOG_RP_P  CHAN_13 // Adjust Roll and Pitch together
//#define ANALOG_RP_I  CHAN_13
//#define ANALOG_RP_D  CHAN_14
//#define ANALOG_RP_PD CHAN_14 // Adjust Roll and Pitch P & D together
//#define ANALOG_R_P   CHAN_13 // Adjust Roll only
//#define ANALOG_R_I   CHAN_13
//#define ANALOG_R_D   CHAN_14
//#define ANALOG_P_P   CHAN_13 // Adjust Pitch only
//#define ANALOG_P_I   CHAN_13
//#define ANALOG_P_D   CHAN_14
//#define ANALOG_Y_P   CHAN_13 // Adjust Yaw only
//#define ANALOG_Y_I   CHAN_13
//#define ANALOG_Y_D   CHAN_14
// The following define can always be left uncommented. It just gathers all analog aux PID settings together into one define.
#if defined USE_ANALOG_AUX && (defined ANALOG_R_P || defined ANALOG_R_I || defined ANALOG_R_D || defined ANALOG_P_P || defined ANALOG_P_I || defined ANALOG_P_D || defined ANALOG_Y_P || defined ANALOG_Y_I || defined ANALOG_Y_D || defined ANALOG_RP_P || defined ANALOG_RP_I || defined ANALOG_RP_D || defined ANALOG_RP_PD)
    #define ANALOG_AUX_PIDS
#endif


// battery saver
// do not start software if battery is too low
// flashes 2 times repeatedly at startup
//#define STOP_LOWBATTERY

// voltage to start warning
// volts
#define VBATTLOW 3.5

// compensation for battery voltage vs throttle drop
#define VDROP_FACTOR 0.7
// calculate above factor automatically
#define AUTO_VDROP_FACTOR

// voltage hysteresis
// in volts
#define HYST 0.10



// lower throttle when battery below threshold
//#define LVC_LOWER_THROTTLE
#define LVC_LOWER_THROTTLE_VOLTAGE 3.30
#define LVC_LOWER_THROTTLE_VOLTAGE_RAW 2.70
#define LVC_LOWER_THROTTLE_KP 3.0



// Gyro LPF filter frequency
// gyro filter 0 = 250hz delay 0.97mS
// gyro filter 1 = 184hz delay 2.9mS
// gyro filter 2 = 92hz delay 3.9mS
// gyro filter 3 = 41hz delay 5.9mS (Default)
#define GYRO_LOW_PASS_FILTER 3


// software gyro lpf ( iir )
// set only one below
//#define SOFT_LPF_1ST_043HZ
//#define SOFT_LPF_1ST_100HZ
//#define SOFT_LPF_2ND_043HZ
//#define SOFT_LPF_2ND_088HZ
//#define SOFT_LPF_4TH_088HZ
//#define SOFT_LPF_4TH_160HZ
//#define SOFT_LPF_4TH_250HZ
#define SOFT_LPF_1ST_HZ 100
//#define SOFT_LPF_2ND_HZ 100
//#define SOFT_KALMAN_GYRO KAL1_HZ_90
//#define SOFT_LPF_NONE

// transmitter type
//#define USE_STOCK_TX
#define USE_DEVO
//#define USE_MULTI

// switch function selection

// H8 protocol channels
// CH_FLIP - flip,  CH_HEADFREE - headfree, CH_RTH - headingreturn
// CH_EXPERT , CH_INV (inv h101 tx)
// CH_RLL_TRIM , CH_PIT_TRIM - trim buttons pitch, roll
// CH_EMG , CH_TO - boldclash stock tx

// DEVO channels (bayang protocol)
// DEVO_CHAN_5 - DEVO_CHAN_12

// Multiprotocol can use MULTI_CHAN_5 - MULTI_CHAN_13, but not MULTI_CHAN_11  (bayang protocol)

// CH_ON - on always ( all protocols)
// CH_OFF - off always ( all protocols)

// CHAN_5 - CHAN_12/13 - auto based on tx selection

// rates / expert mode
#define RATES CH_EXPERT

#define LEVELMODE CH_AUX1

#define STARTFLIP CH_OFF

#define LEDS_ON CH_ON

// switch for fpv / other, requires fet
// comment out to disable
//#define FPV_ON CH_ON


// enable inverted flight code ( brushless only )
//#define INVERTED_ENABLE
//#define FN_INVERTED CH_OFF //for brushless only

// aux1 channel starts on if this is defined, otherwise off.
#define AUX1_START_ON

// automatically remove center bias ( needs throttle off for 1 second )
//#define STOCK_TX_AUTOCENTER

// enable motor filter - select one
// motorfilter1: hanning 3 sample fir filter
// motorfilter2: 1st lpf, 0.2 - 0.6 , 0.6 = less filtering
//#define MOTOR_FILTER
#define MOTOR_FILTER2_ALPHA 0.3
//#define MOTOR_KAL KAL1_HZ_70
//#define MOTOR_KAL_2ND KAL1_HZ_90

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
// if the quad resets , or for brushless ,try a lower value
#define THROTTLE_TRANSIENT_COMPENSATION_FACTOR 7.0 

// lost quad beeps using motors (30 sec timeout)
//#define MOTOR_BEEPS

// throttle angle compensation in level mode
// comment out to disable
//#define AUTO_THROTTLE


// Betaflight like mix scaling
//#define MIX_SCALING


// enable auto lower throttle near max throttle to keep control
// mix3 works better with brushless
// comment out to disable
//#define MIX_LOWER_THROTTLE
//#define MIX_INCREASE_THROTTLE

//#define MIX_LOWER_THROTTLE_3
//#define MIX_INCREASE_THROTTLE_3

// Radio protocol selection
// select only one

#define RX_BAYANG_PROTOCOL_TELEMETRY
//#define RX_NRF24_BAYANG_TELEMETRY
//#define RX_BAYANG_BLE_APP
//#define RX_SBUS
//#define RX_SUMD

// 0 - 7 - power for telemetry
#define TX_POWER 7


// Flash saving features
#define DISABLE_FLIP_SEQUENCER
//#define DISABLE_GESTURES2

// led brightness in-flight ( solid lights only)
// 0- 15 range
#define LED_BRIGHTNESS 15


// external buzzer - pins in hardware.h
//#define BUZZER_ENABLE



// Comment out to disable pid tuning gestures
#define PID_GESTURE_TUNING
#define COMBINE_PITCH_ROLL_PID_TUNING

// flash save method
// flash_save 1: pids + accel calibration
// flash_save 2: accel calibration to option bytes
#define FLASH_SAVE1
//#define FLASH_SAVE2


//#define PID_ROTATE_ERRORS

// Removes roll and pitch bounce back after flips
//#define TRANSIENT_WINDUP_PROTECTION




//##################################
// debug / other things
// this should not be usually changed


// level mode "manual" trims ( in degrees)
// pitch positive forward
// roll positive right
#define TRIM_PITCH 0.0
#define TRIM_ROLL 0.0

// disable motors for testing
//#define NOMOTORS

// throttle direct to motors for thrust measure
// #define MOTORS_TO_THROTTLE

// throttle direct to motors for thrust measure as a flight mode
//#define MOTORS_TO_THROTTLE_MODE MULTI_CHAN_8


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



// for the ble beacon to work after in-flight reset
#ifdef RX_BAYANG_PROTOCOL_BLE_BEACON
#undef STOP_LOWBATTERY
#endif


// gcc warnings in main.c
