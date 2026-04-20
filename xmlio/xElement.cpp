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

#include <xmlio/xElement.h>
#include <xmlio/Attribute.h>
#include <qxml/Attribute.h>

using namespace xmlio;

Element::Element(const qxml::Element* element)
	: m_element{ element }
{
}

bool Element::IsMatch(std::string filter) const
{
	std::string name = m_element->GetName();
	return unify::String::StringIs(name, filter);
	//return m_element->IsTagName(filter);
}

std::string Element::Text() const
{
	return m_element->GetText();
}

std::string Element::Value() const
{
	return m_element->GetText();
}

io::INode::list Element::Children(std::string filter) const
{
	io::INode::list children{};

	for (auto element : m_element->Children())
	{
		if (filter.empty() || element.IsTagName(filter))
		{
			children.push_back(std::make_shared<Element>(&element));
		}
	}

	return children;
}

io::INode::list Element::Attributes(std::string filter) const
{
	io::INode::list attributes{};

	for (unsigned int i = 0; i < m_element->NumAttributes(); i++)
	{
		auto attribute = m_element->GetAttribute(i);
		if (filter.empty() || strcmp(attribute->GetName().c_str(), filter.c_str()) == 0)
		{
			attributes.push_back(std::make_shared<Attribute>(attribute.get()));
		}
	}

	return attributes;
}