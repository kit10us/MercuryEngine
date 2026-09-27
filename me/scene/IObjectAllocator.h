// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <me/IThing.h>
#include <me/object/FinalCamera.h>
#include <me/UpdateParams.h>
#include <me/render/RenderParams.h>

namespace me
{
	namespace render
	{
		class GeometryCacheSummation;
	}

	namespace scene
	{
		class RenderGirl;

		/// <summary>
		/// And object allocator allows for specializing the method of which scene objects are allocated,
		/// for example, from a pre-allocated buffer of objects.
		/// </summary>
		class IObjectAllocator : public me::IThing
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

			/// <summary>
			/// Allocate a new object.
			/// </summary>
			/// <param name="name">object name</param>
			/// <returns></returns>
			virtual object::Object * NewObject( std::string name ) = 0;

			/// <summary>
			/// Destroy an object.
			/// </summary>
			/// <param name="object"></param>
			/// <returns></returns>
			virtual bool DestroyObject( object::Object * object ) = 0;

			/// <summary>
			/// Copy an existing object producing a new object of the same type.
			/// </summary>
			/// <param name="object"></param>
			/// <param name="name"></param>
			/// <returns></returns>
			virtual object::Object * CopyObject( object::Object * object, std::string name ) = 0;

			/// <summary>
			/// Collect all objects into a container.
			/// </summary>
			/// <param name="objects"></param>
			virtual void CollectObjects( std::vector< object::Object * > & objects ) = 0;

			/// <summary>
			/// Finds objects by name.
			/// </summary>
			/// <param name="name"></param>
			/// <returns></returns>
			virtual object::Object * FindObject( std::string name ) = 0;


			virtual void DirtyObject( object::Object* object ) = 0;

			virtual void Update( const UpdateParams & params ) = 0;
			virtual void CollectCameras( RenderGirl & renderGirl ) = 0;
			virtual void CollectRendering( render::Params params, const object::FinalCamera & camera, render::GeometryCacheSummation & solids, render::GeometryCacheSummation & trans ) = 0;
		};
	}
}