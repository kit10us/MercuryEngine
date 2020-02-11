// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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
	std::string casted = element->GetAttributeElse< std::string >( "enable", "true" );
	enable = unify::Cast< bool >( casted );

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
	if( unify::string::StringIs( "Zero", blend ) ) return Blend::Zero;
	else if ( unify::string::StringIs( "One", blend ) ) return Blend::One;
	else if ( unify::string::StringIs( "SrcColor", blend ) ) return Blend::SrcColor;
	else if ( unify::string::StringIs( "InvSrcColor", blend ) ) return Blend::InvSrcColor;
	else if ( unify::string::StringIs( "SrcAlpha", blend ) ) return Blend::SrcAlpha;
	else if ( unify::string::StringIs( "InvSrcAlpha", blend ) ) return Blend::InvSrcAlpha;
	else if ( unify::string::StringIs( "DestAlpha", blend ) ) return Blend::DestAlpha;
	else if ( unify::string::StringIs( "InvDestAlpha", blend ) ) return Blend::InvDestAlpha;
	else if ( unify::string::StringIs( "DestColor", blend ) ) return Blend::DestColor;
	else if ( unify::string::StringIs( "InvDestColor", blend ) ) return Blend::InvDestColor;
	else if ( unify::string::StringIs( "SrcAlphaSaturation", blend ) ) return Blend::SrcAlphaSaturation;
	else if ( unify::string::StringIs( "BlendFactor", blend ) ) return Blend::BlendFactor;
	else if ( unify::string::StringIs( "InvBlendFactor", blend ) ) return Blend::InvBlendFactor;
	else if ( unify::string::StringIs( "Src1Color", blend ) ) return Blend::Src1Color;
	else if ( unify::string::StringIs( "InSrc1Color", blend ) ) return Blend::InSrc1Color;
	else if ( unify::string::StringIs( "Src1Alpha", blend ) ) return Blend::Src1Alpha;
	else if ( unify::string::StringIs( "InSrc1Alpha", blend ) ) return Blend::InSrc1Alpha;
	throw std::exception( "Bad cast from string for Blend!" );
}

template<>
BlendOp unify::Cast( std::string blendOp )
{
	if ( unify::string::StringIs( "Add", blendOp ) ) return BlendOp::Add;
	else if ( unify::string::StringIs( "Subteact", blendOp ) ) return BlendOp::Subteact;
	else if ( unify::string::StringIs( "RevSubtract", blendOp ) ) return BlendOp::RevSubtract;
	else if ( unify::string::StringIs( "Min", blendOp ) ) return BlendOp::Min;
	else if ( unify::string::StringIs( "Max", blendOp ) ) return BlendOp::Max;
	throw std::exception( "Bad cast from string for BlendOp!" );
}

template<>
ColorWriteEnable unify::Cast( std::string in )
{
	if ( unify::string::StringIs( "Red", in ) ) return ColorWriteEnable::Red;
	else if ( unify::string::StringIs( "Green", in ) ) return ColorWriteEnable::Green;
	else if ( unify::string::StringIs( "Blue", in ) ) return ColorWriteEnable::Blue;
	else if ( unify::string::StringIs( "Alpha", in ) ) return ColorWriteEnable::Alpha;
	else if ( unify::string::StringIs( "All", in ) ) return ColorWriteEnable::All;
	throw std::exception( "Bad cast from string for ColorWriteEnable!" );
}
