// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <CameraMotivator.h>
#include <unify/Angle.h>
#include <unify/V3.h>
#include <unify/Quaternion.h>

using namespace me;
using namespace motivator;
using namespace input;

CameraMotivator::CameraMotivator( const CameraMotivator & cameraMotivator )
	: ObjectInputMotivator( cameraMotivator )
{
}

CameraMotivator::CameraMotivator()
	: ObjectInputMotivator()
{
	m_typeName = "CameraMotivator";
	GetLookup()->AddValue( "speed", "2.0" );
}

CameraMotivator::~CameraMotivator() 
{
}

std::string CameraMotivator::GetWhat() const
{
	return std::string();
}								

void CameraMotivator::OnUpdate( UpdateParams params )
{
	if ( ! IsEnabled() )
	{
		return;
	}								 

	// Digital movement...
	auto && moveLeftMotivation = GetMotivator()->GetCondition( "moveleft" );
	auto && moveRightMotivation = GetMotivator()->GetCondition( "moveright" );
	auto && moveUpMotivation = GetMotivator()->GetCondition( "moveup" );
	auto && moveDownMotivation = GetMotivator()->GetCondition( "movedown" );

	float speed = unify::Cast< float >( GetValue( "speed" ) );

	using namespace unify;
	if ( moveLeftMotivation && moveLeftMotivation->IsTrue() )
	{
		float factor = params.renderInfo.GetDelta() * speed;
		GetObject()->GetFrame().SetRotation( QuaternionIdentity() );
		unify::V3< float > position = GetObject()->GetFrame().GetPosition();
		position += unify::V3< float >( -factor, 0, 0 );
		GetObject()->GetFrame().SetPosition( position );
		GetObject()->GetFrame().SetRotation( unify::MatrixRotationY( unify::AngleInDegrees( -90 ) ) );
	}
	else if ( moveRightMotivation && moveRightMotivation->IsTrue() )
	{
		float factor = params.renderInfo.GetDelta() * speed;
		GetObject()->GetFrame().SetRotation( QuaternionIdentity() );
		unify::V3< float > position = GetObject()->GetFrame().GetPosition();
		position += unify::V3< float >( factor, 0, 0 );
		GetObject()->GetFrame().SetPosition( position );
		GetObject()->GetFrame().SetRotation( unify::MatrixRotationY( unify::AngleInDegrees( 90 ) ) );
	}

	if ( moveUpMotivation && moveUpMotivation->IsTrue() )
	{
		float factor = params.renderInfo.GetDelta() * speed;
		GetObject()->GetFrame().SetRotation( QuaternionIdentity() );
		unify::V3< float > position = GetObject()->GetFrame().GetPosition();
		position += unify::V3< float >( 0, 0, factor );
		GetObject()->GetFrame().SetPosition( position );
		GetObject()->GetFrame().SetRotation( unify::MatrixRotationY( unify::AngleInDegrees( 0 ) ) );
	}
	else if ( moveDownMotivation && moveDownMotivation->IsTrue() )
	{
		float factor = params.renderInfo.GetDelta() * speed;
		GetObject()->GetFrame().SetRotation( QuaternionIdentity() );
		unify::V3< float > position = GetObject()->GetFrame().GetPosition();
		position += unify::V3< float >( 0, 0, -factor );
		GetObject()->GetFrame().SetPosition( position );
		GetObject()->GetFrame().SetRotation( unify::MatrixRotationY( unify::AngleInDegrees( 180 ) ) );
	}
}

scene::IObjectComponent * CameraMotivator::Duplicate()
{
	return new CameraMotivator( *this );
}