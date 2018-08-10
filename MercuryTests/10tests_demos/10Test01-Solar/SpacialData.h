#include <unify/Angle.h>
#include <unify/V3.h>
#include <unify/Matrix.h>
#include <memory>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

class SpacialData
{
public:
	SpacialData(float distance, unify::Angle orbitStart, unify::Angle orbitVelocity, unify::Angle rotationStart, unify::Angle rotationVelocity)
		: m_distance(distance)
		, m_orbit(orbitStart)
		, m_orbitVelocity(orbitVelocity)
		, m_rotation(rotationStart)
		, m_rotationVelocity(rotationVelocity)
	{
	}


	virtual ~SpacialData()
	{
	}

	virtual void Update(const UpdateParams & params, const SpacialData * parentData )
	{
		m_orbit += m_orbitVelocity * params.GetDelta();

		unify::V3< float > origin{ parentData ? parentData->m_position : unify::V3< float >::V3Zero() };
		m_position = unify::V3< float >::V3RotateAbout(origin, m_distance, m_rotation, unify::V3< float >::V3Z(1));

		m_rotation += m_rotationVelocity;
	}

	unify::Matrix GetMatrix() const
	{
		using namespace unify;
		auto matrix = unify::MatrixIdentity();
		matrix.RotateAboutAxis( unify::V3< float >( 0, 1, 0 ), m_orbit );
		matrix.SetPosition( m_position );
		return matrix;
	}

private:
	float m_distance;
	unify::Angle m_orbit;
	unify::Angle m_orbitVelocity;
	unify::Angle m_rotation;
	unify::Angle m_rotationVelocity;
	unify::Angle m_velocity;
	unify::V3< float > m_position;
};