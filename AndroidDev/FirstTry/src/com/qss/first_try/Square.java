package com.qss.first_try;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import javax.microedition.khronos.opengles.GL10;

public class Square implements qs.cardice.Graphic3D {
	private qs.cardice.Core m_core;
	
	// Graphical
	private FloatBuffer m_vertexBuffer;
	private ShortBuffer m_indexBuffer;
	private FloatBuffer m_textureCoordinatesBuffer;
	private qs.cardice.Texture m_texture;
	
	private qs.cardice.BBox m_bbox = new qs.cardice.BBox();
	
	public Square( qs.cardice.Core core, float width, float height )
	{
		m_core = core;
		
		float vertices[] =
			{
				-width, height, 0,
				width, height, 0,
				-width, -height, 0,
				width, -height, 0
			};
		
		m_bbox.reset();
		m_bbox.addPoint( -width, -height, 0 );
		m_bbox.addPoint( width, height, 0 );
			
		short indices[] = 
			{0, 1, 2, 1, 3, 2 };

		float textureCoordinates[] = 
			{   0.0f, 0.0f,
				1.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 1.0f
			};

		ByteBuffer vbb = ByteBuffer.allocateDirect( vertices.length * 4 );
		vbb.order( ByteOrder.nativeOrder() );
		m_vertexBuffer = vbb.asFloatBuffer();
		m_vertexBuffer.put( vertices );
		m_vertexBuffer.position( 0 );
		
		ByteBuffer ibb = ByteBuffer.allocateDirect( indices.length * 2 );
		ibb.order( ByteOrder.nativeOrder() );
		m_indexBuffer = ibb.asShortBuffer();
		m_indexBuffer.put( indices );
		m_indexBuffer.position( 0 );
		
		ByteBuffer tcbb = ByteBuffer.allocateDirect( textureCoordinates.length * 4 );
		tcbb.order( ByteOrder.nativeOrder() );
		m_textureCoordinatesBuffer = tcbb.asFloatBuffer();
		m_textureCoordinatesBuffer.put( textureCoordinates );
		m_textureCoordinatesBuffer.position( 0 );
	}
	
	public qs.cardice.BBox getBBox()
	{
		return m_bbox;
	}
	
	public void setTexture( qs.cardice.Texture texture )
	{
		m_texture = texture;
	}
	
	public void render()
	{
		GL10 gl = m_core.getGL();
				
		// General states
		gl.glFrontFace( GL10.GL_CCW );
		gl.glDisable( GL10.GL_CULL_FACE );
		gl.glCullFace( GL10.GL_BACK );
		
		// Texture
		if ( m_texture != null )
		{
			m_texture.use();
			gl.glEnableClientState( GL10.GL_TEXTURE_COORD_ARRAY );
			gl.glTexCoordPointer( 2 , GL10.GL_FLOAT, 0, m_textureCoordinatesBuffer );
		}
	
		gl.glEnableClientState( GL10.GL_VERTEX_ARRAY );
		gl.glVertexPointer( 3, GL10.GL_FLOAT, 0, m_vertexBuffer );
		gl.glDrawElements( GL10.GL_TRIANGLES, 6, GL10.GL_UNSIGNED_SHORT, m_indexBuffer );
		gl.glDisableClientState( GL10.GL_VERTEX_ARRAY );
		
		// Texture
		if ( m_texture != null )
		{
			m_texture.unuse();
			gl.glDisableClientState( GL10.GL_TEXTURE_COORD_ARRAY );
		}

		// General states
		gl.glDisable( GL10.GL_CULL_FACE );
	}
}
