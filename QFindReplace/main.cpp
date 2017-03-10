#include <iostream>
#include <fstream>
#include <string>

int main( int argc, char ** argv )
{
	using namespace std;

	if ( argc < 5 )
	{
		return -1;
	}

	string lines;
	{
		ifstream ifs( argv[ 1 ] );
		string line;
		while ( getline( ifs, line ) )
		{
			lines += line + "\n";
		}
	}
	cout << "input  file: \"" << argv[ 1 ] << "\"\n";
	cout << "output file: \"" << argv[ 2 ] << "\"\n";
	cout << "replace    : \"" << argv[ 3 ] << "\"\n";
	cout << "with       : \"" << argv[ 4 ] << "\"\n";

	{
		size_t i = lines.find( argv[ 3 ] );
		while ( i != std::string::npos )
		{
			lines.replace( i, strlen( argv[ 3 ] ), argv[ 4 ] );
			i = lines.find( argv[ 3 ] );
		}
	}
	
	{
		ofstream ofs( argv[ 2 ] );
		ofs << lines;
	}
	
	return 0;
}