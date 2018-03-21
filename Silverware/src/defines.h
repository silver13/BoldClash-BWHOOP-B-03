
// defines for things that do not normally need changing


#define MOTOR_BL 0
#define MOTOR_FL 1
#define MOTOR_FR 3
#define MOTOR_BR 2

#define PIDNUMBER 3

#define DEGTORAD 0.017453292f
#define RADTODEG 57.29577951f

#define AUXNUMBER 16

#define ROLL 0
#define PITCH 1
#define YAW 2

// this should be precalculated by the compiler as it's a constant
#define FILTERCALC( sampleperiod, filtertime) (1.0f - ( 6.0f*(float)sampleperiod) / ( 3.0f *(float)sampleperiod + (float)filtertime))


#define RXMODE_BIND 0
#define RXMODE_NORMAL (!RXMODE_BIND)

/*
#define CH_ON 4
#define CH_OFF 5
#define CH_FLIP 0
#define CH_EXPERT 1
#define CH_HEADFREE 2
#define CH_RTH 3
*/

// defines for bayang protocol radio
#define CH_ON (AUXNUMBER - 2)
#define CH_OFF (AUXNUMBER - 1)
#define CH_FLIP 0
#define CH_EXPERT 1
#define CH_HEADFREE 2
#define CH_RTH 3
#define CH_AUX1 4
#define CH_AUX2 5
#define CH_EMG 10
#define CH_TO 11
// trims numbers have to be sequential, start at CH_PIT_TRIM
#define CH_PIT_TRIM 6
#define CH_RLL_TRIM 7
#define CH_THR_TRIM 8
#define CH_YAW_TRIM 9
// next 3 channels only when *not* using USE_STOCK_TX
#define CH_INV 6
#define CH_VID 7
#define CH_PIC 8

// defines for cg023 protocol
#define CH_CG023_LED 3
#define CH_CG023_FLIP 0
#define CH_CG023_STILL 2
#define CH_CG023_VIDEO 1

#define CH_H7_FLIP 0
#define CH_H7_VIDEO 1
#define CH_H7_FS 2

#define CH_CX10_CH0 0
#define CH_CX10_CH2 2

#define CH_AUX3 CH_OFF
#define CH_AUX4 CH_OFF

// devo tx channel mapping
// also for nr24multipro
#define DEVO_CHAN_5 CH_INV
#define DEVO_CHAN_6 CH_FLIP
#define DEVO_CHAN_7 CH_PIC
#define DEVO_CHAN_8 CH_VID
#define DEVO_CHAN_9 CH_HEADFREE
#define DEVO_CHAN_10 CH_RTH

// multimodule mapping ( taranis )
#define MULTI_CHAN_5 CH_FLIP
#define MULTI_CHAN_6 CH_RTH
#define MULTI_CHAN_7 CH_PIC
#define MULTI_CHAN_8 CH_VID
#define MULTI_CHAN_9 CH_HEADFREE
#define MULTI_CHAN_10 CH_INV


#ifdef USE_DEVO
// devo tx channel mapping
// also for nr24multipro
#define CHAN_5 CH_INV
#define CHAN_6 CH_FLIP
#define CHAN_7 CH_PIC
#define CHAN_8 CH_VID
#define CHAN_9 CH_HEADFREE
#define CHAN_10 CH_RTH
#define CHAN_ON CH_ON
#define CHAN_OFF CH_OFF
#endif

#ifdef USE_MULTI
// multimodule mapping ( taranis )
#define CHAN_5 CH_FLIP
#define CHAN_6 CH_RTH
#define CHAN_7 CH_PIC
#define CHAN_8 CH_VID
#define CHAN_9 CH_HEADFREE
#define CHAN_10 CH_INV
#define CHAN_ON CH_ON
#define CHAN_OFF CH_OFF
#endif

#ifdef USE_STOCK_TX
#define CHAN_5 CH_EXPERT
#define CHAN_6 CH_AUX1
#define CHAN_7 CH_HEADFREE
#define CHAN_8 CH_RLL_TRIM
#define CHAN_9 CH_PIT_TRIM
#define CHAN_10 CH_OFF
#define CHAN_ON CH_ON
#define CHAN_OFF CH_OFF
#endif



// used for the pwm driver
#define CH1 0
#define CH2 1
#define CH3 2
#define CH4 3


#define int32 int_fast32_t
#define int16 int_fast16_t
#define int8 int_fast8_t

#define uint32 uint_fast32_t
#define uint16 uint_fast16_t
#define uint8 uint_fast8_t

// for h-bridge states
#define FREE 2
#define BRAKE 3
#define DIR1 1
#define DIR2 0

// for inverted flight motor direction
#define FORWARD DIR2
#define REVERSE DIR1



// kalman Q/R ratio for Q = 0.02
// loop time 1000Hz
#define	KAL1_HZ_10	0.004078
#define	KAL1_HZ_20	0.015952
#define	KAL1_HZ_30	0.035546
#define	KAL1_HZ_40	0.062984
#define	KAL1_HZ_50	0.097857
#define	KAL1_HZ_60	0.139957
#define	KAL1_HZ_70	0.190992
#define	KAL1_HZ_80	0.249072
#define	KAL1_HZ_90	0.308894
#define	KAL1_HZ_100	0.397188
#define	KAL1_HZ_120	0.542488
#define	KAL1_HZ_140	0.719026
#define	KAL1_HZ_160	0.928746
#define	KAL1_HZ_180	1.144837
#define	KAL1_HZ_200	1.354386
#define	KAL1_HZ_220	1.611742
#define	KAL1_HZ_240	1.87532
#define	KAL1_HZ_260	2.123421
#define	KAL1_HZ_280	2.377006
#define	KAL1_HZ_300	2.595641
#define	KAL1_HZ_320	2.864404
#define	KAL1_HZ_340	3.052077
#define	KAL1_HZ_360	3.272997
#define	KAL1_HZ_380	3.44942
#define	KAL1_HZ_400	3.679173
#define	KAL1_HZ_420	3.721861
#define	KAL1_HZ_440	3.880844
#define	KAL1_HZ_460	3.908564
#define	KAL1_HZ_480	3.984022
#define	KAL1_HZ_500	4.100000









