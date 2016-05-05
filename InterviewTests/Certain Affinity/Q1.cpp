#include <iostream>
#include <conio.h>

#ifdef PACK_DATABLOCK
#pragma pack(push,1)
#endif

class DataBlock 
{
	bool flag;
	int number; 
	short small;
	float real;

public: // Add by me to be able to access member functions.

	void Clear()
	{
		// Values changed to illustrate by order in print outs.
		flag = true;
		number = 0x44332211;
		small = 0x7766;
		real = 0.31415f;
	}

	virtual const char * GetName()
	{
		return "DataBlock";
	}

	virtual void Print()
	{
		printf( "DataBlock = {...}" );
	}
};

#ifdef PACK_DATABLOCK
#pragma pack(pop)
#endif

void Q1()
{
	printf( "[Q1]\n" );
	size_t size = sizeof( DataBlock );
	DataBlock db;
	db.Clear();
	unsigned char * mem = (unsigned char*)&db;
	for( size_t i = 0, end = sizeof( DataBlock ); i != end; ++i )
	{
		printf( "%2i ", i );
	}
	printf( "\n" );
	for( size_t i = 0, end = sizeof( DataBlock ); i != end; ++i )
	{
		printf( "%2x ", mem[ i ] );
	}
	std::cout << std::endl;

	std::cout << std::endl;
}
