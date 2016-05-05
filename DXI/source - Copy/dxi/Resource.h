// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <list>

namespace dxi
{
	class Resource;
	class ResourceOwner;

	// A Resource class pushing standardization of resource management...
	class Resource
	{
	public:
		Resource();
		virtual ~Resource();

		// Load the resource's information using a description we already have.
		virtual void Preload() {};

		// Create/load our resource.
		virtual void Create() = 0;
 
		virtual void Destroy() = 0;

		// Only when we are acquired, we then should be able to call this to ensure we are updated to said device.
		virtual void Validate() = 0;

		// Only when we are acquired, we then should be able to call this to ensure we are NOT updated to said device.
		virtual void Invalidate() = 0;

		virtual bool Created() const;
		virtual bool Valid() const;

		void AddOwner( const ResourceOwner * owner );
		void RemoveOwner( const ResourceOwner * owner );
	
	protected:
		bool m_created;
		bool m_valid;
		std::list< const ResourceOwner * > m_owners;
	};
} // namespace dxi