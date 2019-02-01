// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/object/component/ObjectComponent.h>
#include <me/object/action/IObjectAction.h>

namespace me
{
	namespace object
	{
		namespace component
		{
			/// <summary>
			/// 
			/// </summary>
			class ObjectActionComponent : public ObjectComponent
			{
			protected:
				ObjectActionComponent( ObjectActionComponent & component );

			public:
				ObjectActionComponent( action::IObjectAction::ptr action );
				virtual ~ObjectActionComponent();

				void OnUpdate( const UpdateParams & params ) override;

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
}