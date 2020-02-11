// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/sculpter/Sculpter.h>
#include <me/render/VertexUtil.h>

using namespace me;
using namespace sculpter;

Sculpter::Sculpter()
	: m_stats{}
{}

Sculpter::~Sculpter()
{}

void Sculpter::AddSheet( SheetStats sheetStats, sculpter::IVertexBuilder::ptr vertexBuilder, sculpter::IFaceBuilder::ptr faceBuilder )
{
	Sheet sheet{};
	sheet.stats = sheetStats;
	sheet.stats.index = m_stats.totalSheets;
	sheet.stats.totalVertices = sheetStats.totalVertices;
	sheet.stats.totalFaces = sheetStats.totalFaces;
	sheet.vertexBuilder = vertexBuilder;
	sheet.faceBuilder = faceBuilder;

	m_stats.totalSheets += 1;
	m_stats.totalVertices += sheet.stats.totalVertices;
	m_sheets.push_back( sheet );
}

bool Sculpter::Build( me::render::PrimitiveList & primitiveList, me::render::Effect::ptr effect )
{
	using namespace me;
	using namespace render;

	assert( m_stats.totalSheets == m_sheets.size() );

	std::vector< VertexOutput > vertexOuptut;
	std::vector< Index32 > indices;

	for( size_t sheetIndex = 0; sheetIndex < m_stats.totalSheets; sheetIndex++ )
	{
		auto sheet = &m_sheets[sheetIndex];
		VertexOutput previousVertexOutput{};
		for( size_t vertexIndex = 0; vertexIndex < sheet->stats.totalVertices; vertexIndex++ )
		{
			VertexInput vertexInput{};
			vertexInput.index = vertexIndex;
			vertexInput.previous = previousVertexOutput;
			previousVertexOutput = sheet->vertexBuilder->Build( sheet->stats, vertexInput );
			vertexOuptut.push_back( previousVertexOutput );
		}

		FaceOutput previousFaceOutput{};
		for( size_t faceIndex = 0; faceIndex < sheet->stats.totalFaces; faceIndex++ )
		{
			FaceInput faceInput{};
			faceInput.index = faceIndex;
			faceInput.previous = previousFaceOutput;
			previousFaceOutput = sheet->faceBuilder->Build( sheet->stats, faceInput );

			indices.insert( indices.end(), previousFaceOutput.indices.begin(), previousFaceOutput.indices.end() );

			sheet->stats.totalIndices += previousFaceOutput.indices.size();
			m_stats.totalTriangles += 1;
		}
		m_stats.totalIndices += sheet->stats.totalIndices;


		BufferUsage::TYPE bufferUsage = BufferUsage::Default;
		BufferSet & set = primitiveList.AddBufferSet();

		// Create vertex chunk...
		VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
		std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSizeInBytes( 0 ) * sheet->stats.totalVertices] );
		unify::DataLock lock( vertices.get(), vd->GetSizeInBytes( 0 ), sheet->stats.totalVertices, unify::DataLockAccess::ReadWrite, 0 );

		size_t triangleCount = sheet->stats.totalTriangles;
		size_t indexCount = sheet->stats.totalIndices;
		VertexBufferParameters vbParameters{ vd, { { sheet->stats.totalVertices, vertices.get() } }, bufferUsage };

		// Get vertex elements...
		int stream = 0;
		VertexElement positionE = CommonVertexElement::Position( stream );
		VertexElement normalE = CommonVertexElement::Normal( stream );
		VertexElement diffuseE = CommonVertexElement::Diffuse( stream );
		VertexElement specularE = CommonVertexElement::Specular( stream );
		VertexElement texE = CommonVertexElement::TexCoords( stream );

		const unify::Matrix origin = unify::MatrixIdentity();
		for( size_t vertexIndex = 0; vertexIndex < sheet->stats.totalVertices; vertexIndex++ )
		{
			auto vertex = vertexOuptut[vertexIndex];
			WriteVertex( *vd, lock, vertexIndex, positionE, vertex.vertex.pos );
			WriteVertex( *vd, lock, vertexIndex, normalE, vertex.vertex.normal );
			WriteVertex( *vd, lock, vertexIndex, texE, vertex.vertex.coords );
			WriteVertex( *vd, lock, vertexIndex, diffuseE, vertex.vertex.diffuse );
			WriteVertex( *vd, lock, vertexIndex, specularE, vertex.vertex.specular );
		}

		set.SetEffect( effect );
		set.AddVertexBuffer( vbParameters );
		set.AddIndexBuffer( { { { sheet->stats.totalIndices, &indices[0] } }, bufferUsage } );
		set.AddMethod( RenderMethod::CreateTriangleListIndexed( sheet->stats.totalVertices, indexCount, 0, 0 ) );
	}
	return true;
}