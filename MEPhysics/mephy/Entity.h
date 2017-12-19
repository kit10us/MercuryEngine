// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <MEPhysics.h>
#include <me/object/component/ObjectComponent.h>

#pragma once 

namespace mephy
{
	class Entity : public me::object::component::ObjectComponent
	{
	protected:
		Entity( Entity & entity );

	public:
		static char* Name();

		Entity();
		~Entity();

		void CaptureEarly();
		const unify::Matrix & GetEarly() const;

	public: // IObjectComponent...
		me::object::component::IObjectComponent::ptr Duplicate() override;

	private:
		unify::Matrix m_early;
	};
}
