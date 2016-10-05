#pragma once

#include <gui/Events.h>
#include <Input.h>

namespace gui
{
	class Thing
	{
	public:
		virtual ~Thing() {}
		virtual void Update( float delta, Input & input ) = 0;
		virtual void Draw() = 0;

		virtual void GetRect( Rect & rect ) const;
		virtual const V2 & GetPosition() const;
		virtual const Size & GetSize() const;
		virtual void Set( const V2 & position, const Size & size );
		virtual void MoveTo( const V2 & position );
		virtual void MoveBy( const V2 & position );
		virtual void AdjustSize( const Size & size );
		virtual void SetUserData( float userData );
		virtual float GetUserData() const;

	private:
		V2 m_position;
		Size m_size;
		float m_userData;
	};

	void PositionFrom( Thing & thing, const Thing & from, Origin::TYPE origin, V2 offset = V2( 0, 0 ) );
}

