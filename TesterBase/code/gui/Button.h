#pragma once

#include <Font.h>
#include <gui/Thing.h>
#include <gui/Text.h>

namespace gui
{
	class Button : public Text
	{
	public:
		Button();
		virtual ~Button();

		virtual void Update( float delta, Input & input );
		virtual void Draw();

		void Create( V2 position, Size size, std::string text, Font * font );
		void Destroy();
		void SetOnPress( OnEventFunctor * onPressFunctor );
		void SetHotKey( unsigned int hotKey );
		void ClearHotKey();

	private:
		bool m_hotKeyIsSet;
		unsigned int m_hotKey;
		OnEventFunctor * m_onPressFunctor;
	};
}
