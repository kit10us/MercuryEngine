#include <iostream>
#include <conio.h>
#include <string>
#include <assert.h>

#pragma pack( push, 1 )
// Following verbose standard (error on the side of more detailed names).
class ShapeDescription
{
	unsigned char m_flags;
	unsigned char m_red;
	unsigned char m_green;
	unsigned char m_blue;
	// 4 bytes

	union {
		struct { // Sphere
			float radius;
		} sphere;
		struct { // Box
			float sizeX;
			float sizeY;
			float sizeZ;
		} box;
		struct { // Cylinder
			float radius, sizeZ;
		} cylinder;
	} m_details; 
	// 12 bytes 

	unsigned short m_priority;
	// 2 byts

	enum { // Constants
		VisibleBit = 0x01,
		WireframeBit = 0x02,
		TypeBitshift = 2,
	};
public:
	// I prefer this case for constants and enums, which help dangerous MACROS stand out.
	enum ShapeType {
		UNKNOWN, // Not a requirement, but the extra bit is available, so might as well add this (would verify with req. provider). 
		ShapeTypeSphere, 
		ShapeTypeBox, 
		ShapeTypeCylinder
	};

	// For the sake of brevity, all functions are inline, however, I typically avoid this, even going so 
	// far as to create a ".inl" file used for template function and inline defintions (.inl is then 
	// included at the bottom of the header).
	ShapeDescription()
		: m_flags( 0 )
		, m_red( 0 )
		, m_green( 0 )
		, m_blue( 0 )
	{
	}
	

	// Common attributes:

	void SetType( ShapeType type )
	{
		unsigned char typeAsNumber = static_cast< unsigned char >( type );
		unsigned char typeValue = typeAsNumber << TypeBitshift;
		m_flags |= typeValue;
	}

	ShapeType GetType() const
	{
		unsigned char typeValue = m_flags >> TypeBitshift;
		
		return static_cast< ShapeType >( typeValue );
	}

	void SetColor( unsigned char red, unsigned char green, unsigned char blue )
	{
		m_red = red;
		m_green = green;
		m_blue = blue;
	}

	unsigned char GetRed() const
	{
		return m_red;
	}

	unsigned char GetGreen() const
	{
		return m_green;
	}

	unsigned char GetBlue() const
	{
		return m_blue;
	}

	void SetVisible( bool visible )
	{
		m_flags = visible ? ( m_flags | VisibleBit ) : ( m_flags & ~VisibleBit );
	}

	bool GetVisible() const
	{
		return ( m_flags & VisibleBit ) ? true : false;
	}

	void SetWireframe( bool wireframe )
	{
		m_flags = wireframe ? ( m_flags | WireframeBit ) : ( m_flags & ~WireframeBit );
	}

	bool GetWireframe() const
	{
		return ( m_flags & WireframeBit ) ? true : false;
	}

	void SetPriority( unsigned short priority )
	{
		assert( priority <= 4000 );
		m_priority = priority;
	}

	unsigned short GetPriority() const
	{
		return m_priority;
	}


	// For a sphere:

	void BuildSphere( float radius )
	{
		SetType( ShapeTypeSphere );
		SetSphereRadius( radius );
	}

	void SetSphereRadius( float radius )
	{
		assert( GetType() == ShapeTypeSphere );
		m_details.sphere.radius = radius;
	}

	float GetSphereRadius() const
	{
		assert( GetType() == ShapeTypeSphere );
		return m_details.sphere.radius;
	}


	// For a box:

	void BuildBox( float sizeX, float sizeY, float sizeZ )
	{
		SetType( ShapeTypeBox );
		SetBoxSizes( sizeX, sizeY, sizeZ );
	}

	void SetBoxSizes( float sizeX, float sizeY, float sizeZ )
	{
		assert( GetType() == ShapeTypeBox );
		m_details.box.sizeX = sizeX;
		m_details.box.sizeY = sizeY;
		m_details.box.sizeZ = sizeZ;
	}

	float GetBoxSizeX() const
	{
		assert( GetType() == ShapeTypeBox );
		return m_details.box.sizeX;
	}

	float GetBoxSizeY() const
	{
		assert( GetType() == ShapeTypeBox );
		return m_details.box.sizeY;
	}

	float GetBoxSizeZ() const
	{
		assert( GetType() == ShapeTypeBox );
		return m_details.box.sizeZ;
	}


	// For a cylinder:

	void BuildCylinder( float radius, float sizeZ )
	{
		SetType( ShapeTypeCylinder );
		m_details.cylinder.radius = radius;
		m_details.cylinder.sizeZ = sizeZ;
	}

	float GetCylinderRadius() const
	{
		assert( GetType() == ShapeTypeCylinder );
		return m_details.cylinder.radius;
	}

	float GetCylinderSizeZ() const
	{
		assert( GetType() == ShapeTypeCylinder );
		return m_details.cylinder.sizeZ;
	}


	void Print() const
	{
		using std::cout;
		cout << "{ ";

		// Specific attributes:
		switch( GetType() )
		{
		case ShapeTypeSphere:
			cout << "Sphere, radis = " << GetSphereRadius();
			break;
		case ShapeTypeBox:
			cout << "Box, sizeX = " << GetBoxSizeX() << ", sizeY = " << GetBoxSizeY() << ", sizeZ = " << GetBoxSizeZ();
			break;
		case ShapeTypeCylinder:
			cout << "Cylinder, raidus = " << GetCylinderRadius() << ", sizeZ = " << GetCylinderSizeZ();
			break;
		default:
			cout << "UNKNOWN }";
		}

		// Common attributes:
		cout << ", RGB( " << static_cast< int >( GetRed() ) << ", " << static_cast< int >( GetGreen() ) << ", " << static_cast< int >( GetBlue() ) << ")";
		cout << ", visible = " << ( GetVisible() ? "true" : "false" );
		cout << ", wireframe = " << ( GetWireframe() ? "true" : "false" );
		cout << ", priority = " << GetPriority();
		cout << " }" << std::endl;
	}
};

#pragma pack( pop )

void Q2()
{
	size_t sizeOfShapeDesription = sizeof( ShapeDescription );

	ShapeDescription sphere;
	sphere.SetColor( 255, 128, 64 );
	sphere.SetVisible( false );
	sphere.SetWireframe( true );
	sphere.SetPriority( 0 );
	sphere.BuildSphere( 300.003f );
	sphere.Print();

	ShapeDescription box;
	box.SetColor( 64, 128, 255 );
	box.SetVisible( true );
	box.SetWireframe( false );
	box.BuildBox( 10.0f, 200.0f, 400.0f );
	box.SetPriority( 2000 );
	box.Print();

	ShapeDescription cylinder;
	cylinder.SetColor( 1, 2, 3 );
	cylinder.SetVisible( false );
	cylinder.SetWireframe( false );
	cylinder.BuildCylinder( 1234.567f, 7654.321f );
	cylinder.SetPriority( 4000 );
	cylinder.Print();
}
