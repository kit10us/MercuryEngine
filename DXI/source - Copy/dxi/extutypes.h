/*
 * Copyright (c) 2002 - 2006 Quentin S. Smith
 * All Rights Reserved
 *
 * Extended types.
 */
#pragma once
#include "unify/unify.h"
#include "d3dx9math.h"

#undef RGB

// TODO: Check that this is REALLY the namespace we want to belong to (unify?)
// TODO: Move functions into associated variable types (Are we sure?)

// Adds to the unify namespace.
namespace unify {

template< class T > class TV2;
class TV2F;
class TV3;
class TV4;
class TQuaternion;
class TColor;
class TColorUnit;
class TColorUnitARGB;
class TTexCoords;
class TPlane;
class TMatrix;
class TMatrixIdentity;
class TBBox;
class TBSphere;
template< class T > class TRect;
class TRectL;
class TViewport;
class TTriangle;
class TRay;

typedef TV2F TSize;


///////////////////////////////////
// TV2
///////////////////////////////////

template < class T >
class TV2
{
public:
	T x, y;

	TV2();
	TV2( T x, T y );
	
	void Zero();

    // binary operators
	TV2<T> & operator = ( const TV2<T> & vec );

	TV2<T> & operator += ( const TV2<T> & vec );
    TV2<T> & operator -= ( const TV2<T> & vec );
    TV2<T> & operator *= ( const TV2<T> & vec );
    TV2<T> & operator /= ( const TV2<T> & vec );
    TV2<T> & operator *= ( T val );
    TV2<T> & operator /= ( T val );

    TV2<T> operator + ( const TV2<T> & vec) const;
    TV2<T> operator - ( const TV2<T> & vec ) const;
    TV2<T> operator * ( const TV2<T> & vec ) const;
    TV2<T> operator / ( const TV2<T> & vec ) const;
    TV2<T> operator * ( T val ) const;
    TV2<T> operator / ( T val ) const;

    TBool operator == ( const TV2<T> & vec ) const;
    TBool operator != ( const TV2<T> & vec ) const;
};


//////////////////////////////////
// TV2F
//////////////////////////////////

class TV2F : public TV2< TFloat >
{
public:
	TV2F();
	TV2F( TFloat x, TFloat y );
	TV2F( const TV2 & vec );
	TV2F & operator = ( const TV2<TFloat> & vIn );
    TFloat Length() const;
	void Normalize();
	void Transform( const TMatrix * pMat );
};

TV2F* qV2Add( TV2F* pOut, const TV2F* pV1, const TV2F* pV2 );
TV2F* qV2BaryCentric( TV2F* pOut, const TV2F* pV1, const TV2F* pV2, const TV2F* pV3, TFloat f, TFloat g );
TV2F* qV2CatmullRom( TV2F* pOut, const TV2F* pV1, const TV2F* pV2, const TV2F* pV3, const TV2F* pV4, TFloat s );
TFloat qV2CCW( const TV2F* pV1, const TV2F* pV2 );
TFloat qV2Dot( const TV2F* pV1, const TV2F* pV2 );
TV2F* qV2Hermite( TV2F* pOut, const TV2F* pV1, const TV2F* pT1, const TV2F* pV2, const TV2F* pT2, TFloat s );
TFloat qV2Length( const TV2F* pV );
TFloat qV2LengthSq( const TV2F* pV );
TV2F* qV2Lerp( TV2F* pOut, const TV2F* pV1, const TV2F* pV2, TFloat s );
TV2F* qV2Maximize( TV2F* pOut, const TV2F* pV1, const TV2F* pV2 );
TV2F* qV2Minimize( TV2F* pOut, const TV2F* pV1, const TV2F* pV2 );
TV2F* qV2Normalize( TV2F* pOut, const TV2F* pV );
TV2F* qV2Scale( TV2F* pOut, const TV2F* pV, TFloat s );
TV2F* qV2Subtract( TV2F* pOut, const TV2F* pV1, const TV2F* pV2 );
TV4* qV2Transform( TV4* pOut, const TV2F* pV, const TMatrix* pM );
TV2F* qV2TransformCoord( TV2F* pOut, const TV2F* pV, const TMatrix* pM );
TV2F* qV2TransformNormal( TV2F* pOut, const TV2F* pV, const TMatrix* pM );
TV4* qV2TransformArray( TV4 *pOut, TUInt32 OutStride, const TV2F* pV, TUInt32 VStride, const TMatrix* pM, TUInt32 n );
TV2F* qV2TransformCoordArray( TV2F* pOut, TUInt32 OutStride, const TV2F* pV, TUInt32 VStride, const TMatrix *pM, TUInt32 n );
TV2F* qV2TransformNormalArray( TV2F* pOut, TUInt32 OutStride, const TV2F* pV, TUInt32 VStride, const TMatrix *pM, TUInt32 n );


///////////////////////////////////
// TV3
///////////////////////////////////

class TV3
{
public:
	TFloat x, y, z;

