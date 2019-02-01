// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/ConstantVariable.h>

using namespace me;
using namespace render;

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
