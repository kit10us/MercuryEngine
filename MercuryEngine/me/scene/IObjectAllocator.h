// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

namespace me
{
	namespace scene
	{
		class Object;

		class IObjectAllocator
		{
		public:
			~IObjectAllocator() {}

			virtual Object * NewObject( std::string name ) = 0;
			virtual bool DestroyObject( Object * object ) = 0;
			virtual Object * CopyObject( Object * object, std::string name ) = 0;
			virtual void CollectObjects( std::vector< Object * > & objects ) = 0;
			virtual Object * FindObject( std::string name ) = 0;

			virtual void Update( IRenderer * renderer, const RenderInfo & renderInfo ) = 0;
			virtual void Render( IRenderer * renderer, const RenderInfo & renderInfo ) = 0;
		};
	}
}