// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/shader/ConstantVariable.h>
#include <unify/String.h>
#include <unify/Matrix.h>

using namespace me;
using namespace shader;

ConstantVariable::ConstantVariable()
	: hasDefault{ false }
{
}

ConstantVariable::ConstantVariable( std::string name, ElementFormat::TYPE type, size_t count )
	: name( name )
	, type( type )
	, count( count )
{
}
