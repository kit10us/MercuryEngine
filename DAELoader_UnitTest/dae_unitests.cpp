#include <qunit/QUnitTest.h>
#include <qjson/QJson.h>
#include <DAE/DaeLib.h>
#include <conio.h>

class DAE_UnitTest : public qunit::TestSuite
{
public:
	DAE_UnitTest()
	{
		AddTest( "DXI_VertexDeclaration_JSON_Basic", []( TestSuite & ts )
		{
			qjson::Object vertexJson;
			vertexJson.Add( { "Position", "Float3" } );
			vertexJson.Add( { "Tex1", "Float2" } );
			
			dxi::VertexDeclaration vd( vertexJson );
			size_t expectedSize = size_t();
			expectedSize += sizeof( float ) * 3; // Position
			expectedSize += sizeof( float ) * 2; // Tex1
			ts.AssertAreEqual< size_t >( vd.GetSize(),  expectedSize, "GetSize test" );
		} );

		AddTest( "DXI_VertexDeclaration_JSON_Typical", []( TestSuite & ts )
		{
			qjson::Object vertexJson;
			vertexJson.Add( { "Position", "Float3" } );
			vertexJson.Add( { "Normal", "Float3" } );
			vertexJson.Add( { "Diffuse", "D3DCOLOR" } );
			vertexJson.Add( { "Specular", "D3DCOLOR" } );
			vertexJson.Add( { "Tex1", "Float2" } );

			dxi::VertexDeclaration vd( vertexJson );
			size_t expectedSize = size_t();
			expectedSize += sizeof( float ) * 3; // Position
			expectedSize += sizeof( float ) * 3; // Normal
			expectedSize += sizeof( unsigned int ) * 1; // Diffuse
			expectedSize += sizeof( unsigned int ) * 1; // Specular
			expectedSize += sizeof( float ) * 2; // Tex1
			ts.AssertAreEqual< size_t >( vd.GetSize(), expectedSize, "GetSize test" );
		} );

	}
};

int main( int argc, char ** argv )
{
	try
	{
		DAE_UnitTest tests;
		tests.Execute();
	}
	catch( std::exception & ex )
	{
		std::cout << std::endl << std::endl << "EXCEPTION: " << ex.what() << std::endl;
	}
	while( !_getch() ) {};
	return 0;
}
