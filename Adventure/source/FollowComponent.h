// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/ObjectComponent.h>

class FollowComponent : public me::scene::ObjectComponent
{
public:
	FollowComponent();
	~FollowComponent();

	void SetTarget( me::scene::Object * target );
	void SetOffset( unify::Matrix offset );

public: // IObjectComponent...
	bool Updateable() const override {
		return true;
	}

	void OnUpdate( me::UpdateParams params ) override;


	me::scene::IObjectComponent * Duplicate() override;

public: // IComponent
	std::string GetWhat() const override;

private:
	me::scene::Object * m_target;
	unify::Matrix m_offset;
};
