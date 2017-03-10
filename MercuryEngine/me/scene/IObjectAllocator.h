// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/scene/Object.h>

#ifdef GetObject
#undef GetObject
#endif

namespace me
{
	class GeometryCacheSummation;

	namespace scene
	{
		class IObjectAllocator
		{
		public:
			typedef std::shared_ptr< IObjectAllocator > ptr;

			~IObjectAllocator() {}

			virtual bool IsResizable() const = 0;
			virtual size_t Count() const = 0;
			virtual bool Available() const = 0;
			   
			virtual Object * NewObject( std::string name ) = 0;
			virtual bool DestroyObject( Object * object ) = 0;
			virtual Object * CopyObject( Object * object, std::string name ) = 0;
			virtual void CollectObjects( std::vector< Object * > & objects ) = 0;
			virtual Object * FindObject( std::string name ) = 0;
			virtual Object * GetObject( size_t index ) = 0;

			virtual void Update( UpdateParams params ) = 0;
			virtual void CollectRendering( RenderParams params, CameraCache & cameraCache, GeometryCacheSummation & summation ) = 0;
		};
	}
}