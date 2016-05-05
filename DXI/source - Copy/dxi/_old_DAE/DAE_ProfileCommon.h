// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/dae/DAE_DocumentNode.h>
#include <dxi/dae/DAE_Profile_Technique.h>
#include <dxi/dae/DAE_NewParam.h>
#include <qxml/QXML.h>
#include <vector>
#include <map>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_effects->effect->profile_COMMON (0 or more)
		/// </summary>
		class ProfileCOMMON : public DocumentNode
		{
		public:
			ProfileCOMMON( IDocument & document, const qxml::Element * node );

			const std::string & GetID() const;
			const Profile_Technique & GetTechnique() const;
			const std::vector< boost::shared_ptr< NewParam > > & GetNewParams() const;
			const NewParam * FindNewParam( const std::string & sid ) const;

		private:
			std::string m_id;
			std::vector< boost::shared_ptr< NewParam > > m_newParams;
			std::map< std::string, boost::shared_ptr< NewParam > > m_newParamsMap;
			boost::shared_ptr< Profile_Technique > m_technique;
		};
	}
}
