// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/dae/DAE_ProfileCommon.h>
#include <dxi/dae/DAE_DocumentNode.h>
#include <qxml/QXML.h>
#include <string>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_effects->effect (1 or more)
		/// </summary>
		class Effect : public DocumentNode
		{
		public:
			Effect( IDocument & document, const qxml::Element * node );

			const std::string & GetID() const;
			const std::string & GetName() const;
			const ProfileCOMMON * GetProfileCOMMON() const;

		private:
			std::string m_id;
			std::string m_name;

			boost::shared_ptr< ProfileCOMMON > m_profileCOMMON;
		};
	}
}