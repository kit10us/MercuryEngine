// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/object/ObjectComponent.h>
#include <me/object/Object.h>
#include <me/motivator/InputMotivator.h>

namespace me
{
	namespace motivator
	{
		///<summary>
		/// Motivates an object to orbit.
		///</summary>
		class ObjectInputMotivator : public object::ObjectComponent
		{
		public:
			ObjectInputMotivator( const ObjectInputMotivator & objectInputMotivator );
			ObjectInputMotivator();

			virtual ~ObjectInputMotivator();
															
			motivator::IInputMotivator * GetMotivator();

		private:
			motivator::InputMotivator m_motivator;
		};
	}
}