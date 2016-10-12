// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/physics/IInstance.h>
#include <dxi/scene/Object.h>
#include <qxml/Element.h>
#include <memory>

namespace dxi
{
	namespace physics
	{
		class IScene
		{
		public:
			IScene() {}
			virtual ~IScene() {}
			virtual void Add( std::shared_ptr< IInstance > object ) = 0;
            virtual void Add( scene::Object::ptr, const std::string & shape ) = 0;
			virtual void Update( const RenderInfo & renderInfo ) = 0;
			virtual void Render() = 0;
		private:
		};
	}
}