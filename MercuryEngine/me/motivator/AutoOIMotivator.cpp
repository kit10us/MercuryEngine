// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/motivator/AutoOIMotivator.h>
#include <unify/Angle.h>
#include <unify/V3.h>
#include <unify/Quaternion.h>

using namespace me;
using namespace motivator;
using namespace input;

AutoOIMotivator::AutoOIMotivator( AutoOIMotivator & motivator )
	: ObjectInputMotivator( motivator )
{
}

AutoOIMotivator::AutoOIMotivator()
	: ObjectInputMotivator()
{
	GetLookup()->AddValue( "speed", "1" );
	GetLookup()->AddValue( "walkSpeed", "1" );
	GetLookup()->AddValue( "runSpeed", "1" );
	GetLookup()->AddValue( "lookXSpeed", "1" );
	GetLookup()->AddValue( "lookYSpeed", "1" );
}

AutoOIMotivator::~AutoOIMotivator() 
{
}

std::string AutoOIMotivator::GetTypeName() const
{
	return "AutoOIMotivator";
}

std::string AutoOIMotivator::GetWhat() const
{
	return std::string();
}								

void AutoOIMotivator::OnUpdate( UpdateParams params )
{
	if ( ! IsEnabled() )
	{
		return;
	}


	auto && runOnMotivation = GetMotivator()->GetCondition( "runOn" );

	// Digital movement...
	auto && strafeLeftMotivation = GetMotivator()->GetCondition( "strafeleft" );
	auto && strafeRightMotivation = GetMotivator()->GetCondition( "straferight" );
	auto && walkForwardMotivation = GetMotivator()->GetCondition( "walkforward" );
	auto && walkBackwardMotivation = GetMotivator()->GetCondition( "walkbackward" );	 

	// Analog movement...
	auto && walkXMotivation = GetMotivator()->GetCondition( "walkx" );
	auto && walkYMotivation = GetMotivator()->GetCondition( "walkY" );

	// Look...
	auto && lookXMotivation = GetMotivator()->GetCondition( "lookX" );
	auto && lookYMotivation = GetMotivator()->GetCondition( "lookY" );

	float speed = unify::Cast< float >( GetValue( "speed" ) );
	if ( runOnMotivation && runOnMotivation->IsTrue() )
	{
		speed = 3.0f;
	}

	float lookXSpeed = unify::Cast< float >( GetValue( "lookXSpeed" ) );
	float lookYSpeed = unify::Cast< float >( GetValue( "lookYSpeed" ) );


	if ( strafeLeftMotivation && strafeLeftMotivation->IsTrue() )
	{
		unify::Matrix translate( unify::MatrixTranslate( unify::V3< float >( -1, 0, 0 ) ) );
		GetObject()->GetFrame().PostMul( translate * params.renderInfo.GetDelta() * speed );
	}
	else if ( strafeRightMotivation && strafeRightMotivation->IsTrue() )
	{
		unify::Matrix translate( unify::MatrixTranslate( unify::V3< float >( 1, 0, 0 ) ) );
		GetObject()->GetFrame().PostMul( translate * params.renderInfo.GetDelta() * speed );
	}

	if ( walkForwardMotivation && walkForwardMotivation->IsTrue() )
	{
		unify::Matrix translate( unify::MatrixTranslate( unify::V3< float >( 0, 0, 1 ) ) );
		GetObject()->GetFrame().PostMul( translate * params.renderInfo.GetDelta() * speed );
	}
	else if ( walkBackwardMotivation && walkBackwardMotivation->IsTrue() )
	{
		unify::Matrix translate( unify::MatrixTranslate( unify::V3< float >( 0, 0, -1 ) ) );
		GetObject()->GetFrame().PostMul( translate * params.renderInfo.GetDelta() * speed );
	}

	if ( walkXMotivation && walkXMotivation->IsTrue() )
	{
		unify::V3< float > left = GetObject()->GetFrame().GetLeft();
		unify::Matrix matrix( unify::MatrixTranslate( left * walkXMotivation->GetValue() * params.renderInfo.GetDelta() * speed * 5.0f ) );
		GetObject()->GetFrame().PostMul( matrix );
	}

	if ( walkYMotivation && walkYMotivation->IsTrue() )
	{
		unify::V3< float > forward = GetObject()->GetFrame().GetForward();
		unify::Matrix matrix( unify::MatrixTranslate( forward * walkYMotivation->GetValue() * params.renderInfo.GetDelta() * speed * 5.0f ) );
		GetObject()->GetFrame().PostMul( matrix );
	}

	bool lookXMotivated = (lookXMotivation && lookXMotivation->IsTrue());
	bool lookYMotivated = (lookYMotivation && lookYMotivation->IsTrue());
	if ( lookXMotivated || lookYMotivated )
	{
		unify::Matrix matrix = unify::MatrixIdentity();

		if ( lookXMotivated )
		{
			float changeX = lookXMotivation->GetValue();
			matrix *= unify::MatrixRotationY( unify::AngleInRadians( params.renderInfo.GetDelta() * changeX * lookXSpeed ) );
		}

		if ( lookYMotivated )
		{
			float changeY = lookYMotivation->GetValue();
			matrix *= unify::MatrixRotationX( unify::AngleInRadians( params.renderInfo.GetDelta() * changeY * lookYSpeed ) );
		}

		GetObject()->GetFrame().PreMul( matrix );

	}

	// Fix up, so we don't tilt to the left or the rigth.
	{
		using namespace unify;
		V3< float > left = GetObject()->GetFrame().GetLeft();
		V3< float > forward = GetObject()->GetFrame().GetForward();
		V3< float > leftFixed = V3< float >( { left.x, 0, left.y } );
		float ln = leftFixed.Normalize();
		Angle angle( AngleInRadians( acos( left.Dot( leftFixed ) ) ) );
		V3< float > up = V3< float >::V3Cross( leftFixed, forward );
		float uv = up.Normalize();

		forward = V3< float >::V3Cross( up, leftFixed );

		GetObject()->GetFrame().SetLeft( leftFixed );
		GetObject()->GetFrame().SetUp( up );
		GetObject()->GetFrame().SetForward( forward );
	}
}

object::IObjectComponent::ptr AutoOIMotivator::Duplicate()
{
	auto duplicate = new AutoOIMotivator( *this );
	return me::object::IObjectComponent::ptr( duplicate );
}
