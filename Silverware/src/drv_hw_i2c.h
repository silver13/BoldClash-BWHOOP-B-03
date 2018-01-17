#include <inttypes.h>

void hw_i2c_init( void);
int hw_i2c_readdata(int address, int reg, int *data, int size );
int hw_i2c_readreg(int address, int reg );
void hw_i2c_writereg(int address, int reg ,int data);












