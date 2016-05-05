package cardice;

import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLU;
import android.opengl.Matrix;
import cardice.Scene;

public class Scene2D implements Scene {
	private Core m_core;
	private int m_width;
	private int m_height;
	private float[] m_projectionMatrix;
	private Object2D m_rootObject;
	
	public Scene2D( Core core )
	{
		m_core = core;
		m_rootObject = new Object2D( core );
	}
	
	public void addObject( Object2D object )
	{
		m_rootObject.addObject( object );
	}

	public void changed( int width, int height )
	{
		m_width = width;
		m_height = height;
		m_projectionMatrix = new float[ 4 * 4 ];
		Matrix.orthoM( m_projectionMatrix, 0, 0, m_width, m_height, 0, -1, 1 );
	}
	
	public void render() 
	{
		assert( m_projectionMatrix != null ) : "Projection matrix not set! render called before changed!";
		
		GL10 gl = m_core.getGL();

		// Prepare for rendering
				
		gl.glMatrixMode( GL10.GL_PROJECTION );
		gl.glPushMatrix();
		gl.glLoadMatrixf( m_projectionMatrix, 0 );

		gl.glMatrixMode( GL10.GL_MODELVIEW );
		gl.glPushMatrix();
		gl.glLoadIdentity();
		
		// Render
		m_rootObject.render();

		// Reset from rendering
		
		gl.glMatrixMode( GL10.GL_MODELVIEW );
		gl.glPopMatrix();

		gl.glMatrixMode( GL10.GL_PROJECTION );
		gl.glPopMatrix();
	}
}
