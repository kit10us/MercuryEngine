package cardice;

import android.R.bool;
import android.opengl.Matrix;

public class Frame2D {
	private float m_x = 0;
	private float m_y = 0;
	private float m_z = 0;
	private float m_scaleX = 1;
	private float m_scaleY = 1;
	private float m_rotationZ = 0;
	private boolean m_dirty = true;
	
	private float[] m_localMatrix = new float[ 4 * 4 ];
	
	public Frame2D()
	{
		syncMatrix();
	}
	
	public float[] getLocalMatrix()
	{
		if ( m_dirty )
		{
			syncMatrix();
		}
		
		return m_localMatrix;
	}
	
	public float getX()
	{
		return m_x;
	}
	
	public float getY()
	{
		return m_y;
	}
	
	public float getZ()
	{
		return m_z;
	}
	
	public void setX( float x )
	{
		m_x = x;
		m_dirty = true;
	}
	
	public void setY( float y )
	{
		m_y = y;
		m_dirty = true;
	}

	public void setZ( float z )
	{
		m_z = z;
		m_dirty = true;
	}
	
	public void setPosition( float x, float y, float z )
	{
		m_x = x;
		m_y = y;
		m_x = z;
		m_dirty = true;
	}

	public void translate( float x, float y )
	{
		m_x += x;
		m_y += y;
		m_dirty = true;
	}
	
	public float getScaleX()
	{
		return m_scaleX;
	}
	
	public float getScaleY()
	{
		return m_scaleY;
	}
	
	public void setScaleX( float scaleX )
	{
		m_scaleX = scaleX;
		m_dirty = true;
	}
	
	public void setScaleY( float scaleY )
	{
		m_scaleY = scaleY;
		m_dirty = true;
	}
	
	public void setScale( float scaleX, float scaleY )
	{
		m_scaleX = scaleX;
		m_scaleY = scaleY;
		m_dirty = true;
	}
	
	public void scale( float scaleX, float scaleY )
	{
		m_scaleX += scaleX;
		m_scaleY += scaleY;
		m_dirty = true;
	}
	
	public float getRotation()
	{
		return m_rotationZ;
	}
	
	public void setRotation( float rotation )
	{
		m_rotationZ = rotation;
		m_dirty = true;
	}
	
	public void rotate( float rotation )
	{
		m_rotationZ += rotation;
		m_dirty = true;
	}

	public void syncMatrix()
	{
		if ( m_dirty )
		{
			Matrix.setIdentityM( m_localMatrix, 0 );
			Matrix.translateM( m_localMatrix, 0, m_x, m_y, m_z );
			Matrix.scaleM( m_localMatrix, 0, m_scaleX, m_scaleY, 1 );
			Matrix.rotateM( m_localMatrix, 0, ( m_rotationZ / 3.1415926f * 2.0f ) * 360.0f, 0, 0, 1 );
			m_dirty = false;
		}
	}
}
