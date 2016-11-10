// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/RenderInfo.h>

using namespace me;

RenderInfo::RenderInfo()
: m_renderer( 0 )
, m_distanceFromCamera( 0.0f )
, m_frameID( 0 )
, m_world( unify::MatrixIdentity() )
, m_view( unify::MatrixIdentity() )
, m_projection( unify::MatrixIdentity() )
, m_frameSetInstance( 0 )
, m_delta{}
, m_totalDelta{}
, m_deltaCount{}
{
}

RenderInfo::RenderInfo( const RenderInfo & info )
: m_renderer( info.m_renderer )
, m_distanceFromCamera( 0.0f )
, m_frameID( 0 )
, m_optionIsSet( info.m_optionIsSet )
, m_optionValue( info.m_optionValue )
, m_world( info.m_world )
, m_view( info.m_view )
, m_projection( info.m_projection )
, m_frameSetInstance( info.m_frameSetInstance )
, m_delta{ info.m_delta }
, m_totalDelta{ info.m_totalDelta}
, m_deltaCount{ info.m_deltaCount }
{
}

RenderInfo::~RenderInfo() throw ()
{
}

void RenderInfo::SetRenderer( me::IRenderer * renderer )
{
	m_renderer = renderer;
}

me::IRenderer * RenderInfo::GetRenderer()
{
	return m_renderer;
}

const me::IRenderer * RenderInfo::GetRenderer() const
{
	return m_renderer;
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
	m_world = info.m_world;
	m_view = info.m_view;
	m_projection = info.m_projection;
	return *this;
}

unsigned int RenderInfo::FrameID() const
{
	return m_frameID;
}

void RenderInfo::IncrementFrameID()
{
	++m_frameID;
}

float RenderInfo::GetDelta() const
{
	return m_delta;
}

void RenderInfo::SetDelta( float delta )
{
	m_delta = delta;
	m_totalDelta += delta;
	m_deltaCount++;
}

float RenderInfo::GetTotalDelta() const
{
	return m_totalDelta / m_deltaCount;
}

float RenderInfo::GetDeltaCount() const
{
	return (float)m_deltaCount;
}

float RenderInfo::GetAverageDelta() const
{
	return m_totalDelta / m_deltaCount;
}

float RenderInfo::GetFPS() const
{
	return 1.0f / GetAverageDelta();
}
		 
void RenderInfo::SetWorldMatrix( const unify::Matrix & matrix )
{
	m_world = matrix;
}

const unify::Matrix & RenderInfo::GetWorldMatrix() const
{
	return m_world;
}

void RenderInfo::SetViewMatrix( const unify::Matrix & matrix )
{
	m_view = matrix;
}

const unify::Matrix & RenderInfo::GetViewMatrix() const
{
	return m_view;
}

void RenderInfo::SetProjectionMatrix( const unify::Matrix & matrix )
{
	m_projection = matrix;
}

const unify::Matrix & RenderInfo::GetProjectionMatrix() const
{
	return m_projection;
}

void RenderInfo::SetFrameSetInstance( const unify::FrameSetInstance * frameSetInstance )
{
	m_frameSetInstance = frameSetInstance;
}

const unify::FrameSetInstance * RenderInfo::GetFrameSetInstance() const
{
	return m_frameSetInstance;
}