	TV3();
	TV3( TFloat x, TFloat y, TFloat z );

	void Zero();
	TFloat Length() const;
	void Normalize();
	void Normalize( const TV3 & norm );
	void Transform( const TMatrix* pMat );
	void TransformNormal( const TMatrix* pMat );

	// OPERATOR OVERLOADING MEMBER FUNCTIONS

    TV3 & operator += ( const TV3 & vec );
    TV3 & operator -= ( const TV3 & vec );
    TV3 & operator *= ( const TV3 & vec );
    TV3 & operator /= ( const TV3 & vec );
    TV3 & operator *= ( TFloat val );
    TV3 & operator /= ( TFloat val );

    // unary operators
    //TV3 operator + () const;
    //TV3 operator - () const;

    // binary operators
    TV3 operator + ( const TV3 & vec) const;
    TV3 operator - ( const TV3 & vec ) const;
    TV3 operator * ( const TV3 & vec ) const;
    TV3 operator / ( const TV3 & vec ) const;
    TV3 operator * ( TFloat val ) const;
    TV3 operator / ( TFloat val ) const;

    //friend TV3 operator * ( TFloat, const struct TV3& );

    TBool operator == ( const TV3 & vec ) const;
    TBool operator != ( const TV3 & vec ) const;
};
TV3* qV3Add( TV3* pOut, const TV3* pV1, const TV3* pV2 );
TV3* qV3BaryCentric( TV3* pOut, const TV3* pV1, const TV3* pV2, const TV3* pV3, TFloat f, TFloat g );
TV3* qV3CatmullRom( TV3* pOut, const TV3* pV1, const TV3* pV2, const TV3* pV3, const TV3* pV4, TFloat s );
TV3* qV3Cross( TV3* pOut, const TV3* pV1, const TV3* pV2 );
TFloat qV3Dot( const TV3* pV1, const TV3* pV2 );
TV3* qV3Hermite( TV3* pOut, const TV3* pV1, const TV3* pT1, const TV3* pV2, const TV3* pT2, TFloat s );
TFloat qV3Length( const TV3* pV );
TFloat qV3LengthSq( const TV3* pV );
TV3* qV3Lerp( TV3* pOut, const TV3* pV1, const TV3* pV2, TFloat s );
TV3* qV3Maximize( TV3* pOut, const TV3* pV1, const TV3* pV2 );
TV3* qV3Minimize( TV3* pOut, const TV3* pV1, const TV3* pV2 );
TV3* qV3Normalize( TV3* dest, const TV3* src );
TV3* qV3Scale( TV3* pOut, const TV3* pV, TFloat s );
TV3* qV3Subtract( TV3* pOut, const TV3* pV1, const TV3* pV2 );
TV4* qV3Transform( TV4* pOut, const TV3* pV, const TMatrix* pM );
TV3* qV3TransformCoord( TV3* dest, const TV3* src, const TMatrix* mat );
TV3* qV3TransformNormal( TV3* dest, const TV3* src, const TMatrix* mat );
TFloat qV3Distance( const TV3* pV1, const TV3* pV2 );
TV4* qV3TransformArray( TV4 *pOut, TUInt32 OutStride, const TV3 *pV, TUInt32 VStride, const TMatrix *pM, TUInt32 n );
TV3* qV3TransformCoordArray( TV3* pOut, TUInt32 OutStride, const TV3* pV, TUInt32 VStride, const TMatrix* pM, TUInt32 n );
TV3* qV3TransformNormalArray( TV3 *pOut, TUInt32 OutStride, const TV3 *pV, TUInt32 VStride, const TMatrix *pM, TUInt32 n );
TV3* qV3Project( TV3 *pOut, const TV3 *pV, const TViewport *pViewport, const TMatrix *pProjection, const TMatrix *pView, const TMatrix *pWorld );
TV3* qV3Unproject( TV3 *pOut, const TV3 *pV, const TViewport *pViewport, const TMatrix *pProjection, const TMatrix *pView, const TMatrix *pWorld );
TV3* qV3ProjectArray( TV3 *pOut, TUInt32 OutStride, const TV3 *pV, TUInt32 VStride, const TViewport *pViewport, const TMatrix *pProjection, const TMatrix *pView, const TMatrix *pWorld, TUInt32 n );
TV3* qV3UnprojectArray( TV3 *pOut, TUInt32 OutStride, const TV3 *pV, TUInt32 VStride, const TViewport *pViewport, const TMatrix *pProjection, const TMatrix *pView, const TMatrix *pWorld, TUInt32 n );


///////////////////////////////////
// TV4
///////////////////////////////////

class TV4
{
public:
	TFloat x, y, z, w;

