// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/Blend.h>
#include <unify/Unify.h>
#include <dxi/core/Game.h>
#include <unify/String.h>

using namespace dxi;

Blend::Effect::TYPE Blend::Effect::FromString( const std::string text )
{
	std::string trimmedText = unify::CleanWhitespace( text );
	if ( unify::StringIs( trimmedText,  "Zero" ) ) return Blend::Effect::Zero;
	else if ( unify::StringIs( trimmedText,  "One" ) ) return Blend::Effect::One;
	else if ( unify::StringIs( trimmedText,  "SrcColor" ) ) return Blend::Effect::SrcColor;
	else if ( unify::StringIs( trimmedText,  "InvSrcColor" ) ) return Blend::Effect::InvSrcColor;
	else if ( unify::StringIs( trimmedText,  "SrcAlpha" ) ) return Blend::Effect::SrcAlpha;
	else if ( unify::StringIs( trimmedText,  "InvSrcAlpha" ) ) return Blend::Effect::InvSrcAlpha;
	else if ( unify::StringIs( trimmedText,  "DestAlpha" ) ) return Blend::Effect::DestAlpha;
	else if ( unify::StringIs( trimmedText,  "InvDestAlpha" ) ) return Blend::Effect::InvDestAlpha;
	else if ( unify::StringIs( trimmedText,  "DestColor" ) ) return Blend::Effect::DestColor;
	else if ( unify::StringIs( trimmedText,  "InvDestColor" ) ) return Blend::Effect::InvDestColor;
	else if ( unify::StringIs( trimmedText,  "SrcAlphaSat" ) ) return Blend::Effect::SrcAlphaSat;
	else if ( unify::StringIs( trimmedText,  "BothSrcAlpha" ) ) return Blend::Effect::BothSrcAlpha;
	else if ( unify::StringIs( trimmedText,  "BothInvSrcAlpha" ) ) return Blend::Effect::BothInvSrcAlpha;
	else if ( unify::StringIs( trimmedText,  "BlendFactor" ) ) return Blend::Effect::BlendFactor;
	else if ( unify::StringIs( trimmedText,  "InvBlendFactor" ) ) return Blend::Effect::InvBlendFactor;
    else
    {
        return Blend::Effect::INVALID;
    }
}


////////////////////////////////////////////////
// Blend
////////////////////////////////////////////////

void Blend::UseDefault()
{
	win::DX::GetDxDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );
	win::DX::GetDxDevice()->SetRenderState( D3DRS_SRCBLEND,			Blend::Effect::One );
	win::DX::GetDxDevice()->SetRenderState( D3DRS_DESTBLEND,		Blend::Effect::Zero );
}

Blend::Blend( Usage::TYPE usage, Blend::Effect::TYPE source, Blend::Effect::TYPE destination )
: m_usage( usage )
, m_source( source )
, m_destination( destination )
{
}

Blend::Blend( const std::string text )
{
	std::vector< std::string > split = unify::Split< std::string >( text, ',' );
	m_usage = Usage::FromString( split[ 0 ] );
    if( split.size() > 1 )
    {
        m_source = Effect::FromString( split[ 1 ] );
        if( split.size() > 2 )
        {
            m_destination = Effect::FromString( split[ 2 ] );
        }
    }
}

Blend & Blend::operator = ( const Blend & blend )
{
	m_usage = blend.m_usage;
	m_source = blend.m_source;
	m_destination = blend.m_destination;
	return *this;
}

bool Blend::operator == ( const Blend & blend ) const
{
	if( m_usage != blend.m_usage ) return FALSE;
	if( m_source != blend.m_source ) return FALSE;
	if( m_destination != blend.m_destination ) return FALSE;
	return true;
}

bool Blend::operator != ( const Blend & blend ) const
{
	return !( *this == blend );
}

void Blend::Use()
{
	switch( m_usage )
	{
	default:
	case Usage::Ignore:
		break;
	case Usage::True:
		win::DX::GetDxDevice()->GetRenderState( D3DRS_ALPHABLENDENABLE, (DWORD*)&m_usageBackup );
		win::DX::GetDxDevice()->GetRenderState( D3DRS_SRCBLEND, (DWORD*)&m_sourceBackup );
		win::DX::GetDxDevice()->GetRenderState( D3DRS_DESTBLEND, (DWORD*)&m_destinationBackup );

		win::DX::GetDxDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
		win::DX::GetDxDevice()->SetRenderState( D3DRS_SRCBLEND, m_source );
		win::DX::GetDxDevice()->SetRenderState( D3DRS_DESTBLEND, m_destination );
		break;
	case Usage::TrueNoSet:
		win::DX::GetDxDevice()->GetRenderState( D3DRS_ALPHABLENDENABLE, (DWORD*)&m_usageBackup );
		win::DX::GetDxDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
		break;
	case Usage::False:
		win::DX::GetDxDevice()->GetRenderState( D3DRS_ALPHABLENDENABLE, (DWORD*)&m_usageBackup );
		win::DX::GetDxDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
		break;
	}
}

void Blend::Unuse()
{
	switch( m_usage )
	{
	default:
	case Usage::Ignore:
		break;
	case Usage::True:
		win::DX::GetDxDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, m_usageBackup );
		win::DX::GetDxDevice()->SetRenderState( D3DRS_SRCBLEND, m_sourceBackup );
		win::DX::GetDxDevice()->SetRenderState( D3DRS_DESTBLEND, m_destinationBackup );
		break;
	case Usage::TrueNoSet:
		// FALL THROUGH
	case Usage::False:
		win::DX::GetDxDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, m_usageBackup );
		break;
	}
}

bool Blend::IsSet() const
{
    return m_usage == Usage::True || m_usage == Usage::TrueNoSet;
}

