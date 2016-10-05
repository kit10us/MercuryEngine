#pragma once

#include <Font.h>
#include <gui/Thing.h>

namespace gui
{
	class Text : public Thing
	{
	public:
		Text();
		virtual ~Text();

		virtual void Update( float delta, Input & input );
		virtual void Draw();

		void Create( V2 position, std::string text, Font * font );
		void Create( V2 position, Size size, std::string text, Font * font );
		void Destroy();
		void SetText( std::string text );
		const std::string & GetText() const;
		void SetColor( D3DCOLOR color );
		D3DCOLOR GetColor() const;
		void SetAlignment( Origin::TYPE alignment );
		Origin::TYPE GetAlignment() const;

	private:
		std::string m_text;
		Font * m_font;
		D3DCOLOR m_color;
		Origin::TYPE m_alignment;
	};
}
