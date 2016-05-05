#include <unify/BBox.h>
#include <iostream>
#include <conio.h>

int main( int argc, char ** argv )
{
	using namespace unify;
	using std::cout;
	using std::endl;

	cout << "Ray collision with BBox:" << endl;
	BBox<float > inFrontBBox( V3<float>( -10, -10, -10 ), V3<float>( 10, 10, 10 ) );
	inFrontBBox += V3<float>( 0, 0, 40 ); // Move BBox infront of us.

	Ray< float > straightForwardRay( V3< float >( 0, 0, 0 ), V3< float >( 0, 0, 1 ) );
	Ray< float > straightBackwardRay( V3< float >( 0, 0, 0 ), V3< float >( 0, 0, -1 ) );
	Ray< float > straightForwardRayTooFarLeft( V3< float >( 0, 0, 0 ), V3< float >( 0, 0, 1 ) );
	straightForwardRayTooFarLeft.origin -= V3< float >( 11, 0, 0 );

	cout << "  1. straightRay should hit inFrontBBox: " << ( inFrontBBox.RayTest( straightForwardRay, 0, 1000 ) == true ? "success" : "fail" ) << endl;
	cout << "  2. inverse straightRay should miss inFrontBBox: " << (inFrontBBox.RayTest( straightBackwardRay, 0, 1000 ) == false ? "success" : "fail") << endl;
	cout << "  3. straightRay too far left should miss inFrontBBox: " << (inFrontBBox.RayTest( straightForwardRayTooFarLeft, 0, 1000 ) == false ? "success" : "fail") << endl;

	cout << std::endl << std::endl;
	while( ! _getch() );

    return 0;
}