	TV4();
	TV4( TFloat x, TFloat y, TFloat z, TFloat w );

	void Zero();
	TFloat Length() const;
	void Normalize();
	void Transform( const TMatrix* pMat );

	// OPERATOR OVERLOADING MEMBER FUNCTIONS

    TV4 & operator += ( const TV4 & vec );
    TV4 & operator -= ( const TV4 & vec );
    TV4 & operator *= ( const TV4 & vec );
    TV4 & operator /= ( const TV4 & vec );
    TV4 & operator *= ( TFloat val );
    TV4 & operator /= ( TFloat val );

    // unary operators
    //TV4 operator + () const;
    //TV4 operator - () const;

    // binary operators
    TV4 operator + ( const TV4 & vec) const;
    TV4 operator - ( const TV4 & vec ) const;
    TV4 operator * ( const TV4 & vec ) const;
    TV4 operator / ( const TV4 & vec ) const;
    TV4 operator * ( TFloat val ) const;
    TV4 operator / ( TFloat val ) const;

    //friend TV4 operator * ( TFloat, const struct TV4& );

    TBool operator == ( const TV4 & vec ) const;
    TBool operator != ( const TV4 & vec ) const;
};
TFloat qV4Length( const TV4 *pV );
TFloat qV4LengthSq( const TV4* pV );
TFloat qV4Dot( const TV4* pV1, const TV4* pV2 );
TV4* qV4Add( TV4* pOut, const TV4* pV1, const TV4* pV2 );
TV4* qV4Substract( TV4* pOut, const TV4* pV1, const TV4* pV2 );
TV4* qV4Minimize( TV4* pOut, const TV4* pV1, const TV4* pV2 );
TV4* qV4Maximize( TV4* pOut, const TV4* pV1, const TV4* pV2 );
TV4* qV4Scale( TV4* pOut, const TV4* pV, TFloat s );
TV4* qV4Lerp( TV4* pOut, const TV4* pV1, const TV4* pV2, TFloat s );
TV4* qV4Cross( TV4 *pOut, const TV4* pV1, const TV4* pV2, const TV4* pV3 );
TV4* qV4Normalize( TV4 *pOut, const TV4 *pV );
TV4* qV4Hermite( TV4* pOut, const TV4* pV1, const TV4* pT1, const TV4* pV2, const TV4* pT2, TFloat s );
TV4* qV4CatmullRom( TV4* pOut, const TV4* pV0, const TV4* pV1, const TV4* pV2, const TV4* pV3, TFloat s );
TV4* qV4BaryCentric( TV4* pOut, const TV4* pV1, const TV4* pV2, const TV4* pV3, TFloat f, TFloat g );
TV4* qV4Transform( TV4* pOut, const TV4* pV, const TMatrix* pM );
TV4* qV4TransformArray( TV4* pOut, TUInt32 OutStride, const TV4* pV, TUInt32 VStride, const TMatrix* pM, TUInt32 n );

///////////////////////////////////
// TQuaternion
///////////////////////////////////

class TQuaternion
{
public:
	TFloat x, y, z, w;
};
TQuaternion* qQuaternionBaryCentric( TQuaternion* pOut, const TQuaternion* pQ1, const TQuaternion* pQ2, const TQuaternion* pQ3, TFloat f, TFloat g );
TQuaternion* qQuaternionConjugate( TQuaternion* pOut, const TQuaternion* pQ );
TFloat qQuaternionDot( TQuaternion* pQ1, const TQuaternion* pQ2 ); 
TQuaternion* qQuaternionExp( TQuaternion* pOut, const TQuaternion* pQ );
TQuaternion* qQuaternionIdentity( TQuaternion* pOut ); 
TQuaternion* qQuaternionInverse( TQuaternion* pOut, const TQuaternion* pQ ); 
TBool qQuaternionIsIdentity( const TQuaternion* pQ ); 
TFloat qQuaternionLength( const TQuaternion* pQ ); 
TFloat qQuaternionLengthSq( const TQuaternion* pQ );
TQuaternion* qQuaternionLn( TQuaternion* pOut, const TQuaternion* pQ ); 
TQuaternion* qQuaternionMultiply( TQuaternion* pOut, const TQuaternion* pQ1, const TQuaternion* pQ2 );
TQuaternion* qQuaternionNormalize( TQuaternion* pOut, const TQuaternion* pQ ); 
TQuaternion* qQuaternionRotationAxis( TQuaternion* pOut, const TV3* pV, TFloat Angle );
TQuaternion* qQuaternionRotationMatrix( TQuaternion* pOut, const TMatrix* pM ); 
TQuaternion* qQuaternionRotationYawPitchRoll( TQuaternion* pOut, TFloat Yaw, TFloat Pitch, TFloat Roll ); 
TQuaternion* qQuaternionSlerp( TQuaternion* pOut, const TQuaternion* pQ1, const TQuaternion* pQ2, TFloat t ); 
TQuaternion* qQuaternionSquad( TQuaternion* pOut, const TQuaternion* pQ1, const TQuaternion* pA, const TQuaternion* pB, const TQuaternion* pC, TFloat t );
void qQuaternionSquadSetup( TQuaternion* pAOut, TQuaternion* pBOut, TQuaternion* pCOut, const TQuaternion* pQ0, const TQuaternion* pQ1, const TQuaternion* pQ2, const TQuaternion* pQ3 ); 
void qQuaternionToAxisAngle( const TQuaternion* pQ, TV3* pAxis, TFloat* pAngle ); 


///////////////////////////////////
// TColor
///////////////////////////////////

class TColor
{
public:
	TUInt32 c;

