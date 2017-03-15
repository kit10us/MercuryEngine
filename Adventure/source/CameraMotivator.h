// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/object/Object.h>
#include <me/object/ObjectComponent.h>
#include <me/motivator/ObjectInputMotivator.h>

///<summary>
/// Motivates an object to orbit.
///</summary>
class CameraMotivator : public me::motivator::ObjectInputMotivator
{
public:
	CameraMotivator( const CameraMotivator & cameraMotivator );
	CameraMotivator();

	virtual ~CameraMotivator();

	std::string GetWhat() const override;

	void OnUpdate( me::UpdateParams params ) override;
	me::object::IObjectComponent * Duplicate() override;
};
