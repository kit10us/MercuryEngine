// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <string>

namespace me
{
	namespace render
	{
		struct BufferUsage
		{
			enum TYPE
			{
				/// <summary>
				/// A resource that requires read and write access by the GPU. This is likely to be the most common usage choice.
				/// </summary>
				Default,

				/// <summary>
				/// A resource that can only be read by the GPU. It cannot be written by the GPU, and cannot be accessed at all by the CPU. This type of resource must be initialized when it is created, since it cannot be changed after creation.
				/// </summary>
				Immutable,

				/// <summary>
				/// A resource that is accessible by both the GPU (read only) and the CPU (write only). A dynamic resource is a good choice for a resource that will be updated by the CPU at least once per frame. To update a dynamic resource, use a Map method.
				/// </summary>
				Dynamic,

				/// <summary>
				/// A resource that supports data transfer (copy) from the GPU to the CPU.
				/// </summary>
				Staging
			};
			static TYPE FromString( std::string usage );
			static std::string ToString( BufferUsage::TYPE usage );
		};

		struct CountAndSource
		{
			CountAndSource( size_t count, const void * source = nullptr )
				: count{ count }
				, source{ source }
			{
			}

			size_t count;
			const void * source;
		};
	}
}