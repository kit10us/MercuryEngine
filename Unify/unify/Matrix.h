/* Copyright (c) 2002 - 2014, Quentin S. Smith
* All Rights Reserved
*/

#pragma once

#include <unify/Unify.h>
#include <unify/V2.h>
#include <unify/V3.h>
#include <unify/V4.h>
#include <unify/RowColumn.h>
#include <unify/Quaternion.h>
#include <unify/Angle.h>
#include <memory>

namespace unify
{
	__declspec(align(16)) class Matrix
	{
	public:
#if defined ( DIRECTX9 )
		typedef D3DXMATRIX DXMatrix;
#elif defined( DIRECTX11 )
		typedef DirectX::XMMATRIX DXMatrix;
#endif

		static Matrix MatrixIdentity();
		static Matrix MatrixTranslate( const V3< float > & vector );
		static Matrix MatrixOrthoOffCenterLH( float left, float right, float bottom, float top, float znear, float zfar );
		static Matrix MatrixScale( const unify::V3< float > & scale );
		static Matrix MatrixScale( float x, float y, float z );
		static Matrix MatrixScale( float scale );
		static Matrix MatrixRotationAboutAxis( const V3< float > & axis, Angle angle );
		static Matrix MatrixPerspectiveFovLH( float w, float h, float zn, float zf );
		static Matrix MatrixRotationX( const float & x );
		static Matrix MatrixRotationY( const float & y );
		static Matrix MatrixRotationZ( const float & z );
		static Matrix MatrixLookAtLH( const V3< float > & eyePosition, const V3< float > & at, const V3< float > & up );

		Matrix();
		Matrix( const Matrix & matrix );
		explicit Matrix( const Quaternion orientation, const V3< float > position = V3< float >( 0, 0, 0 ), const V3< float > scale = V3< float >( 1, 1, 1 ) );
		~Matrix();

		Matrix & operator = ( const Matrix & matrix );

		Matrix & operator += ( const Matrix & matrix );
		Matrix & operator -= ( const Matrix & matrix );
		Matrix & operator *= ( const Matrix & matrix );
		Matrix & operator *= ( const Quaternion & quaternion );
		Matrix & operator *= ( float multiplier );
		Matrix & operator /= ( float divisor );

		// binary operators
		Matrix operator + ( const Matrix & matrix ) const;
		Matrix operator - ( const Matrix & matrix ) const;
		Matrix operator * ( const Matrix & matrix ) const;
		Matrix operator * ( const Quaternion & quaternion ) const;
		Matrix operator * ( float multiplier ) const;
		Matrix operator / ( float divisor ) const;

		bool operator == ( const Matrix & matrix ) const;
		bool operator != ( const Matrix & matrix ) const;

		float operator()( const RowColumn< unsigned int > & rowColumn );
		float operator()( unsigned int row, unsigned int column );

		float operator()( const RowColumn< unsigned int > & rowColumn ) const;
		float operator()( unsigned int row, unsigned int column ) const;

		Matrix & SetRotation( const Quaternion & quaternion );
		Matrix & Translate( const V3< float > & vector );
		Matrix & Scale( const V3< float > & vector );
		Matrix & Scale( const float & scale );

		void Set( unsigned int row, unsigned int column, float value );

		void SetLeft( const V3< float > & vec );
		void SetUp( const V3< float > & vec );
		void SetForward( const V3< float > & vec );
		void SetPosition( const V3< float > & vec );
		void SetLeft( float x, float y, float z );
		void SetUp( float x, float y, float z );
		void SetForward( float x, float y, float z );
		void SetPosition( float x, float y, float z );

		V3< float > GetLeft() const;
		V3< float > GetUp() const;
		V3< float > GetForward() const;
		V3< float > GetPosition() const;
		V3< float > GetScale() const;
		Quaternion GetRotation() const;

		float Determinant() const;
		bool IsIdentity() const;

		void BecomeIdentity();
		void Zero();

		/// <summary>
		/// Inverts this matrix.
		/// </summary>
		void Invert();

		/// <summary>
		/// Returns a copy of the inverse of this matrix, leaving the original matrix in tact.
		/// </summary>
		Matrix Inverse() const;

		void Transpose();
		void Transpose( const Matrix & matrix );

		DXMatrix * unify::Matrix::GetD3DXMatrix();
		const DXMatrix * unify::Matrix::GetD3DXMatrix() const;

		void TransformCoord( V2< float > & v2 ) const;
		void TransformCoord( V3< float > & v3 ) const;
		V2< float > TransformCoord_Copy( const V2< float > & v2 ) const;
		V3< float > TransformCoord_Copy( const V3< float > & v3 ) const;
		void TransformNormal( V2< float > & v2 ) const;
		void TransformNormal( V3< float > & v3 ) const;

		void Transform( V4< float > & v4 ) const;
		void RotateAboutAxis( const V3< float > & axis, Angle angle );

		void LookAtLH( const V3< float > & at, const V3< float > & up );

		std::string ToString() const;

	protected:
		/*
		#if defined( DIRECTX9 )
		std::shared_ptr< DXMatrix > m_matrix;
		#elif defined( DIRECTX11 )
		std::shared_ptr< DXMatrix > m_matrix;
		#endif
		*/
		float m_matrix[4][4];
	};
}