	TColor();
	TColor( const TColor & color );
	TColor( TUInt8 r, TUInt8 g, TUInt8 b, TUInt8 a );

	// Specialized TColors...
	static TColor RGBA( TUInt8 r, TUInt8 g, TUInt8 b, TUInt8 a );
	static TColor ARGB( TUInt8 a, TUInt8 r, TUInt8 g, TUInt8 b );
	static TColor RGB( TUInt8 r, TUInt8 g, TUInt8 b );
	static TColor White();
	static TColor Red( TUInt8 r );
	static TColor Green( TUInt8 g );
	static TColor Blue( TUInt8 b );
	static TColor Grey( TUInt8 grey );
	static TColor Black();
	static TColor Zero();

	// CONVERSIONS
	operator TColorUnit ();
	operator TUInt32 ();

	void SetRGBA( TUInt8 r, TUInt8 g, TUInt8 b, TUInt8 a );

	TUInt8 GetRed();
	TUInt8 GetGreen();
	TUInt8 GetBlue();
	TUInt8 GetAlpha();

	void SetRed( TUInt8 r );
	void SetGreen( TUInt8 g );
	void SetBlue( TUInt8 b );
	void SetAlpha( TUInt8 a );
};


///////////////////////////////////
// TColorARGB
///////////////////////////////////

#define TColorWhiteA(_a)		TColorARGB(_a,255,255,255)
#define TColorBlackA(_a)		TColorARGB(_a,0,0,0)
#define TColorRedA(_r,_a)		TColorARGB(_a,_r,0,0)
#define TColorGreenA(_g,_a)		TColorARGB(_a,0,_g,0)
#define TColorBlueA(_b,_a)		TColorARGB(_a,0,0,_b)
#define TColorGreyA(_g,_a)		TColorARGB(_a,_g,_g,_g)
#define TColorGrayA(_g,_a)		TColorARGB(_a,_g,_g,_g)

#define TColorRGBX(_r,_g,_b)	TColorARGB(0,_r,_g,_b)
#define TColorWhiteX			TColorARGB(0,255,255,255)
#define TColorBlackX			TColorARGB(0,0,0,0)
#define TColorRedX(_r)			TColorARGB(0,_r,0,0)
#define TColorGreenX(_g)		TColorARGB(0,0,_g,0)
#define TColorBlueX(_b)			TColorARGB(0,0,0,_b)
#define TColorGreyX(_g)			TColorARGB(0,_g,_g,_g)
#define TColorGrayX(_g)			TColorARGB(0,_g,_g,_g)


///////////////////////////////////
// TColorUnit
///////////////////////////////////

class TColorUnit
{
public:
	TFloat r, g, b, a;

