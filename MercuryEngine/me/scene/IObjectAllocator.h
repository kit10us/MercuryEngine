// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/scene/Object.h>

namespace me
{
	namespace scene
	{
		class IObjectAllocator
		{
		public:
			~IObjectAllocator() {}

			virtual bool IsResizable() const = 0;
			virtual size_t Count() const = 0;
			virtual bool Available() const = 0;
			   
			virtual Object * NewObject( std::string name ) = 0;
			virtual bool DestroyObject( Object * object ) = 0;
			virtual Object * CopyObject( Object * object, std::string name ) = 0;
			virtual void CollectObjects( std::vector< Object * > & objects ) = 0;
			virtual Object * FindObject( std::string name ) = 0;

			virtual void Update( IRenderer * renderer, const RenderInfo & renderInfo, CameraCache & cameraCache ) = 0;
			virtual void Render( IRenderer * renderer, const RenderInfo & renderInfo, const CameraCache & cameras ) = 0;
		};
	}
}