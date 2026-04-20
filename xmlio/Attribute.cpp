/*
 * QXML Interop Library
 * https://github.com/kit10us/qxmlinterop
 * Copyright (c) 2002, Kit10 Studios LLC
 *
 * This file is part of QXML Interop Library (a.k.a. QXMLIO)
 *
 * QXMLIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QXMLIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QXMLIO.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <xmlio/Attribute.h>

using namespace xmlio;

Attribute::Attribute(const qxml::Attribute* attribute)
	: m_attribute{ attribute }
{
}

bool Attribute::IsMatch(std::string filter) const
{
	// We consider the attribute a match if the filter is empty, or if it is the name of the attribute.
	if (filter.empty() || strcmp(m_attribute->GetName().c_str(), filter.c_str()) == 0)
	{
		return true;
	}

	return false;
}

std::string Attribute::Text() const
{
	return m_attribute->GetString();
}

std::string Attribute::Value() const
{
	return m_attribute->Get<std::string>();
}

io::INode::list Attribute::Children(std::string filter) const
{
	return {};
}

io::INode::list Attribute::Attributes(std::string filter) const
{
	return {};
}