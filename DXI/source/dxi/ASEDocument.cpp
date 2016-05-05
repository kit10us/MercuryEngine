// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/ASEDocument.h>
#include <unify/Path.h>
#include <unify/String.h>

using namespace dxi;

void ASEDocument::Load( const unify::Path & filePath )
{
	// Destroy existing document
	Destroy();

	if( ! filePath.Exists() )
	{
		throw unify::Exception( filePath.ToString() + ": XML File not found!" );
	}

	unify::FileStream stream;
	bool bInElement = false;
	bool bInComment = false;
	std::string sElementName;
	unsigned int uAttribute = 0;
	qxml::Attribute * pAttribute = 0;
	qxml::Element * pElement = 0;

	// Our initial parent is the root.
	qxml::Element * pParent = AddElement( new qxml::Element( "Root", qxml::Element::NodeType::Element, nullptr ) , "Root" );

	if( ! stream.Open( unify::StreamAccessType::STREAMACCESS_READ, (void *)filePath.ToString().c_str() ) )
	{
		throw unify::Exception( "Failed to open document!" );
	}

	m_filePath = filePath;

	// Loop until we find an element, all else is an error (except cr, nl, spaces, tabs)
	std::string sData;
	char cQuotes = ' ';
	bool bInCData = false;
	char strData[ 256 ];
	std::string sText = "";
	while( !stream.EndOfStream() )
	{
		stream.ReadPack( &strData, 256 );
		sData = unify::CleanWhitespace( strData );
		sData = unify::Trim( sData, ' ' );

		// Check if this is the end of the parent...
		if( sData == "}" )
		{
			pParent = pParent->GetParent();
		}

		// Check that this is a valid element...
		else if( sData[ 0 ] == '*' ) 
		{
			std::string sName = unify::StringMinusLeft( unify::ListPart( sData, ' ', 0 ), 1 );
			qxml::Element * pElement = new qxml::Element( sName, qxml::Element::NodeType::Element, nullptr );
			AddElement( pElement, sName );
			pParent->TakeChild( pElement );

			// Check if this is a parent...
			if( unify::RightString( sData, 1 ) == "{" )
			{
				pParent = pElement;
			}

			// 
			else
			{
				std::string sText = unify::StringMinusLeft( sData, (int)sName.length() + 1 );
				sText = unify::Trim( sText, ' ' );
				
				// Allow modifying text...
				ProcessASEElement( pElement, sText );
			}
		}
	}
}

void ASEDocument::ProcessASEElement( qxml::Element * pElement, const std::string & sText )
{
	const std::string & tagName = pElement->GetTagName();
	if( unify::StringIs( tagName, "SCENE_AMBIENT_STATIC" ) )
	{
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "r", unify::ListPart( sText, ' ', 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "g", unify::ListPart( sText, ' ', 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "b", unify::ListPart( sText, ' ', 2 ) ) ) );
	}
	else if( unify::StringIs( tagName, "INHERIT_POS" ) )
	{
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "x", unify::ListPart( sText, ' ', 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "y", unify::ListPart( sText, ' ', 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "z", unify::ListPart( sText, ' ', 2 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MESH_VERTEX" ) )
	{
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "index", unify::ListPart( sText, ' ', 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "x", unify::ListPart( sText, ' ', 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "y", unify::ListPart( sText, ' ', 2 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "z", unify::ListPart( sText, ' ', 3 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MESH_VERTEXNORMAL" ) )
	{
		//MESH_VERTEXNORMAL    0	-5.0000	-5.0000	0.0000
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "index", unify::ListPart( sText, ' ', 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "x", unify::ListPart( sText, ' ', 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "y", unify::ListPart( sText, ' ', 2 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "z", unify::ListPart( sText, ' ', 3 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MESH_FACENORMAL" ) )
	{
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "index", unify::ListPart( sText, ' ', 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "x", unify::ListPart( sText, ' ', 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "y", unify::ListPart( sText, ' ', 2 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "z", unify::ListPart( sText, ' ', 3 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MESH_TVERT" ) )
	{
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "index", unify::ListPart( sText, ' ', 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "u", unify::ListPart( sText, ' ', 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "v", unify::ListPart( sText, ' ', 2 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "w", unify::ListPart( sText, ' ', 3 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MESH_TFACE" ) )
	{
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "index", unify::ListPart( sText, ' ', 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "A", unify::ListPart( sText, ' ', 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "B", unify::ListPart( sText, ' ', 2 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "C", unify::ListPart( sText, ' ', 3 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MESH_FACE" ) )
	{
		std::string sIndex = unify::StringMinusRight( unify::ListPart( sText, ' ', 0 ), 1 );
		std::string sA = unify::ListPart( sText, ' ', 2 );
		std::string sB = unify::ListPart( sText, ' ', 4 );
		std::string sC = unify::ListPart( sText, ' ', 6 );
		std::string sAB = unify::ListPart( sText, ' ', 8 );
		std::string sBC = unify::ListPart( sText, ' ', 10 );
		std::string sCA = unify::ListPart( sText, ' ', 12 );
		std::string sMS = unify::ListPart( sText, ' ', 14 );
		std::string sMM = unify::ListPart( sText, ' ', 16 );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "index", sIndex ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "A", sA ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "B", sB ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "C", sC ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "AB", sAB ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "BC", sBC ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "CA", sCA ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "MESH_SMOOTHING", sMS ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "MESH_MTLID", sMM ) ) );
	}
	else if( unify::StringIs( tagName, "MATERIAL_AMBIENT" ) )
	{
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "R", unify::ListPart( sText, ' ', 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "G", unify::ListPart( sText, ' ', 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "B", unify::ListPart( sText, ' ', 2 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MATERIAL_DIFFUSE" ) )
	{
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "R", unify::ListPart( sText, ' ', 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "G", unify::ListPart( sText, ' ', 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "B", unify::ListPart( sText, ' ', 2 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MATERIAL_SPECULAR" ) )
	{
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "R", unify::ListPart( sText, ' ', 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "G", unify::ListPart( sText, ' ', 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "B", unify::ListPart( sText, ' ', 2 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MATERIAL_NAME" ) || unify::StringIs( tagName, "MATERIAL_CLASS" ) || unify::StringIs( tagName, "MAP_NAME" ) || unify::StringIs( tagName, "BITMAP" ))
	{
		pElement->AddText( unify::Trim( sText, '\"' ) );
	}
	else
	{
		pElement->AddText( sText );
	}
}
