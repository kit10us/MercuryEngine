// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <qxml/QXML.h>
#include <dae/library_effects/Shading.h>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_effects->effect->profile_COMMON.technique (1)
		/// </summary>
		class Profile_Technique
		{
		public:
			Profile_Technique( const qxml::Element * node );

			const std::string & GetID() const;
			const std::string & GetSID() const;
			const Shading & GetShading() const;

		private:
			std::string m_id;
			std::string m_sid;
			//Technique_Shading _shading;
			std::shared_ptr< Shading > m_shading;
		};
	}
}