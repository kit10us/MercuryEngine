#pragma once
#include <list>
#include <string>
#include <functional>
#include <iostream>
#include <cassert>

namespace qunit
{
	/// Used for standardizing testing.
	class TestSuite
	{
	public:
		TestSuite();
		virtual ~TestSuite();

		template< typename T >
		void AssertAreEqual( const T & left, const T & right, std::string description ) const
		{
			using std::cout;
			using std::endl;
			cout << description << "... ";
			if( left == right )
			{
				cout << "PASSED" << endl;
			}
			else
			{
				cout << "FAILED" << endl;
				assert( 0 );
			}
		}

		template<>
		void AssertAreEqual( const std::string & left, const std::string & right, std::string description ) const
		{
			using std::cout;
			using std::endl;
			cout << description << "... ";
			if( left == right )
			{
				cout << "PASSED" << endl;
			}
			else
			{
				cout << "FAILED (\"" << left << "\" != \"" << right << "\") " << endl;
				assert( 0 );
			}
		}

		template<>
		void AssertAreEqual( const bool & left, const bool & right, std::string description ) const
		{
			using std::cout;
			using std::endl;
			cout << description << "... ";
			if( left == right )
			{
				cout << "PASSED" << endl;
			}
			else
			{
				cout << "FAILED (\"" << left << "\" != \"" << right << "\") " << endl;
				assert( 0 );
			}
		}

		template<>
		void AssertAreEqual( const int & left, const int & right, std::string description ) const
		{
			using std::cout;
			using std::endl;
			cout << description << "... ";
			if( left == right )
			{
				cout << "PASSED" << endl;
			}
			else
			{
				cout << "FAILED (\"" << left << "\" != \"" << right << "\") " << endl;
				assert( 0 );
			}
		}

		template<>
		void AssertAreEqual( const size_t & left, const size_t & right, std::string description ) const
		{
			using std::cout;
			using std::endl;
			cout << description << "... ";
			if( left == right )
			{
				cout << "PASSED" << endl;
			}
			else
			{
				cout << "FAILED (\"" << left << "\" != \"" << right << "\") " << endl;
				assert( 0 );
			}
		}
		template<>
		void AssertAreEqual( const double & left, const double & right, std::string description ) const
		{
			using std::cout;
			using std::endl;
			cout << description << "... ";
			if( left == right )
			{
				cout << "PASSED" << endl;
			}
			else
			{
				cout << "FAILED (\"" << left << "\" != \"" << right << "\") " << endl;
				assert( 0 );
			}
		}

		typedef void( *UnitTest )(TestSuite & ts);

		void AddTest( std::string name, UnitTest test );
		void Execute();

	private:
		std::list< std::tuple< std::string /*name*/, UnitTest > > m_tests;
	};
}