	TColorUnit();

	void SetRGBA( TFloat r, TFloat g, TFloat b, TFloat a );
	void Fix();

	// CONVERSIONS
	operator TColor ();

	// OPERATOR OVERLOADING MEMBER FUNCTIONS
    TColorUnit & operator += ( const TColorUnit & col );
    TColorUnit & operator -= ( const TColorUnit & col );
    TColorUnit & operator *= ( TFloat val );
    TColorUnit & operator /= ( TFloat  val );

    // unary operators
    //TV3 operator + () const;
    //TV3 operator - () const;

    // binary operators
    TColorUnit operator + ( const TColorUnit & col ) const;
    TColorUnit operator - ( const TColorUnit & col ) const;
    TColorUnit operator * ( const TColorUnit & col ) const;
    TColorUnit operator / ( const TColorUnit & col ) const;
    TColorUnit operator * ( TFloat val ) const;
    TColorUnit operator / ( TFloat val ) const;

    //friend TV3 operator * ( TFloat, const struct TV3& );

    TBool operator == ( const TColorUnit & col ) const;
    TBool operator != ( const TColorUnit & col ) const;
};
inline TColorUnit StringToColorUnit( const TString & sString );

///////////////////////////////////
// TColorUnitRGBA
///////////////////////////////////
class TColorUnitARGB : public TColorUnit
{
public:
	TColorUnitARGB( TFloat a, TFloat r, TFloat g, TFloat b );
};

#define TColorUnitRGBA(_r,_g,_b,_a) TColorUnitARGB(_a,_r,_g,_b)
#define TColorUnitZero				TColorUnitARGB(0,0,0,0)

#define TColorUnitWhite				TColorUnitARGB(1,1,1,1)
#define TColorUnitBlack				TColorUnitARGB(1,0,0,0)
#define TColorUnitRed(_r)			TColorUnitARGB(1,_r,0,0)
#define TColorUnitGreen(_g)			TColorUnitARGB(1,0,_g,0)
#define TColorUnitBlue(_b)			TColorUnitARGB(1,0,0,_b)
#define TColorUnitGrey(_g)			TColorUnitARGB(1,_g,_g,_g)
#define TColorUnitGray(_g)			TColorUnitARGB(1,_g,_g,_g)

#define TColorUnitWhiteA(_a)		TColorUnitARGB(_a,1,1,1)
#define TColorUnitBlackA(_a)		TColorUnitARGB(_a,0,0,0)
#define TColorUnitRedA(_r,_a)		TColorUnitARGB(_a,_r,0,0)
#define TColorUnitGreenA(_g,_a)		TColorUnitARGB(_a,0,_g,0)
#define TColorUnitBlueA(_b,_a)		TColorUnitARGB(_a,0,0,_b)
#define TColorUnitGreyA(_g,_a)		TColorUnitARGB(_a,_g,_g,_g)
#define TColorUnitGrayA(_g,_a)		TColorUnitARGB(_a,_g,_g,_g)

#define TColorUnitWhiteX			TColorUnitARGB(1,1,1,1)
#define TColorUnitBlackX			TColorUnitARGB(0,0,0,0)
#define TColorUnitRedX(_r)			TColorUnitARGB(0,_r,0,0)
#define TColorUnitGreenX(_g)		TColorUnitARGB(0,0,_g,0)
#define TColorUnitBlueX(_b)			TColorUnitARGB(0,0,0,_b)
#define TColorUnitGreyX(_g)			TColorUnitARGB(0,_g,_g,_g)
#define TColorUnitGrayX(_g)			TColorUnitARGB(0,_g,_g,_g)


///////////////////////////////////
// TTexCoords
///////////////////////////////////
class TTexCoords
{
public:
	TFloat u, v;

