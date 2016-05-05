// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/loader/MeshLoaders.h>
#include <dxi/loader/EffectLoader.h>
#include <dxi/ASEDocument.h>
#include <dxi/loader/MeshLoaderX.h>
#include <dxi/XMLConvert.h>
#include <unify/String.h>
#include <unify/FrameSet.h>

using namespace dxi;

void loader::LoadMesh_XML_V1_0( const unify::Path & filePath, PrimitiveList & primitiveList, Managers managers )
{
	// Destroy exiting primitive list...
	primitiveList.Destroy();

	qxml::Document document( filePath );
	qxml::Element * element;

	//////////////////////////////
	// Frames...
	//////////////////////////////

	qxml::Element * framesElement = document.FindElement( "frames" );
	if( framesElement )
	{
		qxml::ElementList childrenFrames;
		framesElement->FindElementsByTagName( childrenFrames, "frame" );
		for( unsigned int u = 0; u < childrenFrames.Count(); ++u )
		{
			qxml::Element * frameElement = childrenFrames.Item( u );
			std::string name = frameElement->HasAttributes( "name" ) ? frameElement->GetStringAttribute( "name" ) : std::string();
			size_t index = static_cast< size_t >( frameElement->GetIntegerAttribute( "index" ) );
			size_t parent = static_cast< size_t >( frameElement->GetIntegerAttribute( "parent" ) );

			unify::Matrix matrix;
			XMLConvert( frameElement, matrix );

			unify::FrameSet & frameSet = primitiveList.GetFrameSet();
			assert( index == frameSet.Count() );
			frameSet.Add( matrix, parent, name ) ;
		}
	}


	//////////////////////////////
	// Animations...
	//////////////////////////////

	qxml::Element * animationSetElement = document.FindElement( "animationset" );
	if( animationSetElement )
	{
		frameanimation::AnimationSet & animationSet = primitiveList.GetAnimationSet();

		for( qxml::Element * animationItr = animationSetElement->GetFirstChild(); animationItr != 0; animationItr = animationItr->GetNext() )
		{
			if( ! animationItr->IsTagName( "animation" ) )
			{
				continue;
			}

			std::string name = animationItr->GetStringAttribute( "name" );
			unify::Seconds duration = animationItr->GetFloatAttribute( "duration" );

			frameanimation::Animation::shared_ptr animation( new frameanimation::Animation( name, duration ) );

			for( qxml::Element * frameItr = animationItr->GetFirstChild(); frameItr != 0; frameItr = frameItr->GetNext() )
			{
				if( ! frameItr->IsTagName( "frame" ) )
				{
					continue;
				}

				size_t targetBoneIndex;
				if( frameItr->HasAttributes( "targetName" ) )
				{
					std::string targetFrameName = frameItr->GetStringAttribute( "target" );
					targetBoneIndex = primitiveList.GetFrameSet().Find( targetFrameName );
				}
				else if( frameItr->HasAttributes( "targetIndex" ) )
				{
					targetBoneIndex = frameItr->GetAttribute< size_t >( "targetIndex" );
				}
				else
				{
					throw unify::Exception( "animationset's animation requires a target frame specified by either framename or frameindex!" );
				}

				for( qxml::Element * keyItr = frameItr->GetFirstChild(); keyItr != 0; keyItr = keyItr->GetNext() )
				{
					std::string type = keyItr->GetStringAttribute( "type" );
					unify::Seconds time;
					if( keyItr->HasAttributes( "time" ) )
					{
						time = keyItr->GetFloatAttribute( "time" );
					}
					else if( keyItr->HasAttributes( "progress" ) )
					{
						time = keyItr->GetFloatAttribute( "progress" ) * duration;
					}
					else
					{
						throw unify::Exception( "Key missing time or progress attribute!" );
					}

					if( unify::StringIs( type, "scale" ) )
					{
						unify::V3< float > value;
						XMLConvert( keyItr, value );
						animation->AddScaleKey( targetBoneIndex, frameanimation::ScaleKey( time, value ) );
					}
					else if( unify::StringIs( type, "rotation" ) )
					{
						unify::Quaternion value;
						XMLConvert( keyItr, value );
						animation->AddRotationKey( targetBoneIndex, frameanimation::RotationKey( time, value ) );
					}
					else if( unify::StringIs( type, "translation" ) )
					{
						unify::V3< float > value;
						XMLConvert( keyItr, value );
						animation->AddTranslationKey( targetBoneIndex, frameanimation::TranslationKey( time, value ) );
					}
					else
					{
						throw unify::Exception( "Unrecognized animaion key type! (" + type + ")" );
					}
				}

			}

			animationSet.AddAnimation( animation );
		}
	}

	//////////
	// Load Indices...
	//////////

	BufferSet & set = primitiveList.AddBufferSet();

	unsigned int uNumIndices = 0;
	element = document.FindElement( "indexlist" );
	if( element )
	{
		uNumIndices = unify::ListPartCount( element->GetText(), ',' );

		IndexBuffer & ib = set.GetIndexBuffer();
		ib.Create( uNumIndices, IndexFormat::Index16, BufferUsage::Default );
		IndexLock indexLock;
		ib.Lock( indexLock );
		Index16* pIndex = (Index16 *)indexLock.GetData();

		// Load indices...
		for( unsigned int u = 0; u < uNumIndices; u++ )
		{
			pIndex[ u ] = (Index16)unify::Cast< int >( unify::ListPart( element->GetText(), ',', u ) );
		}
		ib.Unlock();
	}

	//////////////////////////////
	// Load effects...
	//////////////////////////////

	qxml::ElementList listEffects;
	document.FindElementsByTagNameRecursive( listEffects, "effect" );
	Effect::shared_ptr firstEffect;
	for( unsigned int uEffect = 0; uEffect < listEffects.Count(); ++uEffect )
	{
		Effect::shared_ptr effect;
		element = listEffects.Item( uEffect );
		std::string name = element->GetStringAttribute( "name" );
		if ( element->HasAttributes( "source" ) )
		{
			unify::Path source( document.GetPath().DirectoryOnly(), element->GetStringAttribute("source") );
			qxml::Document doc( source );
			const qxml::Element * sourceNode = doc.GetRoot();
			effect = loader::EffectLoader( name, sourceNode, managers );
		}
		else 
		{
			effect = loader::EffectLoader( name, element, managers );
		}

		if ( ! firstEffect )
		{
			firstEffect = effect;
		}

		//TODO: NOT A PART OF EFFECT ANY LONGER!
		//qxml::Element* subElement = element->GetFirstChild();
		//while( subElement )
		//{
		//	if( subElement->IsTagName( "frame" ) )
		//	{
		//		const unify::FrameSet & frameSet = primitiveList.GetFrameSet();
		//		size_t frameIndex;
		//		if( subElement->HasAttributes( "frameName" ) )
		//		{
		//			std::string frameName = subElement->GetAttribute( "frameName" )->GetString();
		//			frameIndex = frameSet.Find( frameName );
		//		}
		//		else if( subElement->HasAttributes( "frameIndex" ) )
		//		{
		//			frameIndex = subElement->GetAttribute< size_t >( "frameIndex" );
		//		}
		//		else
		//		{
		//			throw unify::Exception( "addeffect's frame must include either framename or frameindex!" );
		//		}

		//		float influence = subElement->GetAttribute( "influence" )->GetFloat();
		//		effect.AddFrame( frameIndex, influence );
		//	}

		//	subElement = subElement->GetNext();
		//}
	}


	//////////////////////////////
	// Load vertices...
	//////////////////////////////

	element = document.FindElement( "vertexlist" );

	// Vertex declaration
	VertexDeclaration vd;
	if( element->HasElements( "vd" ) )
	{
		// Pull from child element if specified...
		vd = VertexDeclaration( *element->GetElement( "vd" ) );
	}
	else
	{
		// Pull from first effect...
		vd = firstEffect->GetVertexShader()->GetVertexDeclaration();		
	}

	unsigned int uNumVertices = document.CountElements( "vertex" );
	VertexBuffer & vb = set.GetVertexBuffer();
	try
	{
		vb.Create( uNumVertices, vd, BufferUsage::Default );
	}
	catch( ... )
	{
		primitiveList.Destroy();
		throw;
	}

	D3DVERTEXELEMENT9 positionE = {};
	vd.GetElement( "Position", positionE );

	D3DVERTEXELEMENT9 normalE = {};
	vd.GetElement( "Normal", normalE );

	D3DVERTEXELEMENT9 texE = {};
	texE.Usage = D3DDECLUSAGE_TEXCOORD;
	texE.UsageIndex = 0;
	vd.FindElement( texE, texE );

	D3DVERTEXELEMENT9 tex2E = {};
	tex2E.Usage = D3DDECLUSAGE_TEXCOORD;
	tex2E.UsageIndex = 0;
	vd.FindElement( tex2E, tex2E );

	D3DVERTEXELEMENT9 diffuseE = {};
	diffuseE.Usage = D3DDECLUSAGE_COLOR;
	diffuseE.UsageIndex = 0;
	vd.FindElement( diffuseE, diffuseE );

	unify::DataLock lock;
	vb.Lock( lock );

	qxml::ElementList listVertices;
	document.FindElementsByTagNameRecursive( listVertices, "vertex" );

	if ( listVertices.Count() > 0 )
	{
		for( unsigned int u = 0; u < listVertices.Count(); u++ )
		{
			element = listVertices.Item( u );
			unsigned int index = element->GetIntegerAttribute( "index" );

			// XYZ
			unify::V3< float > xyz;
			if( XMLConvert( element, xyz ) )
			{
				vd.WriteVertex( lock, u, positionE, xyz );
			}

			// NORMAL
			unify::V3< float > normal;
			if( element->HasAttributes( "nx" ) )
			{
				normal.x = element->GetFloatAttribute( "nx" );
			}
			if( element->HasAttributes( "ny" ) ) 
			{
				normal.y = element->GetFloatAttribute( "ny" );
			}
			if( element->HasAttributes( "nz" ) ) 
			{
				normal.z = element->GetFloatAttribute( "nz" );
			}
			if( element->HasAttributes( "normal" ) )
			{
				normal = unify::V3< float >( element->GetStringAttribute( "normal" ) );
			}
			if( element->HasAttributes( "nx" ) || element->HasAttributes( "ny" ) || element->HasAttributes( "nz" ) || element->HasAttributes( "normal" ) )
			{
				vd.WriteVertex( lock, index, normalE, normal );
			}

			// TEXTURE 1
			unify::TexCoords uv;
			if( element->HasAttributes( "tu" ) )
			{
				uv.u = element->GetFloatAttribute( "tu" );
			}
			if( element->HasAttributes( "tv" ) )
			{
				uv.v = element->GetFloatAttribute( "tv" );
			}
			if ( element->HasAttributes( "uv" ) )
			{
				uv = unify::TexCoords( element->GetStringAttribute( "uv" ) );
			}
			if( element->HasAttributes( "tu" ) || element->HasAttributes( "tv" ) || element->HasAttributes( "uv" ) ) 
			{
				vd.WriteVertex( lock, index, texE, uv );
			}

			// TEXTURE 2
			unify::TexCoords uv2;
			if( element->HasAttributes( "tu2" ) )
			{
				uv2.u = element->GetFloatAttribute( "tu2" );
			}
			if( element->HasAttributes( "tv2" ) ) 
			{
				uv2.v = element->GetFloatAttribute( "tv2" );
			}
			if ( element->HasAttributes( "uv2" ) )
			{
				uv2 = unify::TexCoords( element->GetStringAttribute( "uv" ) );
			}
			if( element->HasAttributes( "tu2" ) || element->HasAttributes( "tv2" ) || element->HasAttributes( "uv2" ) ) 
			{
				vd.WriteVertex( lock, index, tex2E, uv2 );
			}

			unify::ColorUnit diffuseUnit;
			if( XMLConvert( element, diffuseUnit ) )
			{
				unify::Color diffuse( diffuseUnit );
				vd.WriteVertex( lock, index, diffuseE, diffuse );
			}
		}
		vb.Unlock();
	}

	//////////
	// METHODS...
	//////////

	qxml::ElementList listMethods;
	document.FindElementsByTagNameRecursive( listMethods, "addmethod" );
	for( unsigned int u = 0; u < listMethods.Count(); u++ )
	{
		element = listMethods.Item( u );

		// Determine Primitive Type...
		PrimitiveType::TYPE pt;
        pt = PrimitiveType::StringToPrimitiveType( element->GetStringAttribute( "type" ) );

		// Determine attributes used...
		if( element->HasAttributes( "baseVertex" ) )
		{
			int baseVertex = element->GetIntegerAttribute( "basevertex" );
			int minIndex = element->GetIntegerAttribute( "minindex" );
			int vertices = element->GetIntegerAttribute( "vertices" );
			int startIndex = element->GetIntegerAttribute( "startIndex" );
			int primitives = element->GetIntegerAttribute( "primitives" );
			Effect::shared_ptr effect = managers.GetEffectManager()->Find( element->GetStringAttribute( "effect" ) );
            bool useIB = true;
			set.GetRenderMethodBuffer().AddMethod( RenderMethod( pt, baseVertex, minIndex, vertices, startIndex, primitives, effect, useIB ) );
		}
		else
		{
            int startVertex = element->GetIntegerAttributeElse( "startvertex", 0 );
			int vertices = element->GetIntegerAttribute( "vertices" );
			int primitives = element->GetIntegerAttribute( "primitives" );
			Effect::shared_ptr effect = managers.GetEffectManager()->Find( element->GetStringAttribute( "effect" ) );
            bool useIB = true;
			set.GetRenderMethodBuffer().AddMethod( RenderMethod( pt, startVertex, vertices, primitives, effect, useIB ) );
		}
	}

	document.Destroy();
}

