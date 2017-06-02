
int gestures2( void);
int gesture_sequence( int gesture);

// warning: using if x>GESTURE_UDR to check pid gestures
enum gestures_enum{
    GESTURE_NONE = 0, 
    GESTURE_DDD,
    GESTURE_UUU,
    GESTURE_LLD,
    GESTURE_RRD,
    GESTURE_UDU,
    GESTURE_UDD,
    GESTURE_UDR,
    GESTURE_UDL
    
};


