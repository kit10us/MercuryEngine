// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <ase/ASEDocument.h>
#include <unify/Path.h>
#include <unify/String.h>
#include <regex>

using namespace ase;

void ProcessASEElement( qxml::Element * pElement, std::string text );

void Document::Load( const unify::Path & filePath )
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

	// Our initial parent is the root.
	qxml::Element * parent = AddElement( new qxml::Element( "Root", qxml::Element::NodeType::Element, nullptr ) , "Root" );
	qxml::Element * previous = parent;

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
	size_t depth = 0;

	while( !stream.EndOfStream() )
	{
		stream.ReadPack( &strData, 256 );
					  
		size_t newDepth {};
		for( newDepth = 0; strData[newDepth] == '\t'; ++newDepth );

		if ( newDepth > depth )
		{
			parent = previous;
			depth = newDepth;
		}

		while( newDepth < depth )
		{
			parent = parent->GetParent();
			--depth;
		}

		sData = unify::TrimLeft( strData, '\t' );

		// Check that this is a valid element...
		if( sData[ 0 ] != '*' ) 
		{
			continue;
		}

		std::string name = unify::StringMinusLeft( unify::ListPart( sData, {' ', '\t' }, 0 ), 1 );
		qxml::Element * element = new qxml::Element( name, qxml::Element::NodeType::Element, nullptr );
		previous = element;
		AddElement( element, name );
		parent->TakeChild( element );

		// Ignore a curly brace'd element...
		if( unify::RightString( sData, 1 ) != "{" )
		{
			std::string text = std::regex_replace( sData, std::regex{ "^[*]" + name + "(\t|[ ])*" }, "" );
			
			// Allow modifying text...
			ProcessASEElement( element, text );
		}
	}
}

void ProcessASEElement( qxml::Element * pElement, std::string text )
{
	const std::string & tagName = pElement->GetTagName();
	if( unify::StringIs( tagName, "SCENE_AMBIENT_STATIC" ) )
	{
		text = unify::CleanWhitespace( text );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "r", unify::ListPart( text, {' ', '\t'}, 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "g", unify::ListPart( text, {' ', '\t'}, 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "b", unify::ListPart( text, {' ', '\t'}, 2 ) ) ) );
	}
	else if( unify::StringIs( tagName, "INHERIT_POS" ) )
	{
		text = unify::CleanWhitespace( text );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "x", unify::ListPart( text, {' ', '\t'}, 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "y", unify::ListPart( text, {' ', '\t'}, 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "z", unify::ListPart( text, {' ', '\t'}, 2 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MESH_VERTEX" ) )
	{
		text = unify::CleanWhitespace( text );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "index", unify::ListPart( text, {' ', '\t'}, 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "x", unify::ListPart( text, {' ', '\t'}, 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "y", unify::ListPart( text, {' ', '\t'}, 2 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "z", unify::ListPart( text, {' ', '\t'}, 3 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MESH_VERTEXNORMAL" ) )
	{
		text = unify::CleanWhitespace( text );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "index", unify::ListPart( text, {' ', '\t'}, 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "x", unify::ListPart( text, {' ', '\t'}, 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "y", unify::ListPart( text, {' ', '\t'}, 2 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "z", unify::ListPart( text, {' ', '\t'}, 3 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MESH_FACENORMAL" ) )
	{
		text = unify::CleanWhitespace( text );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "index", unify::ListPart( text, {' ', '\t'}, 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "x", unify::ListPart( text, {' ', '\t'}, 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "y", unify::ListPart( text, {' ', '\t'}, 2 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "z", unify::ListPart( text, {' ', '\t'}, 3 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MESH_TVERT" ) )
	{
		text = unify::CleanWhitespace( text );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "index", unify::ListPart( text, {' ', '\t'}, 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "u", unify::ListPart( text, {' ', '\t'}, 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "v", unify::ListPart( text, {' ', '\t'}, 2 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "w", unify::ListPart( text, {' ', '\t'}, 3 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MESH_TFACE" ) )
	{
		text = unify::CleanWhitespace( text );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "index", unify::ListPart( text, {' ', '\t'}, 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "A", unify::ListPart( text, {' ', '\t'}, 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "B", unify::ListPart( text, {' ', '\t'}, 2 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "C", unify::ListPart( text, {' ', '\t'}, 3 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MESH_FACE" ) )
	{
		text = unify::CleanWhitespace( text );
		std::string sIndex = unify::StringMinusRight( unify::ListPart( text, {' ', '\t'}, 0 ), 1 );
		std::string sA = unify::ListPart( text, {' ', '\t'}, 2 );
		std::string sB = unify::ListPart( text, {' ', '\t'}, 4 );
		std::string sC = unify::ListPart( text, {' ', '\t'}, 6 );
		std::string sAB = unify::ListPart( text, {' ', '\t'}, 8 );
		std::string sBC = unify::ListPart( text, {' ', '\t'}, 10 );
		std::string sCA = unify::ListPart( text, {' ', '\t'}, 12 );
		std::string sMS = unify::ListPart( text, {' ', '\t'}, 14 );
		std::string sMM = unify::ListPart( text, {' ', '\t'}, 16 );
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
		text = unify::CleanWhitespace( text );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "R", unify::ListPart( text, {' ', '\t'}, 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "G", unify::ListPart( text, {' ', '\t'}, 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "B", unify::ListPart( text, {' ', '\t'}, 2 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MATERIAL_DIFFUSE" ) )
	{
		text = unify::CleanWhitespace( text );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "R", unify::ListPart( text, {' ', '\t'}, 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "G", unify::ListPart( text, {' ', '\t'}, 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "B", unify::ListPart( text, {' ', '\t'}, 2 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MATERIAL_SPECULAR" ) )
	{
		text = unify::CleanWhitespace( text );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "R", unify::ListPart( text, {' ', '\t'}, 0 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "G", unify::ListPart( text, {' ', '\t'}, 1 ) ) ) );
		pElement->AppendAttribute( qxml::Attribute::shared_ptr( new qxml::Attribute( "B", unify::ListPart( text, {' ', '\t'}, 2 ) ) ) );
	}
	else if( unify::StringIs( tagName, "MATERIAL_NAME" ) || unify::StringIs( tagName, "MATERIAL_CLASS" ) || unify::StringIs( tagName, "MAP_NAME" ) || unify::StringIs( tagName, "BITMAP" ))
	{
		pElement->AddText( unify::Trim( text, '\"' ) );
	}
	else
	{
		pElement->AddText( text );
	}
}
