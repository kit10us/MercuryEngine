// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/render/Blend.h>
#include <unify/String.h>

using namespace me;
using namespace render;

ColorWriteEnable me::render::operator | ( ColorWriteEnable l, ColorWriteEnable r )
{
	return (ColorWriteEnable)( (unsigned char)l | (unsigned char)r );
}

BlendDesc::BlendDesc()
	: enable{ false }
	, src{ Blend::One }
	, dest{ Blend::Zero }
	, op{ BlendOp::Add }
	, srcAlpha{ Blend::One }
	, destAlpha{ Blend::Zero }
	, opAlpha{ BlendOp::Add }
	, renderTargetWriteMask{ ColorWriteEnable::All }
{
}

BlendDesc::BlendDesc( const qxml::Element * element )
	: BlendDesc()
{
	enable = unify::Cast< bool >( element->GetAttributeElse( "enable", "true" ) );

	if( element->HasElements( "src" ) )
	{
		src = unify::Cast< Blend >( element->GetElement( "src" )->GetText() );
	}
	if( element->HasElements( "dest" ) )
	{
		dest = unify::Cast< Blend >( element->GetElement( "dest" )->GetText() );
	}
	if( element->HasElements( "op" ) )
	{
		op = unify::Cast< BlendOp >( element->GetElement( "op" )->GetText() );
	}
	if( element->HasElements( "srcalpha" ) )
	{
		srcAlpha = unify::Cast< Blend >( element->GetElement( "srcalpha" )->GetText() );
	}
	if( element->HasElements( "destalpha" ) )
	{
		destAlpha = unify::Cast< Blend >( element->GetElement( "destalpha" )->GetText() );
	}
	if( element->HasElements( "opalpha" ) )
	{
		opAlpha = unify::Cast< BlendOp >( element->GetElement( "opalpha" )->GetText() );
	}
}


template<>
Blend unify::Cast( std::string blend )
{
	if( unify::StringIs( "Zero", blend ) ) return Blend::Zero;
	else if ( unify::StringIs( "One", blend ) ) return Blend::One;
	else if ( unify::StringIs( "SrcColor", blend ) ) return Blend::SrcColor;
	else if ( unify::StringIs( "InvSrcColor", blend ) ) return Blend::InvSrcColor;
	else if ( unify::StringIs( "SrcAlpha", blend ) ) return Blend::SrcAlpha;
	else if ( unify::StringIs( "InvSrcAlpha", blend ) ) return Blend::InvSrcAlpha;
	else if ( unify::StringIs( "DestAlpha", blend ) ) return Blend::DestAlpha;
	else if ( unify::StringIs( "InDestAlpha", blend ) ) return Blend::InDestAlpha;
	else if ( unify::StringIs( "DestColor", blend ) ) return Blend::DestColor;
	else if ( unify::StringIs( "InvDestColor", blend ) ) return Blend::InvDestColor;
	else if ( unify::StringIs( "SrcAlphaSaturation", blend ) ) return Blend::SrcAlphaSaturation;
	else if ( unify::StringIs( "BlendFactor", blend ) ) return Blend::BlendFactor;
	else if ( unify::StringIs( "InvBlendFactor", blend ) ) return Blend::InvBlendFactor;
	else if ( unify::StringIs( "Src1Color", blend ) ) return Blend::Src1Color;
	else if ( unify::StringIs( "InSrc1Color", blend ) ) return Blend::InSrc1Color;
	else if ( unify::StringIs( "Src1Alpha", blend ) ) return Blend::Src1Alpha;
	else if ( unify::StringIs( "InSrc1Alpha", blend ) ) return Blend::InSrc1Alpha;
	throw std::exception( "Bad cast from string for Blend!" );
}

template<>
BlendOp unify::Cast( std::string blendOp )
{
	if ( unify::StringIs( "Add", blendOp ) ) return BlendOp::Add;
	else if ( unify::StringIs( "Subteact", blendOp ) ) return BlendOp::Subteact;
	else if ( unify::StringIs( "RevSubtract", blendOp ) ) return BlendOp::RevSubtract;
	else if ( unify::StringIs( "Min", blendOp ) ) return BlendOp::Min;
	else if ( unify::StringIs( "Max", blendOp ) ) return BlendOp::Max;
	throw std::exception( "Bad cast from string for BlendOp!" );
}

template<>
ColorWriteEnable unify::Cast( std::string in )
{
	if ( unify::StringIs( "Red", in ) ) return ColorWriteEnable::Red;
	else if ( unify::StringIs( "Green", in ) ) return ColorWriteEnable::Green;
	else if ( unify::StringIs( "Blue", in ) ) return ColorWriteEnable::Blue;
	else if ( unify::StringIs( "Alpha", in ) ) return ColorWriteEnable::Alpha;
	else if ( unify::StringIs( "All", in ) ) return ColorWriteEnable::All;
	throw std::exception( "Bad cast from string for ColorWriteEnable!" );
}
