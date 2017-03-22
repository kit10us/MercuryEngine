// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/object/ObjectComponent.h>
#include <me/object/action/IObjectAction.h>

namespace me
{
	namespace object
	{
		class ObjectActionComponent : public ObjectComponent
		{
		protected:
			ObjectActionComponent(ObjectActionComponent & component );

		public:
			ObjectActionComponent(action::IObjectAction::ptr action);
			virtual ~ObjectActionComponent();

			void OnUpdate(UpdateParams params) override;

		public: // IObjectComponent...
			IObjectComponent::ptr Duplicate() override;

		public: // me::IThing...
			std::string GetWhat() const override;

		private:
			action::IObjectAction::ptr m_action;
			bool m_stopped;
		};
	}
}