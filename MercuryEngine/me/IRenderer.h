// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <me/Viewport.h>
#include <me/Display.h>
#include <me/IVertexBuffer.h>
#include <me/IIndexBuffer.h>
#include <me/IVertexShader.h>
#include <me/IPixelShader.h>
#include <me/ITexture.h>
#include <me/RenderInstance.h>
#include <unify/FrameLite.h>
#include <memory>

namespace me
{
	class VertexDeclaration;

	struct CullMode
	{
		enum TYPE
		{
			None,
			Clockwise,
			CounteClockwise
		};
	};

	struct InstancesSet
	{
		const unify::FrameLite ** instances;
		const size_t instances_size;
	};				

	class IMatrixSource
	{
	public:
		IMatrixSource() {}

		virtual size_t Count() const = 0;
		virtual unify::Matrix GetMatrix( size_t i ) const = 0;
		virtual void CopyMatrices( unify::Matrix * matrices ) const = 0;
	};


	/// <summary>
	/// Supports access to the renderer.
	/// </summary>
	class IRenderer
	{
	public:
		typedef std::shared_ptr< IRenderer > ptr;

		virtual ~IRenderer() {}

		virtual const me::Display & GetDisplay() const = 0;

		virtual Viewport GetViewport() const = 0;

		/// <summary>
		/// Called before render to allow renderer to perapre.
		/// </summary>
		virtual void BeforeRender() = 0;

		/// <summary>
		/// Called after render to allow renderer to cleanup.
		/// </summary>
		virtual void AfterRender() = 0;

		// New support for render states
		virtual void SetCullMode( CullMode::TYPE mode ) = 0;

		virtual bool IsFullscreen() const = 0;

		virtual size_t GetIndex() const = 0;

		virtual void* GetHandle() const = 0;

		virtual void Render( const class RenderMethod & method, const RenderInfo & renderInfo, const unify::Matrix * instance, const size_t instances_size ) = 0;

		virtual void RenderInstanced( const RenderMethod & method, const RenderInfo & renderInfo, const unify::FrameLite ** instances, const size_t instances_size ) = 0;
		
		virtual void RenderInstanced( const RenderMethod & method, const RenderInfo & renderInfo, const InstancesSet * instancesList, const size_t instancesList_size ) = 0;

		virtual void RenderInstanced( const RenderMethod & method, const RenderInfo & renderInfo, const IMatrixSource * sources, const size_t sources_size, bool contiguous ) = 0;

		virtual IVertexBuffer::ptr ProduceVB( VertexBufferParameters parameters ) const = 0;
		virtual IIndexBuffer::ptr ProduceIB( IndexBufferParameters parameters ) const = 0;
		virtual IVertexShader::ptr ProduceVS( VertexShaderParameters parameters ) const = 0;
		virtual IPixelShader::ptr ProducePS( PixelShaderParameters parameters ) const = 0;
		virtual IVertexConstruct::ptr ProduceVC( const VertexDeclaration & vd, const IVertexShader & vs ) const = 0;
		virtual ITexture::ptr ProduceT( TextureParameters parameters ) const = 0;
	};
}

