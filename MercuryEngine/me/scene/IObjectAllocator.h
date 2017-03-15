// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/object/Object.h>
#include <me/IUnknown.h>

#ifdef GetObject
#undef GetObject
#endif

namespace me
{
	class GeometryCacheSummation;

	namespace scene
	{
		class IObjectAllocator : public IUnknown
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
			   
			virtual object::Object * NewObject( std::string name ) = 0;
			virtual bool DestroyObject( object::Object * object ) = 0;
			virtual object::Object * CopyObject( object::Object * object, std::string name ) = 0;
			virtual void CollectObjects( std::vector< object::Object * > & objects ) = 0;
			virtual object::Object * FindObject( std::string name ) = 0;
			virtual object::Object * GetObject( size_t index ) = 0;

			virtual void Update( UpdateParams params ) = 0;
			virtual void CollectCameras( object::CameraCache & camerasOut ) = 0;
			virtual void CollectRendering( RenderParams params, const object::FinalCamera & camera, GeometryCacheSummation & summation ) = 0;
		};
	}
}