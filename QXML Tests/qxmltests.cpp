#include <iostream>
#include <qxml/ExportDocument.h>

int main( char *, int )
{
	using namespace std;
	using namespace qxml;

	cout << "XML Tests being..." << endl;

	{		
		cout << "Test write XML document";

		Document doc;
		auto * root = new Element( "root", Element::NodeType::Element, &doc ) ;

		doc.AddElement( root );
		
		auto * first = root->AddElement( "first" );
		
		auto * second = root->AddElement( "second" );
		second->AddAttribute( "attr1", "1234" );

		auto * third = root->AddElement( "third" );
		third->AddAttribute( "attr1", "1234" );
		third->AddAttribute( "attr2", "Longer String" );
		auto * third_first = third->AddElement( "third_first", Element::NodeType::Element );
		third_first->AddAttribute( "attr1", "1234" );

		auto * fourth = root->AddCData( "Line 1\nLine 2\nLine 3 - with </node_like_text>" );

		ExportDocument docout( unify::Path( "testwrite.xml" ) );
		docout.WriteDoc( doc );
		docout.Close();
	}

	cout << "...XML Test done." << endl;

	return 0;
}