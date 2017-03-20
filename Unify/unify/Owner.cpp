// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/Owner.h>

using namespace unify;

Owner::Owner(std::string name)
	: m_name{ name }
{
}

Owner::ptr unify::Owner::Create(std::string name) 
{
	return ptr(new Owner{ name } ); 
}

std::string Owner::Name() const
{
	return m_name;
}
