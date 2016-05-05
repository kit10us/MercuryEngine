#include <dxi/dae/library_images/DAE_Image.h>

using namespace dxi;
using namespace dae;

dae::Image::Image( IDocument & document, const qxml::Element * node )
: DocumentNode( document, node )
, m_id( node->GetStringAttributeElse( "id", std::string() ) )
, m_sid( node->GetStringAttributeElse( "sid", std::string() ) )
, m_name( node->GetStringAttributeElse( "name", std::string() ) )
{
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "init_from" ) )
		{
			unify::Path source = unify::Path( childNode->GetDocument()->GetPath().DirectoryOnly(), childNode->GetText() ).Normalize();
			m_texture.reset( new Texture( source ) );
		}
	}
}

const std::string & dae::Image::GetID() const
{
	return m_id;
}

const std::string & dae::Image::GetSID() const
{
	return m_sid;
}

const std::string & dae::Image::GetName() const
{
	return m_name;
}

Texture::shared_ptr dae::Image::GetTexture() const
{
	return m_texture;
}