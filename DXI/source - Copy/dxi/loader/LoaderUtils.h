// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/TextureManager.h>
#include <qxml/Document.h>
#include <dxi/geo/Mesh.h>
#include <qxml/XMLParameters.h>
#include <unify/ColorUnit.h>
#include <unify/Size3.h>

namespace dxi
{
    namespace loader
    {
        template< typename T >
        struct LexicalConvertor : public qxml::XMLParameters::INodeConvertor
        {
	        bool operator()( const qxml::Element * element, unify::Parameters & parameters );
        };

        template< typename T, typename T2 = T::TYPE >
        struct FromStringConvertor : public qxml::XMLParameters::INodeConvertor
        {
	        bool operator()( const qxml::Element * element, unify::Parameters & parameters );
        };

        template< typename T >
        struct ConstructorConvertor : public qxml::XMLParameters::INodeConvertor
        {
	        bool operator()( const qxml::Element * element, unify::Parameters & parameters );
        };

        struct TextureModeConvertor : public qxml::XMLParameters::INodeConvertor
        {
	        bool operator()( const qxml::Element * element, unify::Parameters & parameters );
        };

        struct TextureConvertor : public qxml::XMLParameters::INodeConvertor
        {
	        std::weak_ptr< TextureManager > m_textureManager;
	        TextureConvertor( std::weak_ptr< TextureManager > textureManager );
	        bool operator()( const qxml::Element * element, unify::Parameters & parameters );
        };

        struct BoolConvertor : public qxml::XMLParameters::INodeConvertor
        {
	        bool operator()( const qxml::Element * element, unify::Parameters & parameters );
        };

        #include <dxi/loader/LoaderUtils.inl>
    }
}