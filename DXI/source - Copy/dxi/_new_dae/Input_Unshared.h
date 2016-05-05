// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/dae/DAE_DocumentNode.h>
#include <dxi/FVF.h>
#include <qxml/QXML.h>
#include <string>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_geometries->geometry->geometry (1 or more)
		/// </summary>
		class Input_Unshared : public DocumentNode
		{
		public:
			Input_Unshared( IDocument & document, const qxml::Element * node );

			const std::string & GetSemantic() const;
			const std::string & GetSource() const;
			
		private:
			std::string m_semantic;
			std::string m_source;
		};
	}
}
