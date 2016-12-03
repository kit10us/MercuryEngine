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
			virtual void DestroyObject( Object * object ) = 0;
			virtual Object * CopyObject( Object * object, std::string name ) = 0;
		};
	}
}