	TTexCoords();
	TTexCoords( TFloat u, TFloat v );

	// OPERATOR OVERLOADING MEMBER FUNCTIONS

    TTexCoords & operator += ( const TTexCoords & vec );
    TTexCoords & operator -= ( const TTexCoords & vec );
    TTexCoords & operator *= ( TFloat val );
    TTexCoords & operator /= ( TFloat val );

    // unary operators
    //TTexCoords operator + () const;
    //TTexCoords operator - () const;

    // binary operators
    TTexCoords operator + ( const TTexCoords & vec) const;
    TTexCoords operator - ( const TTexCoords & vec ) const;
    TTexCoords operator * ( const TTexCoords & vec ) const;
    TTexCoords operator / ( const TTexCoords & vec ) const;
    TTexCoords operator * ( TFloat val ) const;
    TTexCoords operator / ( TFloat val ) const;

    //friend TV3 operator * ( TFloat, const struct TV3& );

    TBool operator == ( const TTexCoords & vec ) const;
    TBool operator != ( const TTexCoords & vec ) const;

	TTexCoords & Transform( const TMatrix* pMatrix );
};
#define TTexAreaOne					TTexArea( TTexCoords(0,0) TTexCoords(1,1) )

///////////////////////////////////
// TTexArea
///////////////////////////////////
class TTexArea
{
public:
	TTexCoords ul;
	TTexCoords dr;

	TTexArea();
	TTexArea( const TTexCoords&, const TTexCoords& );

	TFloat Width() const;
	TFloat Height() const;

	TTexCoords Size() const;
};
#define TTexAreaFull TTexArea( TTexCoords( 0, 0 ), TTexCoords( 1, 1 ) ) 
#define TTexAreaDouble TTexArea( TTexCoords( 0, 0 ), TTexCoords( 2, 2 ) )

///////////////////////////////////
// TPlane
///////////////////////////////////
class TPlane
{
public:
	TFloat a, b, c, d;

	TPlane();
	TPlane( TFloat a, TFloat b, TFloat c, TFloat d );

	TFloat DotCoord( const TV3* pV ) const;
	TFloat DotNormal( const TV3* pV ) const;
};
TPlane* qPlaneFromPointNormal( TPlane* pOut, const TV3* pPoint, const TV3* pNormal );
TPlane* qPlaneFromPoints( TPlane* pOut, const TV3* pV1, const TV3* pV2, const TV3* pV3 );
TV3* qPlaneIntersectLine( TV3* pOut, const TPlane* pP, const TV3* pV1, const TV3* pV2 );
TPlane* qPlaneNormalize( TPlane* pOut, const TPlane* pP );
TPlane* qPlaneTransform( TPlane* pOut, const TPlane* pP, const TMatrix* pM );


///////////////////////////////////
// TMatrix
///////////////////////////////////

class TMatrix
{
public:
	//TMatrix mat;
	//D3DXMATRIXA16 mat;
	D3DXMATRIX mat;

	TV3* GetLeft();
	TV3* GetUp();
	TV3* GetDir();	// Used to be "At", this is not a correct name
	TV3* GetPos();

	TFloat Determinant();
	TBool IsIdentity();

	void Inverse();
	void Transpose();

	TMatrix & operator = ( const TMatrix & matrix );

	// OPERATOR OVERLOADING MEMBER FUNCTIONS
	TMatrix& operator += ( const TMatrix& mat );
    TMatrix& operator -= ( const TMatrix& mat );
    TMatrix& operator *= ( const TMatrix& mat );
    TMatrix& operator *= ( TFloat val );
    TMatrix& operator /= ( TFloat val );

    // unary operators
    //TV3 operator + () const;
    //TV3 operator - () const;

