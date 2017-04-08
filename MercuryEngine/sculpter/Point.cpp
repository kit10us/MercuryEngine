// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <sculpter/Point.h>

using namespace sculpter;

Point::Point()
	: m_matrix{ unify::MatrixIdentity() }
{
}

Point::Point( unify::Matrix matrix )
	: m_matrix{ matrix }
{
}

Point::~Point()
{
}

unify::Matrix Point::GetMatrix() const
{
	return m_matrix;
}

const unify::Matrix & Point::operator()() const
{
	return m_matrix;
}
