// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/object/action/IObjectAction.h>
#include <me/dyna/IDynaPosition.h>
#include <list>

namespace me
{
	namespace object
	{
		namespace action
		{
			class SetPosition : public IObjectAction
			{
			public:
				SetPosition( dyna::IDynaPosition::ptr position );

				bool Perform( Object* object, unify::TimeDelta delta ) override;

			private:
				dyna::IDynaPosition::ptr m_position;
			};
		}
	}
}