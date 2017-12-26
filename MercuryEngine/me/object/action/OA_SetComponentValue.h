// Copyright (c) 2002 - 2018, Quentin S. Smith
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
			class SetComponentValue : public IObjectAction
			{
			public:
				SetComponentValue( std::string componentName, std::string valueName, std::string value );
				SetComponentValue( std::string componentName, std::string componentAlias, std::string valueName, std::string value );

				bool Perform( Object* object, float delta ) override;

			private:
				std::string m_componentName;
				std::string m_componentAlias;
				std::string m_valueName;
				std::string m_value;
			};
		}
	}
}