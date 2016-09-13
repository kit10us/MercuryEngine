
template< typename T >
ResourceManagerBase< T >::ResourceManagerBase( std::string resourceName )
	: m_resourceName( resourceName )
{
}

template< typename T >
ResourceManagerBase< T >::~ResourceManagerBase()
{
}

template< typename T >
std::string ResourceManagerBase< T >::GetName() const
{
	return m_resourceName;
}

template< typename T >
void ResourceManagerBase< T >::AddResource( std::string name, T * resource )
{
	if( Exists( id ) )
	{
		throw std::exception( std::string( m_resourceName + " resource manager: Attempted to add resource \"" + id + "\", when ID is already in use by a different resource!" ).c_str() );
	}

	std::shared_ptr< ResourcePack< T > > pack( std::make_shared< ResourcePack< T > >( resource ) );

	m_resources[ id ] = pack;
}

template< typename T >
bool ResourceManagerBase< T >::Exists( std::string name ) const
{
	auto itr = m_resources.find( id );
	return !(itr == m_resources.end());
}

template< typename T >
std::shared_ptr< ResourcePack< T > > ResourceManagerBase< T >::Find( std::string name )
{
	return m_resources[ id ];
}

template< typename T >
std::shared_ptr< const ResourcePack< T > > ResourceManagerBase< T >::Find( std::string name ) const
{
	return m_resources[id];
}

template< class T >
size_t ResourceManagerBase< T >::Count() const
{
	return m_resourceList.size();
}
