// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/Mercury.h>
#include <me/render/IBuffer.h>
#include <me/render/ResourceType.h>
#include <me/render/ConstantTable.h>
#include <me/render/RenderInfo.h>

namespace me 
{
	namespace render
	{
		struct ConstantBufferParameters
		{
			ConstantBufferParameters( ResourceType::TYPE type, BufferUsage::TYPE usage, const ConstantTable & constantTable )
				: type{ type }
				, usage{ usage }
				, constantTable{ constantTable }
			{
			}

			ResourceType::TYPE type;
			BufferUsage::TYPE usage;
			ConstantTable constantTable;
		};

		class IConstantBuffer
		{
		public:
			typedef std::shared_ptr< IConstantBuffer > ptr;

			virtual ConstantBufferParameters GetParameters() const = 0;

			virtual const me::render::ConstantTable * GetTable() const = 0;

			virtual void Create( ConstantBufferParameters parameters ) = 0;
			virtual void Destroy() = 0;

			virtual size_t GetBufferCount() const = 0;

			virtual void Update( const RenderInfo & renderInfo, const unify::Matrix * world, size_t world_size ) = 0;

			virtual void Use( size_t startSlot, size_t startBuffer ) = 0;

			virtual void LockConstants( size_t bufferIndex, unify::DataLock & lock ) = 0;
			virtual void UnlockConstants( size_t buffer, unify::DataLock & lock ) = 0;

			virtual ResourceType::TYPE GetType() const = 0;
			virtual BufferUsage::TYPE GetUsage() const = 0;
		};
	}
}