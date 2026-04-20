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

#include <io/INode.h>
#include <qxml/Element.h>

namespace xmlio
{
	class Element : public io::INode
	{
	public:
		Element(const qxml::Element* element);

		virtual bool IsMatch(std::string filter) const;
		virtual std::string Text() const;
		virtual std::string Value() const;
		
		virtual INode::list Children(std::string filter) const;
		virtual INode::list Attributes(std::string filter) const;

	private:
		const qxml::Element* m_element;
	};
}