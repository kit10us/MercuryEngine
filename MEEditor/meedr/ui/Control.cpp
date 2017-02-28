// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Control.h>

using namespace meedr;
using namespace ui;

Control::Control( int id )
	: m_parent{ nullptr }
	, m_id{ id }
	, m_fillWidthWeight{ -1 }
	, m_fillHeightWeight{ -1 }
{
}

Control::~Control()
{
}

void Control::SetParent( container::Container * parent )
{
	m_parent = parent;
}

container::Container * Control::GetParent()
{
	return m_parent;
}

int Control::GetID()
{
	return m_id;
}

int Control::GetDefaultWidth()
{
	return -1;
}

int Control::GetDefaultHeight()
{
	return -1;
}

void Control::SetFillWidthWeight( int weight )
{
	m_fillWidthWeight = weight;
}

int Control::GetFillWidthWeight()
{
	return m_fillWidthWeight;
}

void Control::SetFillHeightWeight( int weight )
{
	m_fillHeightWeight = weight;
}

int Control::GetFillHeightWeight()
{
	return m_fillHeightWeight;
}
