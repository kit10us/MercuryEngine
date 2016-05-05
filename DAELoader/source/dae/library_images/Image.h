// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dae/DocumentNode.h>
#include <qxml/QXML.h>
#include <string>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_images->image (1 or more)
		/// </summary>
		class Image : public DocumentNode
		{
		public:
			Image( IDocument & document, const qxml::Element * node );

			const std::string & GetID() const;
			const std::string & GetSID() const;
			const std::string & GetName() const;
			Texture::shared_ptr GetTexture() const;

		private:
			std::string m_id;
			std::string m_sid;
			std::string m_name;
			Texture::shared_ptr m_texture;
		};
	}
}