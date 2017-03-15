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

			/// <summary>
			/// How many objects are current active.
			/// </summary>
			virtual size_t Count() const = 0;

			/// <summary>
			/// Can we allocate a new object?
			/// </summary>
			virtual bool Available() const = 0;
			   
			virtual Object * NewObject( std::string name ) = 0;
			virtual bool DestroyObject( Object * object ) = 0;
			virtual Object * CopyObject( Object * object, std::string name ) = 0;
			virtual void CollectObjects( std::vector< Object * > & objects ) = 0;
			virtual Object * FindObject( std::string name ) = 0;
			virtual Object * GetObject( size_t index ) = 0;

			virtual void Update( UpdateParams params ) = 0;
			virtual void CollectCameras( CameraCache & camerasOut ) = 0;
			virtual void CollectRendering( RenderParams params, const FinalCamera & camera, GeometryCacheSummation & summation ) = 0;
		};
	}
}