void loader::LoadMesh_XML_V1_1( const unify::Path & filePath, PrimitiveList & primitiveList, Managers managers )
{
	// Destroy exiting primitive list...
	primitiveList.Destroy();

	qxml::Document document( filePath );
	qxml::Element * element;

	//////////////////////////////
	// Frames...
	//////////////////////////////

	qxml::Element * framesElement = document.FindElement( "frames" );
	if( framesElement )
	{
		qxml::ElementList childrenFrames;
		framesElement->FindElementsByTagName( childrenFrames, "frame" );
		for( unsigned int u = 0; u < childrenFrames.Count(); ++u )
		{
			qxml::Element * frameElement = childrenFrames.Item( u );
			std::string name = frameElement->HasAttributes( "name" ) ? frameElement->GetStringAttribute( "name" ) : std::string();
			size_t index = static_cast< size_t >( frameElement->GetIntegerAttribute( "index" ) );
			size_t parent = static_cast< size_t >( frameElement->GetIntegerAttribute( "parent" ) );

			unify::Matrix matrix;
			XMLConvert( frameElement, matrix );

			unify::FrameSet & frameSet = primitiveList.GetFrameSet();
			assert( index == frameSet.Count() );
			frameSet.Add( matrix, parent, name ) ;
		}
	}


	//////////////////////////////
	// Animations...
	//////////////////////////////

	qxml::Element * animationSetElement = document.FindElement( "animationset" );
	if( animationSetElement )
	{
		frameanimation::AnimationSet & animationSet = primitiveList.GetAnimationSet();

		for( qxml::Element * animationItr = animationSetElement->GetFirstChild(); animationItr != 0; animationItr = animationItr->GetNext() )
		{
			if( ! animationItr->IsTagName( "animation" ) )
			{
				continue;
			}

			std::string name = animationItr->GetStringAttribute( "name" );
			unify::Seconds duration = animationItr->GetFloatAttribute( "duration" );

			frameanimation::Animation::shared_ptr animation( new frameanimation::Animation( name, duration ) );

			for( qxml::Element * frameItr = animationItr->GetFirstChild(); frameItr != 0; frameItr = frameItr->GetNext() )
			{
				if( ! frameItr->IsTagName( "frame" ) )
				{
					continue;
				}

				size_t targetBoneIndex;
				if( frameItr->HasAttributes( "targetName" ) )
				{
					std::string targetFrameName = frameItr->GetStringAttribute( "target" );
					targetBoneIndex = primitiveList.GetFrameSet().Find( targetFrameName );
				}
				else if( frameItr->HasAttributes( "targetIndex" ) )
				{
					targetBoneIndex = frameItr->GetAttribute< size_t >( "targetIndex" );
				}
				else
				{
					throw unify::Exception( "animationset's animation requires a target frame specified by either framename or frameindex!" );
				}

				for( qxml::Element * keyItr = frameItr->GetFirstChild(); keyItr != 0; keyItr = keyItr->GetNext() )
				{
					std::string type = keyItr->GetStringAttribute( "type" );
					unify::Seconds time;
					if( keyItr->HasAttributes( "time" ) )
					{
						time = keyItr->GetFloatAttribute( "time" );
					}
					else if( keyItr->HasAttributes( "progress" ) )
					{
						time = keyItr->GetFloatAttribute( "progress" ) * duration;
					}
					else
					{
						throw unify::Exception( "Key missing time or progress attribute!" );
					}

					if( unify::StringIs( type, "scale" ) )
					{
						unify::V3< float > value;
						XMLConvert( keyItr, value );
						animation->AddScaleKey( targetBoneIndex, frameanimation::ScaleKey( time, value ) );
					}
					else if( unify::StringIs( type, "rotation" ) )
					{
						unify::Quaternion value;
						XMLConvert( keyItr, value );
						animation->AddRotationKey( targetBoneIndex, frameanimation::RotationKey( time, value ) );
					}
					else if( unify::StringIs( type, "translation" ) )
					{
						unify::V3< float > value;
						XMLConvert( keyItr, value );
						animation->AddTranslationKey( targetBoneIndex, frameanimation::TranslationKey( time, value ) );
					}
					else
					{
						throw unify::Exception( "Unrecognized animaion key type! (" + type + ")" );
					}
				}

			}

			animationSet.AddAnimation( animation );
		}
	}

	//////////
	// Load Indices...
	//////////

	BufferSet & set = primitiveList.AddBufferSet();

	unsigned int uNumIndices = 0;
	element = document.FindElement( "indexlist" );
	if( element )
	{
		uNumIndices = unify::ListPartCount( element->GetText(), ',' );

		IndexBuffer & ib = set.GetIndexBuffer();
		ib.Create( uNumIndices, IndexFormat::Index16, BufferUsage::Default );
		IndexLock indexLock;
		ib.Lock( indexLock );
		Index16* pIndex = (Index16 *)indexLock.GetData();

		// Load indices...
		for( unsigned int u = 0; u < uNumIndices; u++ )
		{
			pIndex[ u ] = (Index16)unify::Cast< int >( unify::ListPart( element->GetText(), ',', u ) );
		}
		ib.Unlock();
	}

	//////////////////////////////
	// Load effects...
	//////////////////////////////

	qxml::ElementList listEffects;
	document.FindElementsByTagNameRecursive( listEffects, "effect" );
	Effect::shared_ptr firstEffect;
	for( unsigned int uEffect = 0; uEffect < listEffects.Count(); ++uEffect )
	{
		Effect::shared_ptr effect;
		element = listEffects.Item( uEffect );
		std::string name = element->GetStringAttribute( "name" );
		if ( element->HasAttributes( "source" ) )
		{
			unify::Path source( document.GetPath().DirectoryOnly(), element->GetStringAttribute( "source" ) );
			qxml::Document doc( source );
			const qxml::Element * sourceNode = doc.GetRoot();
			effect = loader::EffectLoader( name, sourceNode, managers );
		}
		else 
		{
			effect = loader::EffectLoader( name, element, managers );
		}

		if ( ! firstEffect )
		{
			firstEffect = effect;
		}

		//TODO: NOT A PART OF EFFECT ANY LONGER!
		//qxml::Element* subElement = element->GetFirstChild();
		//while( subElement )
		//{
		//	if( subElement->IsTagName( "frame" ) )
		//	{
		//		const unify::FrameSet & frameSet = primitiveList.GetFrameSet();
		//		size_t frameIndex;
		//		if( subElement->HasAttributes( "frameName" ) )
		//		{
		//			std::string frameName = subElement->GetAttribute( "frameName" )->GetString();
		//			frameIndex = frameSet.Find( frameName );
		//		}
		//		else if( subElement->HasAttributes( "frameIndex" ) )
		//		{
		//			frameIndex = subElement->GetAttribute< size_t >( "frameIndex" );
		//		}
		//		else
		//		{
		//			throw unify::Exception( "addeffect's frame must include either framename or frameindex!" );
		//		}

		//		float influence = subElement->GetAttribute( "influence" )->GetFloat();
		//		effect.AddFrame( frameIndex, influence );
		//	}

		//	subElement = subElement->GetNext();
		//}
	}


	//////////////////////////////
	// Load vertices...
	//////////////////////////////

	element = document.FindElement( "vertexlist" );

	// Vertex declaration
	VertexDeclaration vd;
	if( element->HasElements( "vd" ) )
	{
		// Pull from child element if specified...
		vd = VertexDeclaration( *element->GetElement( "vd" ) );
	}
	else
	{
		// Pull from first effect...
		vd = firstEffect->GetVertexShader()->GetVertexDeclaration();		
	}

	unsigned int uNumVertices = document.CountElements( "vertex" );
	VertexBuffer & vb = set.GetVertexBuffer();
	try
	{
		vb.Create( uNumVertices, vd, BufferUsage::Default );
	}
	catch( ... )
	{
		primitiveList.Destroy();
		throw;
	}

	D3DVERTEXELEMENT9 positionE = {};
	vd.GetElement( "Position", positionE );

	D3DVERTEXELEMENT9 normalE = {};
	vd.GetElement( "Normal", normalE );

	D3DVERTEXELEMENT9 texE = {};
	texE.Usage = D3DDECLUSAGE_TEXCOORD;
	texE.UsageIndex = 0;
	vd.FindElement( texE, texE );

	D3DVERTEXELEMENT9 tex2E = {};
	tex2E.Usage = D3DDECLUSAGE_TEXCOORD;
	tex2E.UsageIndex = 0;
	vd.FindElement( tex2E, tex2E );

	D3DVERTEXELEMENT9 diffuseE = {};
	diffuseE.Usage = D3DDECLUSAGE_COLOR;
	diffuseE.UsageIndex = 0;
	vd.FindElement( diffuseE, diffuseE );

	unify::DataLock lock;
	vb.Lock( lock );

	qxml::ElementList listVertices;
	document.FindElementsByTagNameRecursive( listVertices, "vertex" );

	if ( listVertices.Count() > 0 )
	{
		for( unsigned int u = 0; u < listVertices.Count(); u++ )
		{
			element = listVertices.Item( u );
			unsigned int index = element->GetIntegerAttribute( "index" );

			// XYZ
			unify::V3< float > xyz;
			if( XMLConvert( element, xyz ) )
			{
				vd.WriteVertex( lock, u, positionE, xyz );
			}

			// NORMAL
			unify::V3< float > normal;
			if( element->HasAttributes( "nx" ) )
			{
				normal.x = element->GetFloatAttribute( "nx" );
			}
			if( element->HasAttributes( "ny" ) ) 
			{
				normal.y = element->GetFloatAttribute( "ny" );
			}
			if( element->HasAttributes( "nz" ) ) 
			{
				normal.z = element->GetFloatAttribute( "nz" );
			}
			if( element->HasAttributes( "normal" ) )
			{
				normal = unify::V3< float >( element->GetStringAttribute( "normal" ) );
			}
			if( element->HasAttributes( "nx" ) || element->HasAttributes( "ny" ) || element->HasAttributes( "nz" ) || element->HasAttributes( "normal" ) )
			{
				vd.WriteVertex( lock, index, normalE, normal );
			}

			// TEXTURE 1
			unify::TexCoords uv;
			if( element->HasAttributes( "tu" ) )
			{
				uv.u = element->GetFloatAttribute( "tu" );
			}
			if( element->HasAttributes( "tv" ) )
			{
				uv.v = element->GetFloatAttribute( "tv" );
			}
			if ( element->HasAttributes( "uv" ) )
			{
				uv = unify::TexCoords( element->GetStringAttribute( "uv" ) );
			}
			if( element->HasAttributes( "tu" ) || element->HasAttributes( "tv" ) || element->HasAttributes( "uv" ) ) 
			{
				vd.WriteVertex( lock, index, texE, uv );
			}

			// TEXTURE 2
			unify::TexCoords uv2;
			if( element->HasAttributes( "tu2" ) )
			{
				uv2.u = element->GetFloatAttribute( "tu2" );
			}
			if( element->HasAttributes( "tv2" ) ) 
			{
				uv2.v = element->GetFloatAttribute( "tv2" );
			}
			if ( element->HasAttributes( "uv2" ) )
			{
				uv2 = unify::TexCoords( element->GetStringAttribute( "uv" ) );
			}
			if( element->HasAttributes( "tu2" ) || element->HasAttributes( "tv2" ) || element->HasAttributes( "uv2" ) ) 
			{
				vd.WriteVertex( lock, index, tex2E, uv2 );
			}

			unify::ColorUnit diffuseUnit;
			if( XMLConvert( element, diffuseUnit ) )
			{
				unify::Color diffuse( diffuseUnit );
				vd.WriteVertex( lock, index, diffuseE, diffuse );
			}
		}
		vb.Unlock();
	}

	//////////
	// METHODS...
	//////////

	qxml::ElementList listMethods;
	document.FindElementsByTagNameRecursive( listMethods, "addmethod" );
	for( unsigned int u = 0; u < listMethods.Count(); u++ )
	{
		element = listMethods.Item( u );

		// Determine Primitive Type...
		PrimitiveType::TYPE pt;
        pt = PrimitiveType::StringToPrimitiveType( element->GetStringAttribute( "type" ) );

		// Determine attributes used...
		if( element->HasAttributes( "baseVertex" ) )
		{
			int baseVertex = element->GetIntegerAttribute( "basevertex" );
			int minIndex = element->GetIntegerAttribute( "minindex" );
			int vertices = element->GetIntegerAttribute( "vertices" );
			int startIndex = element->GetIntegerAttribute( "startIndex" );
			int primitives = element->GetIntegerAttribute( "primitives" );
			Effect::shared_ptr effect = managers.GetEffectManager()->Find( element->GetStringAttribute( "effect" ) );
            bool useIB = true;
			set.GetRenderMethodBuffer().AddMethod( RenderMethod( pt, baseVertex, minIndex, vertices, startIndex, primitives, effect, useIB ) );
		}
		else
		{
            int startVertex = element->GetIntegerAttributeElse( "startvertex", 0 );
			int vertices = element->GetIntegerAttribute( "vertices" );
			int primitives = element->GetIntegerAttribute( "primitives" );
			Effect::shared_ptr effect = managers.GetEffectManager()->Find( element->GetStringAttribute( "effect" ) );
            bool useIB = true;
			set.GetRenderMethodBuffer().AddMethod( RenderMethod( pt, startVertex, vertices, primitives, effect, useIB ) );
		}
	}

	document.Destroy();
}

