// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/object/action/OA_SetComponentValue.h>

using namespace me;
using namespace object;
using namespace action;

SetComponentValue::SetComponentValue( std::string componentName, std::string valueName, std::string value )
	: m_componentName{ componentName }
	, m_componentAlias{ std::string() }
	, m_valueName{ valueName }
	, m_value{ value }
{
}

SetComponentValue::SetComponentValue( std::string componentName, std::string componentAlias, std::string valueName, std::string value )
	: m_componentName{ componentName }
	, m_componentAlias{ componentAlias }
	, m_valueName{ valueName }
	, m_value{ value }
{
}

bool SetComponentValue::Perform( Object* object, unify::TimeDelta delta )
{
	object->GetComponent( m_componentName, m_componentAlias )->GetLookup()->SetValue( m_valueName, m_value );
	return true;
}
