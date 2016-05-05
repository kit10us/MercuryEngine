package cardice;

import java.util.Map;

public class TextureManager {
	private Map< String, Texture > m_textures;
	
	public void AddTexture( String name, Texture texture )
	{
		m_textures.put( name, texture );
	}
	
	public Texture FindTexture( String name )
	{
		return m_textures.get( name );
	}
}
