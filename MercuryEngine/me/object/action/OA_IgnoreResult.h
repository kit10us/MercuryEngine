// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/object/action/IObjectAction.h>
#include <list>

namespace me
{
	namespace object
	{
		namespace action
		{
			class IgnoreResult : public IObjectAction
			{
			public:
				IgnoreResult( IObjectAction::ptr action );

				bool Perform( Object* object, const UpdateParams & params) override;

			private:
				IObjectAction::ptr m_action;
			};
		}
	}
}