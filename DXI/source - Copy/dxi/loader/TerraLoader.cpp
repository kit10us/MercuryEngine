// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/loader/TerraLoader.h>

using namespace dxi;
	
geo::Terra * loader::TerraLoader( const qxml::Element * node, TextureManager::shared_ptr textureManager, EffectManager::shared_ptr effectManager )
{
    using namespace dxi::loader;
	
	geo::Terra::Parameters parameters;
    parameters.Set< unify::Size< float > >( "size", unify::Size< float >( node->GetStringAttribute( "size" ) ) );
    parameters.Set< unify::RowColumn< unsigned int > >( "rowscolumns", unify::RowColumn< unsigned int >( node->GetStringAttribute( "rowscolumns" ) ) );
    parameters.Set< float >( "constant", node->GetFloatAttribute( "constant" ) );
    parameters.Set< unify::TexArea >( "texarea", unify::TexArea( node->GetStringAttribute( "texarea" ) ) );
    
	const qxml::Element * childNode = node->GetFirstChild();
	while( childNode )
    {
        if ( unify::StringIs( childNode->GetTagName(), "heightmap" ) )
        {
            assert( textureManager );
            std::string name = childNode->GetStringAttribute( "name" );
            Texture::shared_ptr texture = textureManager->Find( name );
			unify::ColorUnit colorOp( childNode->GetStringAttribute( "colorop" ) );
            unify::TexArea texArea( childNode->GetStringAttributeElse( "texarea", "full" ) );
			parameters.SetHeightMap( geo::Terra::TextureOpMap( texture, colorOp, texArea ) );
        }
        else if ( unify::StringIs( childNode->GetTagName(), "alphamap" ) )
        {
            assert( textureManager );
            std::string name = childNode->GetStringAttribute( "name" );
            Texture::shared_ptr texture = textureManager->Find( name );
			unify::ColorUnit colorOp( childNode->GetStringAttribute( "colorop" ) );
            unify::TexArea texArea( childNode->GetStringAttributeElse( "texarea", "full" ) );
			parameters.SetAlphaMap( geo::Terra::TextureOpMap( texture, colorOp, texArea ) );
        }
        else if ( unify::StringIs( childNode->GetTagName(), "effect" ) )
        {
            assert( effectManager );
            int index = childNode->GetIntegerAttributeElse( "index", 0 );
			std::string name = childNode->GetStringAttribute( "name" );
			parameters.SetEffect( effectManager->Find( name ) );
        }

        childNode = childNode->GetNext();
	}

    geo::Terra * terra = new geo::Terra( parameters );

	return terra;
}

geo::Terra::shared_ptr loader::TerraLoader( const qxml::Element * node, GeometryManager::shared_ptr geometryManager, TextureManager::shared_ptr textureManager, EffectManager::shared_ptr effectManager )
{
	std::string name = node->GetStringAttribute( "name" );
	if ( geometryManager->Exists( name ) )
	{
		return geometryManager->Find( name );
	}

	geo::Terra * terra = TerraLoader( node, textureManager, effectManager );
	return geometryManager->Add( name, terra );
}