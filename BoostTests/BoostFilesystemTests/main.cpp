#include <iostream>
#include <conio.h>
#include <boost/filesystem.hpp>

void Test( std::string expected, std::string actual )
{
	std::cout << "Test for [" << expected << "]" << std::endl;
	std::cout << "     got [" << expected << "]" << std::endl;
	std::cout << std::endl;
}

int main( int argc, const char ** argv )
{
	boost::filesystem::path myPath( "root\\sub\\filename.extension" );

	Test( "root\\sub\\filename.extension", myPath.generic_string() );
	Test( "filename.extension", myPath.filename().generic_string() );

	while( ! _getch() );
	return 0;
}