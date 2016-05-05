// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/loader/ShapeLoader.h>

using namespace dxi;

void loader::ShapeLoader( const unify::Path & filePath, PrimitiveList & primitiveList, Managers managers )
{
	qxml::Document doc( filePath );
	qxml::Element * shapeNode = doc.GetRoot();
	return ShapeLoader( shapeNode, primitiveList, managers ); 
}

void loader::ShapeLoader( const qxml::Element * shapeNode, PrimitiveList & primitiveList, Managers managers )
{
	// Store convertors here statically.
    using namespace dxi::loader;
	static qxml::XMLParameters xmlParameters;
	if ( ! xmlParameters.NumberOfConvertors() )
	{
		xmlParameters.AddConvertor( "radius", qxml::XMLParameters::INodeConvertor::shared_ptr( new LexicalConvertor< float > ) );
		xmlParameters.AddConvertor( "radius2", qxml::XMLParameters::INodeConvertor::shared_ptr( new LexicalConvertor< float > ) );
		xmlParameters.AddConvertor( "majorradius", qxml::XMLParameters::INodeConvertor::shared_ptr( new LexicalConvertor< float > ) );
		xmlParameters.AddConvertor( "minorradius", qxml::XMLParameters::INodeConvertor::shared_ptr( new LexicalConvertor< float > ) );
		xmlParameters.AddConvertor( "height", qxml::XMLParameters::INodeConvertor::shared_ptr( new LexicalConvertor< float > ) );
		xmlParameters.AddConvertor( "definition", qxml::XMLParameters::INodeConvertor::shared_ptr( new LexicalConvertor< float > ) );
		xmlParameters.AddConvertor( "segments", qxml::XMLParameters::INodeConvertor::shared_ptr( new LexicalConvertor< unsigned int > ) );
		xmlParameters.AddConvertor( "count", qxml::XMLParameters::INodeConvertor::shared_ptr( new LexicalConvertor< unsigned int > ) );
        xmlParameters.AddConvertor( "type", qxml::XMLParameters::INodeConvertor::shared_ptr( new FromStringConvertor< shapes::Shape > ) );
		xmlParameters.AddConvertor( "inf", qxml::XMLParameters::INodeConvertor::shared_ptr( new ConstructorConvertor< unify::V3< float > > ) );
		xmlParameters.AddConvertor( "sup", qxml::XMLParameters::INodeConvertor::shared_ptr( new ConstructorConvertor< unify::V3< float > > ) );
		xmlParameters.AddConvertor( "center", qxml::XMLParameters::INodeConvertor::shared_ptr( new ConstructorConvertor< unify::V3< float > > ) );
		xmlParameters.AddConvertor( "size1", qxml::XMLParameters::INodeConvertor::shared_ptr( new LexicalConvertor< float > ) );
		xmlParameters.AddConvertor( "size2", qxml::XMLParameters::INodeConvertor::shared_ptr( new ConstructorConvertor< unify::Size< float > > ) );
		xmlParameters.AddConvertor( "size3", qxml::XMLParameters::INodeConvertor::shared_ptr( new ConstructorConvertor< unify::Size3< float > > ) );
		xmlParameters.AddConvertor( "ul", qxml::XMLParameters::INodeConvertor::shared_ptr( new ConstructorConvertor< unify::TexCoords > ) );
		xmlParameters.AddConvertor( "dr", qxml::XMLParameters::INodeConvertor::shared_ptr( new ConstructorConvertor< unify::TexCoords > ) );
		xmlParameters.AddConvertor( "caps", qxml::XMLParameters::INodeConvertor::shared_ptr( new BoolConvertor ) );
		xmlParameters.AddConvertor( "diffuse", qxml::XMLParameters::INodeConvertor::shared_ptr( new ConstructorConvertor< unify::Color > ) );
		xmlParameters.AddConvertor( "specular", qxml::XMLParameters::INodeConvertor::shared_ptr( new ConstructorConvertor< unify::Color > ) );
		xmlParameters.AddConvertor( "texturemode", qxml::XMLParameters::INodeConvertor::shared_ptr( new TextureModeConvertor ) );
		xmlParameters.AddConvertor( "texture", qxml::XMLParameters::INodeConvertor::shared_ptr( new TextureConvertor( managers.GetTextureManager() ) ) ); // TODO:Effect
	}
	
    unify::Parameters parameters;
	const qxml::Element * childNode = shapeNode->GetFirstChild();
	while( childNode )
	{
		xmlParameters.Convert( childNode, parameters );
		childNode = childNode->GetNext();
	}

    shapes::CreateShape( primitiveList, parameters );
}
