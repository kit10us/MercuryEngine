package cardice;

import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLUtils;

public class Texture {
	private Core m_core;
	Bitmap m_bitmap;
	private int[] m_textureId;
	float m_aspectRatio;
	
	public Texture( Core core, int id )
	{
		m_core = core;
		Context context = m_core.getContext();
		GL10 gl = m_core.getGL();
    	m_bitmap = BitmapFactory.decodeResource( context.getResources(), id );
    	m_aspectRatio = getWidth() / getHeight();
    	m_textureId = new int[1];
    	gl.glGenTextures( 1, m_textureId, 0 );
    	gl.glBindTexture( GL10.GL_TEXTURE_2D, m_textureId[ 0 ] );
    	gl.glTexParameterf( GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR );
    	gl.glTexParameterf( GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR );
    	GLUtils.texImage2D( GL10.GL_TEXTURE_2D, 0, m_bitmap, 0 );
	}
	
	public float getWidth()
	{
		return (float)m_bitmap.getWidth();
	}
	
	public float getHeight()
	{
		return (float)m_bitmap.getHeight();
	}
	
	public float getAspectRatio()
	{
		return m_aspectRatio;
	}
	
	public void use()
	{
		GL10 gl = m_core.getGL();
		gl.glEnable( GL10.GL_TEXTURE_2D );
		gl.glBindTexture( GL10.GL_TEXTURE_2D, m_textureId[ 0 ] );
	}
	
	public void unuse()
	{
		GL10 gl = m_core.getGL();
		gl.glDisable( GL10.GL_TEXTURE_2D );
	}
}
