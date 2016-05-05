package cardice;

import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLU;
import android.opengl.Matrix;
import cardice.Scene;

public class Scene3D implements Scene {
	private Core m_core;
	private int m_width;
	private int m_height;
	private float[] m_projectionMatrix;
	private Object3D m_rootObject;
	
	public Scene3D( Core core )
	{
		m_core = core;
		m_rootObject = new Object3D( core );
	}
	
	public void addObject( Object3D object )
	{
		m_rootObject.addObject( object );
	}

	public void changed(int width, int height) 
	{		
		m_width = width;
		m_height = height;
		m_projectionMatrix = new float[ 4 * 4 ];
		
		float TWOPI_OVER_360 = 3.1415926f * 2 / 360;
		float fovy = 45;
		float near = 0.1f;
		float far = 100.f;
		float aspect = (float)width / (float)height;
	    float half_height = near * (float)Math.tan( fovy * 0.5 * TWOPI_OVER_360 );
	    float half_width = half_height * aspect;

	    Matrix.frustumM( m_projectionMatrix, 0, -half_width, half_width, -half_height, half_height, near, far );
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
