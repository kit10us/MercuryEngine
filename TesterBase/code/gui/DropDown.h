#pragma once

#include <Font.h>
#include <gui/Thing.h>

namespace gui
{
	class DropDown : public Thing
	{
	public:
		DropDown();
		virtual ~DropDown();

		virtual void Update( float delta, Input & input );
		virtual void Draw();

		void Create( V2 position, Size size, Font * font );
		void Destroy();
		size_t AddChoice( std::string text );
		const std::string & GetChoiceText( size_t index ) const;
		size_t NumberOfChoices() const;
		size_t GetCurrentChoice() const;
		void SetCurrentChoice( size_t choice );
		void SetOnChange( OnEventFunctor * onChangeFunctor );
		void SetHotKey( unsigned int hotKey );
		void ClearHotKey();

	private:
		std::vector< std::string > m_choices;
		size_t m_current;
		bool m_hotKeyIsSet;
		unsigned int m_hotKey;
		OnEventFunctor * m_onChangeFunctor;
		Font * m_font;
		InputLock m_dropDownLock;
	};
}
