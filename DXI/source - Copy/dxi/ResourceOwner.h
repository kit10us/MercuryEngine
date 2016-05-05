#pragma once

namespace dxi
{
	/// Every resource has a ResourceOwner. This allows a grouping of resoures, typically to a 
	/// device. A resources states directly affected first by a resource owner. If, for example,
	/// a resource owner is "lost", or removed, it necessitates that all of its resources are 
	/// also lost. ResourceOwners can relate to other resource owners. For example, a graphics
	/// device, as a ResourceOwner, can be exclusive with another graphics device, that is to 
	/// say, their resources can't mix together (i.e., a geometry shouldn't use a texture that 
	/// belongs to a different ResourceOwner).
	/// Resources are created by referencing their owner (a DirectX Device creates a texture).
	class ResourceOwner
	{
	public:
	};
}