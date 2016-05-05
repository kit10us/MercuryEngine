#include <iostream>
#include <conio.h>
#include <string>
#include <assert.h>

void ClearMemory( void * buffer, unsigned long bufferSizeBytes, unsigned char clearValue )
{
	// Note: Never done anything like this before...
	// I'm working off of the principal that (from memory, good or bad) a size_t is the native alignment size.
	unsigned char * ucBuffer = static_cast< unsigned char *>( buffer );
	static size_t assumedAlignment = sizeof( size_t ); // Keywords are ok to use, right?
	size_t fillValue( clearValue );
	for ( size_t i = 1; i < assumedAlignment; ++i )
	{
		size_t s = sizeof( unsigned char );
		s <<= 3;
		fillValue <<= s;
		fillValue |= clearValue;
	}

	unsigned char * b = (unsigned char * )&fillValue;

	// First "fast" pass...
	size_t charOffset = 0;

	while( charOffset + assumedAlignment <= bufferSizeBytes )
	{
		size_t * stBuffer = reinterpret_cast< size_t * >( ucBuffer + charOffset );
		*stBuffer = fillValue;
		charOffset += assumedAlignment;
	}

	// Second "slow" pass...
	while( charOffset < bufferSizeBytes )
	{
		ucBuffer[ charOffset ] = clearValue;
		charOffset += sizeof( unsigned char );
	}
}


void Q3()
{
	const size_t bufferSize = 13;
	unsigned char buffer[ bufferSize ];
	ClearMemory( buffer, bufferSize, 13 );
	for ( size_t i = 0; i < bufferSize; ++i )
	{
		std::cout << static_cast< int >( buffer[ i ] ) << " ";
	}
	std::cout << std::endl;
}
