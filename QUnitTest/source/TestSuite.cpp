#include "TestSuite.h"
#include <chrono>

TestSuite::TestSuite()
{
}

TestSuite::~TestSuite()
{
}

void TestSuite::AddTest( std::string name, UnitTest test )
{
	m_tests.push_back( std::tuple<std::string, UnitTest >( name, test ) );
}

void TestSuite::Execute()
{
	using std::cout;
	using std::endl;

	cout << "Begin unit testing..." << endl << endl;
	std::chrono::steady_clock::time_point begin_total = std::chrono::steady_clock::now();

	int i = 0;
	for( auto test : m_tests )
	{
		cout << "Executing test " << i << " - \"" << std::get<0>( test ) << "\"." << endl;
		std::chrono::steady_clock::time_point begin_test = std::chrono::steady_clock::now();
		std::get<1>( test )(*this);
		std::chrono::steady_clock::time_point end_test = std::chrono::steady_clock::now();
		cout << "Time Duration: " << std::chrono::duration_cast< std::chrono::microseconds >( end_test - begin_test ).count() << "ms" << endl;
		i++;
	}

	std::chrono::steady_clock::time_point end_total = std::chrono::steady_clock::now();
	cout << "Total test duration: " << std::chrono::duration_cast<std::chrono::microseconds>(end_total - begin_total ).count() << "ms" << endl;

	cout << endl << "End unit testing." << endl;
}
