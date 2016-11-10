#pragma once
#include <list>
#include <me/mesh/MeshAnimationKey.h>

namespace me
{
	namespace ma // mesh animation
	{
		class FrameController
		{
		public:
			unify::Frame * m_frame;
			unify::Frame::mapToIndex m_frameMap; // This is used for quick finding.
			unify::Frame::vector m_frameLookup; // This is used primarily for animations.
		};
	}
}