// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/win/DirectX.h>
#include <unify/unify.h>
#include <dxi/Usage.h>

namespace dxi
{
	class Blend
	{
	public:
		struct Effect
		{
			enum TYPE {
				Zero = D3DBLEND_ZERO,
				One = D3DBLEND_ONE,
				SrcColor = D3DBLEND_SRCCOLOR,
				InvSrcColor = D3DBLEND_INVSRCCOLOR,
				SrcAlpha = D3DBLEND_SRCALPHA,
				InvSrcAlpha = D3DBLEND_INVSRCALPHA,
				DestAlpha = D3DBLEND_DESTALPHA,
				InvDestAlpha = D3DBLEND_INVDESTALPHA,
				DestColor = D3DBLEND_DESTCOLOR,
				InvDestColor = D3DBLEND_INVDESTCOLOR,
				SrcAlphaSat = D3DBLEND_SRCALPHASAT,
				BothSrcAlpha = D3DBLEND_BOTHSRCALPHA,
				BothInvSrcAlpha = D3DBLEND_BOTHINVSRCALPHA,
				BlendFactor = D3DBLEND_BLENDFACTOR,
				InvBlendFactor = D3DBLEND_INVBLENDFACTOR,
				INVALID
			};

            static TYPE FromString( const std::string text );
		};

		static void UseDefault();

		Blend( Usage::TYPE blendUsage = Usage::False,  Blend::Effect::TYPE source = Blend::Effect::SrcColor, Blend::Effect::TYPE destination = Blend::Effect::InvSrcColor );
        explicit Blend( const std::string text );
		Blend & operator = ( const Blend & blend );
		bool operator == ( const Blend & blend ) const;
		bool operator != ( const Blend & blend ) const;

        void Use();
		void Unuse();
        bool IsSet() const;

    private:

		Usage::TYPE m_usageBackup;
		Blend::Effect::TYPE m_sourceBackup;
		Blend::Effect::TYPE m_destinationBackup;

		Usage::TYPE m_usage;
		Blend::Effect::TYPE m_source;
		Blend::Effect::TYPE m_destination;
    };
}