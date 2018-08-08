// Copyright (c) 2002 - 2018, Evil Quail LLC
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

ConstantVariable::ConstantVariable( std::string name, render::ElementFormat::TYPE type, size_t count )
	: name( name )
	, type( type )
	, count( count )
{
}