    // binary operators
    TMatrix operator + ( const TMatrix& mat ) const;
    TMatrix operator - ( const TMatrix& mat ) const;
    TMatrix operator * ( const TMatrix& mat ) const;
    TMatrix operator * ( TFloat val ) const;
    TMatrix operator / ( TFloat val ) const;

    //friend TV3 operator * ( TFloat, const struct TV3& );

    TBool operator == ( const TMatrix& mat ) const;
    TBool operator != ( const TMatrix& mat ) const;

	TFloat& operator()( TUInt32 uRow, TUInt32 uColumn );
};
TMatrix* qMatrixIdentity( TMatrix* pMatrix );
TMatrix* qMatrixLookAtLH( TMatrix* pOut, const TV3* pEye, const TV3* pAt, const TV3* pUp );
TMatrix* qMatrixLookAtRH( TMatrix* pOut, const TV3* pEye, const TV3* pAt, const TV3* pUp );
TMatrix* qMatrixMultiply( TMatrix* pOut, const TMatrix* pM1, const TMatrix* pM2 );
TMatrix* qMatrixMultiplyTranspose( TMatrix* pOut, const TMatrix* pM1, const TMatrix* pM2 );
TMatrix* qMatrixOrthoLH( TMatrix* pOut, TFloat w, TFloat h, TFloat zn, TFloat zf );
TMatrix* qMatrixOrthoRH( TMatrix* pOut, TFloat w, TFloat h, TFloat zn, TFloat zf );
TMatrix* qMatrixOrthoOffCenterLH( TMatrix* pOut, TFloat l, TFloat r, TFloat b, TFloat t, TFloat zn, TFloat zf );
TMatrix* qMatrixOrthoOffCenterRH( TMatrix* pOut, TFloat l, TFloat r, TFloat b, TFloat t, TFloat zn, TFloat zf );
TMatrix* qMatrixInverse( TMatrix* pOut, TFloat* pDeterminant, const TMatrix* pM );
TMatrix* qMatrixAffineTransformation( TMatrix* pOut, TFloat scaling, const TV3* pRotationCenter, const TQuaternion* pRotation, const TV3* pTranslation );
TMatrix* qMatrixPerspectiveFovLH( TMatrix* pOut, TFloat fovy, TFloat Aspect, TFloat zn, TFloat zf );
TMatrix* qMatrixPerspectiveFovRH( TMatrix* pOut, TFloat fovy, TFloat Aspect, TFloat zn, TFloat zf );
TMatrix* qMatrixPerspectiveLH( TMatrix* pOut, TFloat w, TFloat h, TFloat zn, TFloat zf );
TMatrix* qMatrixPerspectiveRH( TMatrix* pOut, TFloat w, TFloat h, TFloat zn, TFloat zf );
TMatrix* qMatrixPerspectiveOffCenterLH( TMatrix* pOut, TFloat l, TFloat r, TFloat b, TFloat t, TFloat zn, TFloat zf );
TMatrix* qMatrixPerspectiveOffCenterRH( TMatrix* pOut, TFloat l, TFloat r, TFloat b, TFloat t, TFloat zn, TFloat zf );
TMatrix* qMatrixReflect( TMatrix* pOut, const TPlane* pPlane );
TMatrix* qMatrixRotationAxis( TMatrix* pMatrix, const TV3* vAxis, TFloat fAngle );
TMatrix* qMatrixRotationQuaternion( TMatrix* pOut, const TQuaternion* pQ );
TMatrix* qMatrixRotationX( TMatrix* pOut, TFloat Angle );
TMatrix* qMatrixRotationY( TMatrix* pOut, TFloat Angle );
TMatrix* qMatrixRotationZ( TMatrix* pOut, TFloat Angle );
TMatrix* qMatrixRotationYawPitchRoll( TMatrix* pOut, TFloat Yaw, TFloat Pitch, TFloat Roll );
TMatrix* qMatrixScaling( TMatrix* pOut, TFloat sx, TFloat sy, TFloat sz );
TMatrix* qMatrixShadow( TMatrix* pOut, const TV4* pLight, const TPlane* pPlane );
TMatrix* qMatrixTransformation( TMatrix* pOut, const TV3* pScalingCenter, const TQuaternion* pScalingRotation,
									const TV3* pScaling, const TV3* pRotationCenter, const TQuaternion* pRotation, const TV3* pTranslation );
TMatrix* qMatrixTranslation( TMatrix* pOut, TFloat x, TFloat y, TFloat z );
TMatrix* qMatrixTranslation( TMatrix* pOut, const TV3* pVec );
TMatrix* qMatrixTranspose( TMatrix* pOut, const TMatrix* pM );


///////////////////////////////////
// TMatrixIdentity
///////////////////////////////////

class TMatrixIdentity : public TMatrix
{
public:
	TMatrixIdentity();
};


///////////////////////////////////
// TBBox
///////////////////////////////////

class TBBox
{
public:
	TV3 sup;
	TV3 inf;

