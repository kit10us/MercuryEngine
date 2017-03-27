// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <sculpter/Shape.h>

using namespace sculpter;

Shape::Shape()
{
}

Shape::~Shape()
{
}

void Shape::AddSheet( Sheet::ptr sheet )
{
	m_sheets.push_back( sheet );
}

size_t Shape::GetSheetCount() const
{
	return m_sheets.size();
}
