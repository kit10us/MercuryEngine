#include <rm/ResourceManager.h>
#include <QUnitTest.h>
#include <conio.h>

// Dummy class used as a resoruce in testing resource manager.
class Thing
{
public:
};

class ResourceManager_UnitTest : public TestSuite
{
public:
	static rm::ResourceManager< Thing > * s_manager;

	~ResourceManager_UnitTest()
	{
		delete s_manager;
		s_manager = 0;
	}
	
	ResourceManager_UnitTest()
	{
		if ( ! s_manager )
		{
			s_manager = new rm::ResourceManager< Thing >( "Thing" );
		}

		AddTest( "ResourceManager_GetName", []( TestSuite & ts )
		{
			ts.AssertAreEqual< std::string >( "Thing", s_manager->GetName(), "GetName test" );
		} );

		AddTest( "ResourceManager_NegativeExists", []( TestSuite & ts )
		{
			ts.AssertAreEqual< bool >( false, s_manager->Exists( "First" ), "Negative exists \"First\" test" );
		} );
		
		AddTest( "ResourceManager_AddFirst", []( TestSuite & ts )
		{
			s_manager->AddResource( "First", new Thing );
			ts.AssertAreEqual< bool >( true, s_manager->Exists( "First" ), "Exists \"First\" test" );
			ts.AssertAreEqual< bool >( false, s_manager->Exists( "first" ), "Negative exists \"first\" (wrong case) test" );
		} );
	}
};

rm::ResourceManager< Thing > * ResourceManager_UnitTest::s_manager = 0;

int main( int argc, char ** argv )
{
	try
	{
		ResourceManager_UnitTest tests;
		tests.Execute();
	}
	catch( std::exception & ex )
	{
		std::cout << std::endl << std::endl << "EXCEPTION: " << ex.what() << std::endl;
	}
	while( !_getch() ) {};
	return 0;
}