package cardice;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import javax.microedition.khronos.opengles.GL10;

import android.util.Log;

public class Object3D {
	private Core m_core;
	
	// Tree structure
	private Object3D m_parent;
	private List< Object3D > m_children = new LinkedList< Object3D >();
	private Map< String, Object3D > m_childrenMap = new HashMap< String, Object3D >();
 	
	// Personal data
	private Frame3D m_frame = new Frame3D();
	private Graphic3D m_graphic;
	
	public Object3D( Core core )
	{
		m_core = core;
	}
	
	public void addObject( Object3D object )
	{
		m_children.add( object );
	}
	
	public void addObject( String name, Object3D object )
	{
		addObject( object );
		m_childrenMap.put( name, object );
	}
	
	public Object3D findObject( String name )
	{
		return m_childrenMap.get( name );
	}
	
	public Frame3D getFrame()
	{
		return m_frame;
	}
	
	public void setGraphic( Graphic3D graphic )
	{
		m_graphic = graphic;
	}
	
	public void render()
	{
		GL10 gl = m_core.getGL();
		
		// Our frame
		gl.glMatrixMode( GL10.GL_MODELVIEW );
		gl.glPushMatrix();
		gl.glLoadMatrixf( m_frame.getLocalMatrix(), 0 );

		if ( m_graphic != null )
		{
			m_graphic.render();
		}
		
		for ( Object3D child : m_children )
		{
			child.render();
		}
		
		gl.glMatrixMode( GL10.GL_MODELVIEW );
		gl.glPopMatrix();
	}
}
