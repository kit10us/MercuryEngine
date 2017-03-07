// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/Object.h>
#include <me/scene/ObjectComponent.h>
#include <me/motivator/ObjectInputMotivator.h>

///<summary>
/// Motivates an object to orbit.
///</summary>
class CameraMotivator : public me::motivator::ObjectInputMotivator
{
public:
	CameraMotivator( me::IOS * os );

	virtual ~CameraMotivator();

	std::string GetType() const override;
	std::string GetWhat() const override;

	void OnUpdate( me::UpdateParams params ) override;
	me::scene::IObjectComponent * Duplicate() override;
	
private:
	me::scene::Object * m_target;
};
