// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/RenderInfo.h>

using namespace dxi;

RenderInfo RenderInfo::RenderInfoStandalone()
{
	RenderInfo renderInfo;
	return renderInfo;
}

RenderInfo::RenderInfo()
: m_distanceFromCamera( 0.0f )
, m_frameID( 0 )
, m_final( unify::Matrix::MatrixIdentity() )
, m_frameSetInstance( 0 )
{
}

RenderInfo::RenderInfo( const RenderInfo & info )
: m_distanceFromCamera( 0.0f )
, m_frameID( 0 )
, m_optionIsSet( info.m_optionIsSet )
, m_optionValue( info.m_optionValue )
, m_final( info.m_final )
, m_frameSetInstance( info.m_frameSetInstance )
{
}

RenderInfo::~RenderInfo() throw ()
{
}

bool RenderInfo::IsOptionSet( RenderOption::TYPE option ) const
{
	return m_optionIsSet.test( option );
}

bool RenderInfo::IsOptionTrue( RenderOption::TYPE option ) const
{
	return m_optionValue.test( option );
}

bool RenderInfo::IsOptionFalse( RenderOption::TYPE option ) const
{
	return ! m_optionValue.test( option );
}

void RenderInfo::SetOption( RenderOption::TYPE option, bool enabled )
{
	m_optionIsSet[ option ] = 1;
	m_optionValue[ option ] = enabled ? 1 : 0;
}

void RenderInfo::ClearOption( RenderOption::TYPE option )
{
	m_optionIsSet[ option ] = 0;
}	

void RenderInfo::SetDistanceFromCamera( float distance )
{
	m_distanceFromCamera = distance;
}

float RenderInfo::DistanceFromCamera() const
{
	return m_distanceFromCamera;
}

RenderInfo & RenderInfo::operator=( const RenderInfo & info )
{
	m_optionIsSet = info.m_optionIsSet;
	m_optionValue = info.m_optionValue;
	m_distanceFromCamera = info.m_distanceFromCamera;
	m_frameID = info.m_frameID;
	m_final = info.m_final;
	return *this;
}

RenderInfo & RenderInfo::operator+=( const RenderInfo & info )
{
	// Combine render flags. 
	// If info -in- is set, it's value superceeds member value. An or operation.
	if( info.m_optionIsSet.any() )
	{
		for( int option = 0; option < (int)RenderOption::COUNT; ++option )
		{
			if( info.m_optionIsSet[ option ] == 1 )
			{
				m_optionIsSet[ option ] = 1;
				m_optionValue[ option ] = info.m_optionValue[ option ];
			}
		}
	}
	m_distanceFromCamera = info.m_distanceFromCamera;
	m_frameID = info.m_frameID;
	m_final *= info.m_final;
	return *this;
}

RenderInfo RenderInfo::operator+( const RenderInfo & info )
{
	RenderInfo newInfo = *this;
	newInfo += info;
	return newInfo;
}

unsigned int RenderInfo::FrameID() const
{
	return m_frameID;
}

void RenderInfo::IncrementFrameID()
{
	++m_frameID;
}

void RenderInfo::SetFinalMatrix( const unify::Matrix & matrix )
{
	m_final = matrix;
}

const unify::Matrix & RenderInfo::GetFinalMatrix() const
{
	return m_final;
}

void RenderInfo::SetFrameSetInstance( const unify::FrameSetInstance * frameSetInstance )
{
	m_frameSetInstance = frameSetInstance;
}

const unify::FrameSetInstance * RenderInfo::GetFrameSetInstance() const
{
	return m_frameSetInstance;
}
