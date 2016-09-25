#include <iostream>
#include <conio.h>
#include <lua.hpp>

#include <test-01.h>
#include <test-02.h>
#include <test-03.h>
#include <test-04.h>

#pragma comment( lib, "lua53" )


int main( int argc, char ** argv )
{
	using namespace std;

	cout << "BEGIN LUA TESTS..." << endl << endl;

	test01::main( argc, argv );

	cout << endl << endl;

	test02::main( argc, argv );

	cout << endl << endl;

	test03::main( argc, argv );		  

	cout << endl << endl;

	test04::main( argc, argv );

	cout << "END LUA TESTS." << endl;

	while( !_getch() );

	return 0;
}
