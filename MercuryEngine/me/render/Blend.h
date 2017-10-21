// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <qxml/Element.h>
#include <unify/Cast.h>

namespace me
{
	namespace render
	{
		enum class Blend
		{
			Zero = 1,
			One = 2,
			SrcColor = 3,
			InvSrcColor = 4,
			SrcAlpha = 5,
			InvSrcAlpha = 6,
			DestAlpha = 7,
			InvDestAlpha = 8,
			DestColor = 9,
			InvDestColor = 10,
			SrcAlphaSaturation = 11,
			BlendFactor = 14,
			InvBlendFactor = 15,
			Src1Color = 16,
			InSrc1Color = 17,
			Src1Alpha = 18,
			InSrc1Alpha = 19
		};

		enum class BlendOp
		{
			Add = 1,
			Subteact = 2,
			RevSubtract = 3,
			Min = 4,
			Max = 5
		};

		enum class ColorWriteEnable : unsigned char
		{
			Red = 1,
			Green = 2,
			Blue = 4,
			Alpha = 8,
			All = ( ( ( ColorWriteEnable::Red | ColorWriteEnable::Green ) | ColorWriteEnable::Blue ) | ColorWriteEnable::Alpha )
		};

		ColorWriteEnable operator | ( ColorWriteEnable l, ColorWriteEnable r );

		struct BlendDesc
		{
			BlendDesc();
			BlendDesc( const qxml::Element * element );

			bool enable;
			Blend src;
			Blend dest;
			BlendOp op;
			Blend srcAlpha;
			Blend destAlpha;
			BlendOp opAlpha;
			ColorWriteEnable renderTargetWriteMask;
		};
	}
}

template<> me::render::Blend unify::Cast( std::string blend );

template<> me::render::BlendOp unify::Cast( std::string blend );
	
template<> me::render::ColorWriteEnable unify::Cast( std::string blend );
