#pragma once

#include <unify/V3.h>
#include <unify/Quaternion.h>
#include <list>

namespace me
{
	namespace frameanimation
	{
		// A specific event/status key.
		template< typename T >
		class Key
		{
		public:
			typedef std::list< Key< T > > list;

			Key( float time, const T & value );
			bool operator<( const Key< T > & otherKey );
			bool operator<( float time ) const;
			float Time() const;
			const T & Value() const;

		private:
			float m_time;
			T m_value;
		};

		template< typename T >
		void FindLeftRight( float time, const typename Key< T >::list & timeline, typename Key< T >::list::const_iterator & leftItr, typename Key< T >::list::const_iterator & rightItr );

		template< typename T >
		T InterpretValue( float time, const typename Key< T >::list & timeline, T defaultValue );

		template<>
		unify::Quaternion InterpretValue< unify::Quaternion >( float time, const typename Key< unify::Quaternion >::list & timeline, unify::Quaternion defaultValue );

		typedef Key< unify::V3< float > > ScaleKey;
		typedef Key< unify::Quaternion > RotationKey;
		typedef Key< unify::V3< float > > TranslationKey;
		struct SRTTimeline 
		{
			ScaleKey::list scale;
			RotationKey::list rotation;
			TranslationKey::list translation;
		};

		#include <me/frameanimation/FrameAnimationKey.inl>
	}
}