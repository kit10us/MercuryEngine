// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <memory>
#include <unify/DataLock.h>
#include <unify/unify.h>
#include <unify/Flags.h>

namespace me
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
		static TYPE FromString( const std::string & usage );
		static std::string ToString( BufferUsage::TYPE usage );
	};

	class DataBuffer
	{
	public:
		DataBuffer();
		virtual ~DataBuffer();

		bool Locked() const;
		BufferUsage::TYPE GetUsage() const;
		unsigned int GetStride() const;
		unsigned int GetLength() const;
		unsigned int GetSize() const;

	protected:
		bool m_locked;
		BufferUsage::TYPE m_usage;
		unsigned int m_stride; // Size of each item in the buffer.
		unsigned int m_length; // Number of items we can store in the buffer.
	};
}