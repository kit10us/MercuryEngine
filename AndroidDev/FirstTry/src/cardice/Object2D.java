package cardice;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import javax.microedition.khronos.opengles.GL10;

import android.opengl.Matrix;

public class Object2D {
	private Core m_core;

	// Tree structure
	private Object2D m_parent;
	private List< Object2D > m_children = new LinkedList< Object2D >();
	private Map< String, Object2D > m_childrenMap = new HashMap< String, Object2D >();
 	
	// Personal data
	private Frame2D m_frame = new Frame2D();
	private Graphic2D m_graphic;
	
	public Object2D( Core core )
	{
		m_core = core;
	}
	
	public void addObject( Object2D object )
	{
		m_children.add( object );
	}
	
	public void addObject( String name, Object2D object )
	{
		addObject( object );
		m_childrenMap.put( name, object );
	}
	
	public Object2D findObject( String name )
	{
		return m_childrenMap.get( name );
	}
	
	public Frame2D getFrame()
	{
		return m_frame;
	}
	
	public void setGraphic( Graphic2D graphic )
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
		
		for ( Object2D child : m_children )
		{
			child.render();
		}
		
		gl.glMatrixMode( GL10.GL_MODELVIEW );
		gl.glPopMatrix();
	}
}
