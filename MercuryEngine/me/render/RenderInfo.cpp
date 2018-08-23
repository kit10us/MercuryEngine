// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/render/RenderInfo.h>

using namespace me;
using namespace render;

RenderInfo::RenderInfo()
: m_distanceFromCamera( 0.0f )
, m_frameID( 0 )
, m_view( unify::MatrixIdentity() )
, m_projection( unify::MatrixIdentity() )
, m_frameSetInstance( 0 )
, m_delta{}
, m_totalDelta{}
, m_deltaCount{}
{
}

RenderInfo::RenderInfo( const RenderInfo & info )
: m_distanceFromCamera( 0.0f )
, m_frameID( 0 )
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
	m_distanceFromCamera = info.m_distanceFromCamera;
	m_frameID = info.m_frameID;
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

unify::TimeDelta RenderInfo::GetDelta() const
{
	return m_delta;
}

void RenderInfo::SetDelta( unify::TimeDelta delta )
{
	m_delta = delta;
	m_totalDelta += delta;
	m_deltaCount++;
}

unify::TimeDelta RenderInfo::GetTotalDelta() const
{
	return m_totalDelta / (float)m_deltaCount;
}

float RenderInfo::GetDeltaCount() const
{
	return (float)m_deltaCount;
}

unify::TimeDelta RenderInfo::GetAverageDelta() const
{
	return m_totalDelta / (float)m_deltaCount;
}

float RenderInfo::GetFPS() const
{
	return 1.0f / GetAverageDelta().GetMS();
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