	TBBox();
	TBBox( const TV3 & inInf, const TV3 & inSup );

	TBBox operator*( const TFloat & fFloat );
	
	void GenerateCorners( TV3* pBounds );
	void Initialize( const TV3* pInf = &TV3( 0, 0, 0 ), const TV3* pSup = &TV3( 0, 0, 0 ) );
	void AddPoint( const TV3* pV );
	void AddBBox( const TBBox* pBBox );
	TBool ContainsPoint( const TV3* pV );
	const TV3 Size();
};


///////////////////////////////////
// TBSphere
///////////////////////////////////

class TBSphere
{
public:
	TFloat fRadius;
};


///////////////////////////////////
// TDataLock
///////////////////////////////////

class TDataLock
{
public:
	void* pData;
	TUInt32 uStride;
	TUInt32 uCount;	// Number of items we can stride through

	void Invalidate();
};


///////////////////////////////////
// TRect
///////////////////////////////////

template< class T >
class TRect
{
public:
	T left;
	T top;
	T right;
	T bottom;

	TRect();
	TRect( T l, T t, T r, T b );
	TRect( const TV2<T> & vUL, const TV2<T> & vDR );

	T Width() const;

	T Height() const;

	TV2<T> UL() const;
	TV2<T> DR() const;

	void And( const TRect<T> & rect );

	TRect & operator += ( const TRect<T> & rect );
	TRect & operator -= ( const TRect<T> & rect );
	TRect operator + ( const TRect<T> & rect ) const;
	TRect operator - ( const TRect<T> & rect ) const;
	TRect & operator = ( const TRect<T> & rect );

	void SizeTo( T lWidth, T lHeight );
	TBool IsIn( T x, T y ) const;
	void MoveTo( T x, T y );
	void MoveBy( T x, T y );
	void MoveTo( const TV2<T> & pVec );
	void MoveBy( const TV2<T> & pVec );
	void AddToRegion( const TV2<T> & pVec );
};


///////////////////////////////////
// TRectL
///////////////////////////////////

class TRectL : public TRect<TLong> 
{
public:
	TRectL();
	TRectL( TLong l, TLong t, TLong r, TLong b );
	TRectL & operator = ( const TRect<TLong> & rect );
};


///////////////////////////////////
// Radius...
///////////////////////////////////

void qRadiusAddPoint( TFloat* pRadius, const TV3* pVec );
void qRadiusAddRadius( TFloat* pRadiusOut, TFloat pRadiusIn );


///////////////////////////////////
// TViewport
///////////////////////////////////

class TViewport
{
public:
    TDWord		X;
    TDWord		Y;            /* Viewport Top left */
    
    /* Viewport Dimensions */
	TDWord		Width;
    TDWord		Height;
    
	// Min/max of clip Volume...
    TFloat		MinZ;
    TFloat		MaxZ;
};


///////////////////////////////////
// TTriangle
///////////////////////////////////

class TTriangle
{
	TV3 v[3];

	TTriangle( const TV3* pV1, const TV3* pV2, const TV3* pV3 );
	TTriangle & operator = ( const TTriangle & triangle );
};


///////////////////////////////////
// TRay
///////////////////////////////////
class TRay
{
public:
	TRay( const TV3 & _pos, const TV3 & _dir );

	TV3 pos;
	TV3 dir;
};
inline TBool RayIntersectsBBox( const TBBox * pBBox, const TRay * pRay ); 

#include "dxi\extutypes.inl"

}; // namespace unify
