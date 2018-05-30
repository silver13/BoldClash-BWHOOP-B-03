# E011-BWHOOP-H8-Silverware
E011 flight controllers with Silverware/NotFastEnuf experimental features and chime13 settings
Thanks to @NotFastEnuf for all his great contributions

These are my preferred defaults for my E011 + (Taranis QX7 + iRX4+) using NFE/Boss frames and "x" type frames under 120mm

RX_BAYANG_PROTOCOL_TELEMETRY_AUTOBIND

PIDs work with all brushed motor sizes without any additional tuning

Separate Arm and Idle_Up

Acro Only

1200 d/sec for Pitch and Roll

1000 d/sec for Yaw

Rates can be reduced to 60% on channel 8

Torque Boost of 2

STRONG_FILTERING

set D TERM CALCULATION TO ERROR full time

MIX_THROTTLE_INCREASE_MAX 0.6f for 60% Authority

The precompiled hex file is configured for an E011 + QX7 & iRX4+ using RX_BAYANG_PROTOCOL_TELEMETRY_AUTOBIND

channel assignments as follow:

ARM = ch5

IDLE_UP = ch6

RATES = ch8
