// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <qxml/XPath.h>

using namespace qxml;

XPath::XPath()
{
}

XPath::XPath( std::string path )
	: m_path( path )
{
}

XPath::~XPath()
{
}

std::string XPath::GetPath() const
{
	return m_path;
}

bool XPath::Test( const qxml::Element & element ) const
{
	return false;
}
