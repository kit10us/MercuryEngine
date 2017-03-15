// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/object/ObjectComponent.h>

class FollowComponent : public me::object::ObjectComponent
{
public:
	FollowComponent();
	~FollowComponent();

	void SetTarget( me::object::Object * target );
	void SetOffset( unify::Matrix offset );

public: // IObjectComponent...
	bool Updateable() const override {
		return true;
	}

	void OnUpdate( me::UpdateParams params ) override;


	me::object::IObjectComponent * Duplicate() override;

public: // IComponent
	std::string GetWhat() const override;

private:
	me::object::Object * m_target;
	unify::Matrix m_offset;
};
