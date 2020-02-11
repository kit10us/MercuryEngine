// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/V3.h>
#include <unify/Quaternion.h>
#include <unify/TimeDelta.h>
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

			Key( unify::TimeDelta time, const T & value );
			bool operator<( const Key< T > & otherKey );
			bool operator<( unify::TimeDelta time ) const;
			unify::TimeDelta Time() const;
			const T & Value() const;

		private:
			unify::TimeDelta m_time;
			T m_value;
		};

		template< typename T >
		void FindLeftRight( unify::TimeDelta time, const typename Key< T >::list & timeline, typename Key< T >::list::const_iterator & leftItr, typename Key< T >::list::const_iterator & rightItr );

		template< typename T >
		T InterpretValue( unify::TimeDelta time, const typename Key< T >::list & timeline, T defaultValue );

		template<>
		unify::Quaternion InterpretValue< unify::Quaternion >( unify::TimeDelta time, const typename Key< unify::Quaternion >::list & timeline, unify::Quaternion defaultValue );

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