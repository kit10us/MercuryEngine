// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <canvas/ListItem.h>
#include <me/render/Effect.h>

using namespace me;
using namespace canvas;

ListItem::ListItem( IElement::ptr item, std::string tag )
	: m_item{ item } 
	, m_tag{ tag }
{
}

IElement::ptr ListItem::GetItem()
{
	return m_item;
}

IElement::const_ptr ListItem::GetItem() const
{
	return m_item;
}

std::string ListItem::GetTag() const
{
	return m_tag;
}
