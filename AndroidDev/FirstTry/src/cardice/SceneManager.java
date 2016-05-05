package cardice;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class SceneManager {
	private List< Scene > m_scenes = new LinkedList< Scene >();
	private Map< String, Scene > m_sceneMap = new HashMap< String, Scene >();
	
	public void addScene( String name, Scene scene )
	{
		m_scenes.add( scene );
		m_sceneMap.put( name, scene );
	}
	
	public Scene FindScene( String name )
	{
		return m_sceneMap.get( name );
	}
	
	public void render()
	{
		for ( Scene scene : m_scenes )
		{
			scene.render();
		}
	}
	
	public void changed( int width, int height )
	{
		for ( Scene scene : m_scenes )
		{
			scene.changed( width, height );
		}
	}
}
