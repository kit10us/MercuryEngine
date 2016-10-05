#pragma once

#include <gui/Thing.h>
#include <Texture.h>

namespace gui
{
	class Picture : public Thing
	{
	public:
		Picture();
		virtual ~Picture();

		virtual void Update( float delta, Input & input );
		virtual void Draw();

		void Create( V2 position, Texture * texture );
		void Destroy();

	private:
		Texture * m_texture;
	};
}
