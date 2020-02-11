// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/object/component/ObjectComponent.h>

namespace me
{
	namespace phy
	{
		class Entity : public me::object::component::ObjectComponent
		{
		public:
			Entity( std::string type, bool update, bool render );
			Entity( Entity & entity );
			virtual ~Entity();

			/// <summary>
			///
			/// </summary>
			void AttachEntityOn( Entity * entity ){}

			/// <summary>
			///
			/// </summary>
			void UpdateEntities(){}

			/// <summary>
			///
			/// </summary>
			void MoveTo( unify::V3< float > position ){}
		};
	}
}