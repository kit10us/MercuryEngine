// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/motivator/ObjectInputMotivator.h>

namespace me
{
	namespace motivator
	{
		///<summary>
		/// Motivates an object to orbit.
		///</summary>
		class AutoOIMotivator : public ObjectInputMotivator
		{
		public:
			AutoOIMotivator( IOS * os );

			virtual ~AutoOIMotivator();

			std::string GetType() const override;
			std::string GetWhat() const override;

			void OnUpdate( UpdateParams params ) override;


			scene::IObjectComponent * Duplicate() override;
		};
	}
}