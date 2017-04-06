
#include "binary.h"
#include "drv_spi.h"

#include "project.h"
#include "xn297.h"
#include "hardware.h"

#ifdef XN297L_3WIRE

extern int spi_recvbyte( void);

void xn_writereg( int reg , int val)
{
	reg = reg&0x0000003F;
	reg = reg|0x00000020;
	spi_cson();
	spi_sendbyte( reg);
	spi_sendbyte( val);
	spi_csoff();
}

int xn_readreg( int reg)
{
	reg = reg&0x0000001F;
	spi_cson();
	spi_sendbyte( reg);
	int val =spi_recvbyte();
	spi_csoff();
	return val;
}

int xn_command( int command)
{
	spi_cson();
	spi_sendbyte(command);
	spi_csoff();
	return 0;
}
//


void xn_readpayload( int *data , int size )
{
	int index = 0;
	spi_cson();
	spi_sendbyte( B01100001 ); // read rx payload
	while(index<size)
	{
	data[index]=	spi_recvbyte();
	index++;
	}
	spi_csoff();
}



void xn_writerxaddress(  int *addr )	
{
 int index = 0;
spi_cson();
spi_sendbyte(0x2a);
	while(index<5)
	{
	spi_sendbyte( addr[index] );
	index++;
	}
spi_csoff();
}


void xn_writetxaddress(  int *addr )	
{
 int index = 0;
spi_cson();
spi_sendbyte(0x10|0x20);
	while(index<5)
	{
	spi_sendbyte( addr[index] );
	index++;
	}
spi_csoff();
}


void xn_writepayload( int data[] , int size )
{
	int index = 0;
	spi_cson();
	spi_sendbyte( 0xA0 ); // write tx payload
	while(index<size)
	{
	spi_sendbyte( data[index] );
	index++;
	}
	spi_csoff();
}


#endif










