// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/input/StickCondition.h>
#include <Windows.h>

using namespace me;
using namespace input;

StickCondition::StickCondition( size_t subSource, std::string name, unify::V3< unify::Range< float > > low, unify::V3< unify::Range< float > > high )
	: InputCondition( subSource, name )
	, m_low{ low }
	, m_high{ high }
{
}

StickCondition::~StickCondition()
{
}
										
bool StickCondition::IsTrue( IInputDevice* device ) const
{
	IData::ptr data = device->GetInputData( GetSubSource(), GetName() );
	if ( ! data )
	{
		return false;
	}
	
	if( data->type != InputType::Stick )
	{
		throw std::exception( "StickCondition is invalid!" );
	}

	StickData * stickData = reinterpret_cast<StickData *>(data.get());
	if( m_low.x.IsWithin( stickData->axis.x ) || m_high.x.IsWithin( stickData->axis.x ) ||
		m_low.y.IsWithin( stickData->axis.y ) || m_high.y.IsWithin( stickData->axis.y ) ||
		m_low.z.IsWithin( stickData->axis.z ) || m_high.z.IsWithin( stickData->axis.z ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

unify::V3< float > StickCondition::GetValue( IInputDevice* device ) const
{
	IData::ptr data = device->GetInputData( GetSubSource(), GetName() );
	if ( ! data )
	{
		return unify::V3< float >( 0.0f );
	}

	if ( data->type != InputType::Stick )
	{
		throw std::exception( "StickCondition is invalid!" );
	}

	StickData * stickData = reinterpret_cast<StickData *>(data.get());

	if( m_low.x.IsWithin( stickData->axis.x ) || m_high.x.IsWithin( stickData->axis.x ) || 
		m_low.y.IsWithin( stickData->axis.y ) || m_high.y.IsWithin( stickData->axis.y ) ||
		m_low.z.IsWithin( stickData->axis.z ) || m_high.z.IsWithin( stickData->axis.z ) )
	{
		return stickData->axis;
	}
	else
	{
		return unify::V3< float >( 0.0f );
	}
}																			 
