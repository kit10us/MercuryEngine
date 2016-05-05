#include <qjson/QJson.h>
#include <QUnitTest.h>
#include <conio.h>

class JSON_UnitTest : public TestSuite
{
public:
	JSON_UnitTest()
	{
		AddTest( "JSON_ParseString_String", []( TestSuite & ts )
		{
			std::string jsonAsText( "\"a string\"" );

			qjson::String s( jsonAsText );

			ts.AssertAreEqual< std::string >( "a string", s.GetValue(), "GetValue test" );
			ts.AssertAreEqual< std::string >( "a string", s.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"a string\"", s.ToJSONString(), "ToJSONString test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );
		} );

		AddTest( "JSON_ParseString_String_OneCharacter", []( TestSuite & ts )
		{
			std::string jsonAsText( "\"1\"" );

			qjson::String s( jsonAsText );

			ts.AssertAreEqual< std::string >( "1", s.GetValue(), "GetValue test" );
			ts.AssertAreEqual< std::string >( "1", s.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"1\"", s.ToJSONString(), "ToJSONString test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );
		} );

		AddTest( "JSON_ParseString_String_Empty", []( TestSuite & ts )
		{
			std::string jsonAsText( "\"\"" );

			qjson::String s( jsonAsText );

			ts.AssertAreEqual< std::string >( "", s.GetValue(), "GetValue test" );
			ts.AssertAreEqual< std::string >( "", s.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"\"", s.ToJSONString(), "ToJSONString test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );
		} );

		AddTest( "JSON_ParseString_Number_Decimal", []( TestSuite & ts )
		{
			std::string jsonAsText( "12.21" );

			qjson::Number n( jsonAsText );

			ts.AssertAreEqual< double >( 12.21, n.GetValue(), "GetValue test" );
			ts.AssertAreEqual< double >( 12.21, n.ToDouble(), "ToDouble test" );
			ts.AssertAreEqual< std::string >( "12.21", n.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "12.21", n.ToJSONString(), "ToJSONString test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );
		} );

		AddTest( "JSON_ParseString_Number_Whole", []( TestSuite & ts )
		{
			std::string jsonAsText( "1221" );

			qjson::Number n( jsonAsText );

			ts.AssertAreEqual< double >( 1221, n.GetValue(), "GetValue test" );
			ts.AssertAreEqual< double >( 1221, n.ToDouble(), "ToDouble test" );
			ts.AssertAreEqual< std::string >( "1221", n.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "1221", n.ToJSONString(), "ToJSONString test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );
		} );

		AddTest( "JSON_ParseString_Number_Exponent", []( TestSuite & ts )
		{
			std::string jsonAsText( "12e2" );

			qjson::Number n( jsonAsText );

			ts.AssertAreEqual< double >( 1200, n.GetValue(), "GetValue test" );
			ts.AssertAreEqual< double >( 1200, n.ToDouble(), "ToDouble test" );
			ts.AssertAreEqual< std::string >( "1200", n.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "1200", n.ToJSONString(), "ToJSONString test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );
		} );

		AddTest( "JSON_ParseString_Number_Negative", []( TestSuite & ts )
		{
			std::string jsonAsText( "-1" );

			qjson::Number n( jsonAsText );

			ts.AssertAreEqual< double >( -1, n.GetValue(), "GetValue test" );
			ts.AssertAreEqual< double >( -1, n.ToDouble(), "ToDouble test" );
			ts.AssertAreEqual< std::string >( "-1", n.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "-1", n.ToJSONString(), "ToJSONString test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );
		} );

		AddTest( "JSON_ParseString_True", []( TestSuite & ts )
		{
			std::string jsonAsText( "true" );

			qjson::True s( jsonAsText );

			ts.AssertAreEqual< bool >( true, s.GetValue(), "GetValue test" );
			ts.AssertAreEqual< std::string >( "true", s.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "true", s.ToJSONString(), "ToJSONString test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );
		} );

		AddTest( "JSON_ParseString_False", []( TestSuite & ts )
		{
			std::string jsonAsText( "false" );

			qjson::False s( jsonAsText );

			ts.AssertAreEqual< bool >( false, s.GetValue(), "GetValue test" );
			ts.AssertAreEqual< std::string >( "false", s.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "false", s.ToJSONString(), "ToJSONString test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );
		} );

		AddTest( "JSON_ParseString_Null", []( TestSuite & ts )
		{
			std::string jsonAsText( "null" );

			qjson::Null s( jsonAsText );

			ts.AssertAreEqual< int >( 0, (int)s.GetValue(), "GetValue test" );
			ts.AssertAreEqual< std::string >( "null", s.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "null", s.ToJSONString(), "ToJSONString test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );
		} );

		AddTest( "JSON_ParseString_Pair_String", []( TestSuite & ts )
		{
			std::string jsonAsText( "\"pair name\":\"pair value\"" );

			qjson::Pair s( jsonAsText );

			ts.AssertAreEqual< std::string >( "pair name", s.GetName(), "GetName test" );
			ts.AssertAreEqual< std::string >( "pair value", s.GetValue()->ToString(), "GetValue().ToString test" );
			ts.AssertAreEqual<>( qjson::ValueType::String, s.GetValue()->GetType(), "GetValue()->GetType test" );
			ts.AssertAreEqual< std::string >( "\"pair name\":\"pair value\"", s.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"pair name\":\"pair value\"", s.ToJSONString(), "ToJSONString test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "Should consume the entire text." );
		} );

		AddTest( "JSON_ParseString_Array_Strings", []( TestSuite & ts )
		{
			std::string jsonAsText( "[\"a string\",\"another string\"]" );

			qjson::Array a( jsonAsText );
			ts.AssertAreEqual< size_t >( 2, a.Count(), "Count test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );

			const qjson::String & s = dynamic_cast< const qjson::String & >( a[ 0 ] );
			ts.AssertAreEqual< std::string >( "a string", s.GetValue(), "GetValue test" );
			ts.AssertAreEqual< std::string >( "a string", s.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"a string\"", s.ToJSONString(), "ToJSONString test" );

			const qjson::String & s2 = dynamic_cast< const qjson::String & >( a[ 1 ] );
			ts.AssertAreEqual< std::string >( "another string", s2.GetValue(), "GetValue test" );
			ts.AssertAreEqual< std::string >( "another string", s2.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"another string\"", s2.ToJSONString(), "ToJSONString test" );

			ts.AssertAreEqual< std::string >( "[\"a string\",\"another string\"]", a.ToJSONString(), "ToJSONString test" );
		} );

		AddTest( "JSON_ParseString_Object_Single", []( TestSuite & ts )
		{
			std::string jsonAsText( "{\"first name\":\"a string\"}" );

			qjson::Object o( jsonAsText );
			ts.AssertAreEqual< size_t >( 1, o.Count(), "Count test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );

			const qjson::Pair & p = o[ 0 ];
			ts.AssertAreEqual<>( qjson::ValueType::String, p.GetValue()->GetType(), "GetType test" );
			ts.AssertAreEqual< std::string >( "first name", p.GetName(), "GetName test" );
			ts.AssertAreEqual< std::string >( "\"first name\":\"a string\"", p.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"first name\":\"a string\"", p.ToJSONString(), "ToJSONString test" );
		} );

		AddTest( "JSON_ParseString_Object_WithNegativeNumber", []( TestSuite & ts )
		{
			std::string jsonAsText( "{\"first name\":-1}" );

			qjson::Object o( jsonAsText );
			ts.AssertAreEqual< size_t >( 1, o.Count(), "Count test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );

			const qjson::Pair & p = o[ 0 ];
			ts.AssertAreEqual<>( qjson::ValueType::Number, p.GetValue()->GetType(), "GetType test" );
			ts.AssertAreEqual< std::string >( "first name", p.GetName(), "GetName test" );
			ts.AssertAreEqual< std::string >( "\"first name\":-1", p.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"first name\":-1", p.ToJSONString(), "ToJSONString test" );
		} );

		AddTest( "JSON_ParseString_Object_Simple", []( TestSuite & ts )
		{
			std::string jsonAsText(  "{\"first name\":\"a string\",\"second name\":12.321}" );

			qjson::Object o( jsonAsText );
			ts.AssertAreEqual< size_t >( 2, o.Count(), "Count test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );

			const qjson::Pair & p = o[ 0 ];
			ts.AssertAreEqual< >( qjson::ValueType::String, p.GetValue()->GetType(), "GetType test" );
			ts.AssertAreEqual< std::string >( "first name", p.GetName(), "GetName test" );
			ts.AssertAreEqual< std::string >( "\"first name\":\"a string\"", p.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"first name\":\"a string\"", p.ToJSONString(), "ToJSONString test" );

			const qjson::Pair & p2 = o[ 1 ];
			ts.AssertAreEqual< >( qjson::ValueType::Number, p2.GetValue()->GetType(), "GetValue()->GetType Test" );
			ts.AssertAreEqual< std::string >( "second name", p2.GetName(), "GetName test" );
			ts.AssertAreEqual< std::string >( "12.321", p2.GetValue()->ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"second name\":12.321", p2.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"second name\":12.321", p2.ToJSONString(), "ToJSONString test" );
		} );

		AddTest( "JSON_ParseString_Object_AllPossibleTypes", []( TestSuite & ts )
		{
			std::string jsonAsText(  "{\"pair with string\":\"string\",\"pair with number\":3.1415,\"pair with true\":true,\"pair with false\":false,\"pair with null\":null}" );

			qjson::Object o( jsonAsText );
			ts.AssertAreEqual< size_t >( 5, o.Count(), "Count test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );

			const qjson::Pair & p = o[ 0 ];
			ts.AssertAreEqual< >( qjson::ValueType::String, p.GetValue()->GetType(), "GetType test" );
			ts.AssertAreEqual< std::string >( "pair with string", p.GetName(), "GetName test" );
			ts.AssertAreEqual< std::string >( "\"pair with string\":\"string\"", p.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"pair with string\":\"string\"", p.ToJSONString(), "ToJSONString test" );

			const qjson::Pair & p2 = o[ 1 ];
			ts.AssertAreEqual< >( qjson::ValueType::Number, p2.GetValue()->GetType(), "GetType test" );
			ts.AssertAreEqual< std::string >( "pair with number", p2.GetName(), "GetName test" );
			ts.AssertAreEqual< std::string >( "\"pair with number\":3.1415", p2.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"pair with number\":3.1415", p2.ToJSONString(), "ToJSONString test" );

			const qjson::Pair & p3 = o[ 2 ];
			ts.AssertAreEqual< >( qjson::ValueType::True, p3.GetValue()->GetType(), "GetType test" );
			ts.AssertAreEqual< std::string >( "pair with true", p3.GetName(), "GetName test" );
			ts.AssertAreEqual< std::string >( "\"pair with true\":true", p3.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"pair with true\":true", p3.ToJSONString(), "ToJSONString test" );

			const qjson::Pair & p4 = o[ 3 ];
			ts.AssertAreEqual< >( qjson::ValueType::False, p4.GetValue()->GetType(), "GetType test" );
			ts.AssertAreEqual< std::string >( "pair with false", p4.GetName(), "GetName test" );
			ts.AssertAreEqual< std::string >( "\"pair with false\":false", p4.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"pair with false\":false", p4.ToJSONString(), "ToJSONString test" );

			const qjson::Pair & p5 = o[ 4 ];
			ts.AssertAreEqual< >( qjson::ValueType::Null, p5.GetValue()->GetType(), "GetType test" );
			ts.AssertAreEqual< std::string >( "pair with null", p5.GetName(), "GetName test" );
			ts.AssertAreEqual< std::string >( "\"pair with null\":null", p5.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"pair with null\":null", p5.ToJSONString(), "ToJSONString test" );
		} );

		AddTest( "JSON_ParseString_Object_withObject", []( TestSuite & ts )
		{
			std::string jsonAsText( "{\"first level\":{\"second level\":\"deep string\"}}" );

			qjson::Object o( jsonAsText );
			ts.AssertAreEqual< int >( 1, o.Count(), "Count test" );
			ts.AssertAreEqual< size_t >( 0, jsonAsText.length(), "length of JSON test" );

			ts.AssertAreEqual< std::string >( "{\"first level\":{\"second level\":\"deep string\"}}", o.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "{\"first level\":{\"second level\":\"deep string\"}}", o.ToJSONString(), "ToJSONString test" );

			const qjson::Pair & p = o[ 0 ];
			ts.AssertAreEqual< >( qjson::ValueType::Object, p.GetValue()->GetType(), "GetType test" );
			ts.AssertAreEqual< std::string >( "first level", p.GetName(), "GetName test" );
			ts.AssertAreEqual< std::string >( "\"first level\":{\"second level\":\"deep string\"}", p.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"first level\":{\"second level\":\"deep string\"}", p.ToJSONString(), "ToJSONString test" );

			const qjson::Object & secondLevel = dynamic_cast< const qjson::Object & >( *p.GetValue() );
			ts.AssertAreEqual< int >( 1, secondLevel.Count(), "Count test" );

			ts.AssertAreEqual< std::string >( "{\"second level\":\"deep string\"}", secondLevel.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "{\"second level\":\"deep string\"}", secondLevel.ToJSONString(), "ToJSONString test" );

			const qjson::Pair & p2 = secondLevel[ 0 ];
			ts.AssertAreEqual< >( qjson::ValueType::String, p2.GetValue()->GetType(), "GetType test" );
			ts.AssertAreEqual< std::string >( "second level", p2.GetName(), "GetName test" );
			ts.AssertAreEqual< std::string >( "\"second level\":\"deep string\"", p2.ToString(), "ToString test" );
			ts.AssertAreEqual< std::string >( "\"second level\":\"deep string\"", p2.ToJSONString(), "ToJSONString test" );
		} );

		AddTest( "JSON_ParseString_Object_ArrayAccessByName", []( TestSuite & ts )
		{
			qjson::Object o = { { "first", "first value" }, { "second", "second value" } };

			ts.AssertAreEqual< std::string >( o[ "first" ].ToString(), "first value", "First value" );
			ts.AssertAreEqual< std::string >( o[ "second" ].ToString(), "second value", "Second value" );
		} );

		AddTest( "JSON_ObjectWithinObject", []( TestSuite & ts )
		{
			std::string secondObjectString = "{\"third\":\"third value\",\"fourth\":\"fourth value\"}";
			qjson::Object secondObject( secondObjectString );
			qjson::Pair p = { "secondObject", secondObject };
			qjson::Object o = { { "first", "first value" }, { "second", "second value" }, { "secondObject", secondObject } };
			qjson::Object o2ndo = o["secondObject"];
			
			ts.AssertAreEqual< std::string >( o["first"].ToString(), "first value", "first value" );
			ts.AssertAreEqual< std::string >( o["second"].ToString(), "second value", "second value" );
			ts.AssertAreEqual<>( o["secondObject"].GetType(), qjson::ValueType::Object , "secondObject is object" );

			const qjson::Object & secondObjectOut = (const qjson::Object&)o["secondObject"];
			ts.AssertAreEqual< size_t >( secondObjectOut.Count(), 2, "Count of pairs in second object." );


		} );
		
		AddTest( "JSON_Object_Add", []( TestSuite & ts )
		{
			qjson::Object o;
			o.Add( { "number", 1 }  );
			o.Add( { "string", "Hello, world!" } );
			o.Add( { "true", true } );
			o.Add( { "false", false } );

			ts.AssertAreEqual< std::string >( "Number", qjson::ToString( o[ 0 ].GetValue()->GetType() ), "Number type test" );
			ts.AssertAreEqual< std::string >( "String", qjson::ToString( o[1].GetValue()->GetType() ), "String type test" );
			ts.AssertAreEqual< std::string >( "True", qjson::ToString( o[2].GetValue()->GetType() ), "True type test" );
			ts.AssertAreEqual< std::string >( "False", qjson::ToString( o[3].GetValue()->GetType() ), "False type test" );
			ts.AssertAreEqual< std::string >( "{\"number\":1,\"string\":\"Hello, world!\",\"true\":true,\"false\":false}", o.ToJSONString(), "ToJSONString test" );
		} );

		AddTest( "JSON_WithWhitespace", []( TestSuite & ts )
		{
			std::string jsonRaw = "  { \"something\" : { \"somthing else\" :  \"value is here\" } , \"an array\" : [ true, false,   null ] } ";
			std::string jsonClean = "{\"something\":{\"somthing else\":\"value is here\"},\"an array\":[true,false,null]}";
			qjson::Object o( jsonRaw );

			ts.AssertAreEqual< std::string >( jsonClean, o.ToJSONString(), "ToJSONString test" );
		} );

		AddTest( "JSON_Array_ForEach", []( TestSuite & ts )
		{
			std::string jsonAsText( "[\"first string\",\"second string\",\"final string\"]" );
			qjson::Array a( jsonAsText );

			std::string expected[] = { "first string", "second string", "final string" };

			size_t i = 0;
			for( auto & item : a )
			{
				ts.AssertAreEqual< std::string >( item.ToString(), expected[i], "String compare" );
				i++;
			}
		} );

		AddTest( "JSON_Object_ForEach", []( TestSuite & ts )
		{
			qjson::Object o;
			o.Add( { "number", 1 } );
			o.Add( { "string", "Hello, world!" } );
			o.Add( { "true", true } );
			o.Add( { "false", false } );

			qjson::Pair expected[] = { { "number", 1 }, { "string", "Hello, world!" }, { "true", true }, { "false", false } };
			
			size_t i = 0;
			for( const auto item : o )
			{
				ts.AssertAreEqual< std::string >( item.ToString(), expected[i].ToString(), "String compare" );
				i++;
			}
		} );

		AddTest( "JSON_Object_SimpleCreate", []( TestSuite & ts )
		{
			qjson::Object o = { { "First", "1st" }, {"Second", "2nd" } };

			ts.AssertAreEqual< std::string >( o[0].GetName(), "First", "\"First\" element name." );
			ts.AssertAreEqual< std::string >( o[0].GetValue()->ToString(), "1st", "\"1st\" element value."  );
			ts.AssertAreEqual< std::string >( o[1].GetName(), "Second", "\"Second\" element name." );
			ts.AssertAreEqual< std::string >( o[1].GetValue()->ToString(), "2nd", "\"2nd\" element value." );
		} );
	}
};


int main( int argc, char ** argv )
{
	try
	{
		JSON_UnitTest tests;
		tests.Execute();
	}
	catch( std::exception & ex )
	{
		std::cout << std::endl << std::endl << "EXCEPTION: " << ex.what() << std::endl;
	}
	while( !_getch() ) {};
	return 0;
}