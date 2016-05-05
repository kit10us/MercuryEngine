package com.qss.first_try;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.content.Context;

class BeginMoveAction implements qs.actions.IAction {
	private qs.cardice.Scene3D m_scene3D;
	private boolean m_toDeck2 = true;
	
	public BeginMoveAction( qs.cardice.Scene3D scene3D )
	{
		m_scene3D = scene3D;
	}
	
	public qs.actions.IAction update( float elapsedInSeconds )
	{
		qs.cardice.Object3D deck1 = (qs.cardice.Object3D)m_scene3D.findObject( "deck" );
		qs.cardice.Object3D deck2 = (qs.cardice.Object3D)m_scene3D.findObject( "deck2" );
		
		// Check if we have to change direction.
		if ( m_toDeck2 )
		{
			if ( ((qs.cardice.dominion.DeckGraphic)deck1.getGraphic()).getDeck().size() == 0 )
			{
				m_toDeck2 = false;
			}
		}
		else
		{
			if ( ((qs.cardice.dominion.DeckGraphic)deck2.getGraphic()).getDeck().size() == 0 )
			{
				m_toDeck2 = true;
			}
		}

		if ( m_toDeck2 )
		{
			return new MoveCardAction( this, m_scene3D, deck1, deck2, 1.0f );
		}
		else
		{
			return new MoveCardAction( this, m_scene3D, deck2, deck1, 1.0f );
		}
	}
}


// Move the top card from one deck to another.
class MoveCardAction implements qs.actions.IAction {
	private qs.actions.IAction m_doneAction;
	private float m_duration;
	private float m_elapsed = 0;
		
	private qs.cardice.Scene3D m_scene;
	private qs.cardice.Object3D m_sourceObject;
	private qs.cardice.Object3D m_destinationObject;
	
	private qs.cardice.rules.Deck m_sourceDeck;
	private qs.cardice.rules.Deck m_destinationDeck;
	
	private qs.cardice.rules.Deck m_tempDeck;
	private qs.cardice.dominion.DeckGraphic m_tempGraphic;
	private qs.cardice.Object3D m_tempObject;
	
	public MoveCardAction( qs.actions.IAction doneAction, qs.cardice.Scene3D scene, qs.cardice.Object3D sourceObject, qs.cardice.Object3D destinationObject, float duration ) 
	{
		m_doneAction = doneAction;
		m_duration = duration;
		m_scene = scene;
		m_sourceObject = sourceObject;
		m_destinationObject = destinationObject;
		
		qs.cardice.dominion.DeckGraphic deckGraphicSource = (qs.cardice.dominion.DeckGraphic)m_sourceObject.getGraphic();		
		m_sourceDeck = (qs.cardice.rules.Deck)deckGraphicSource.getDeck();
		
		qs.cardice.dominion.DeckGraphic deckGraphicDestination = (qs.cardice.dominion.DeckGraphic)m_destinationObject.getGraphic();		
		m_destinationDeck = (qs.cardice.rules.Deck)deckGraphicDestination.getDeck();
		
		if ( m_sourceDeck.size() > 0 )
		{
			m_tempDeck = new qs.cardice.rules.Deck( m_sourceDeck.getSideTexture() );
			m_tempDeck.addCardTop( m_sourceDeck.takeTopCard() );
			m_tempDeck.setFaceDown( m_sourceDeck.getFaceDown() );
			
			m_tempGraphic = new qs.cardice.dominion.DeckGraphic( m_scene.getCore(), m_tempDeck );

			m_tempObject = new qs.cardice.Object3D( m_scene.getCore() );
			m_tempObject.setGraphic( m_tempGraphic );
			m_tempObject.getFrame().translate( m_sourceObject.getPositionAbove() );
			m_sourceObject.getParent().addObject( m_tempObject );
		}
	}
	
	private float[] getSourcePosition()
	{
		return m_sourceObject.getPositionAbove();
	}
	
	private float[] getDestinationPosition()
	{
		return m_destinationObject.getPositionAbove();
	}
	
	public qs.actions.IAction update( float elapsedInSeconds ) 
	{
		m_elapsed += elapsedInSeconds;
		float progress = m_elapsed / m_duration;
		if ( m_elapsed >= m_duration )
		{
			float over = m_elapsed - m_duration;
			m_elapsed = m_duration;
			m_destinationDeck.addCardTop( m_tempDeck.takeTopCard() );
			m_scene.removeObject( m_tempObject, true );
			m_tempObject = null;
			return m_doneAction;
		}
		else
		{
			m_tempObject.getFrame().copyFrom( m_sourceObject.getFrame() );
			if ( m_sourceDeck.getFaceDown() != m_destinationDeck.getFaceDown() )
			{
				m_tempObject.getFrame().rotate( qs.cardice.V3.make( 0, 0, 1.0f ), 180.0f * progress );
			}
			
			float position[] = new float[ 3 ];
			float midPoint[] = new float[ 3 ];
			qs.cardice.V3.lerp( midPoint, 0.5f, getSourcePosition(), getDestinationPosition() );
			midPoint[ 1 ] += m_tempGraphic.getCardWidth();
			qs.cardice.V3.berp( position, progress, getSourcePosition(), midPoint, getDestinationPosition() );
			m_tempObject.getFrame().setPosition( position );
		}
		
		// If not done, return this.
		return this;
	}
}



public class MyRenderer implements android.opengl.GLSurfaceView.Renderer {
	private Context m_context;
	private qs.cardice.Core m_core;
	private long m_lastTimeNS = System.nanoTime();

	private qs.cardice.rules.Store m_store = new qs.cardice.rules.Store();
	private qs.cardice.rules.CardStock m_cardStock;
	private qs.actions.IAction m_action;
	
	public MyRenderer( Context context )
	{
		m_context = context;
	}
	
	public void onSurfaceCreated( GL10 gl, EGLConfig config )
	{
    	gl.glClearColor( 0.0f, 0.0f, 0.3f, 0.5f );
    	gl.glShadeModel( GL10.GL_SMOOTH );
    	gl.glClearDepthf( 1.0f );
    	gl.glEnable( GL10.GL_DEPTH_TEST );
    	gl.glDepthFunc( GL10.GL_LEQUAL );
    	gl.glHint( GL10.GL_PERSPECTIVE_CORRECTION_HINT, GL10.GL_NICEST );
    	
    	m_core = new qs.cardice.Core( m_context, gl );

    	qs.cardice.TextureManager textures = m_core.getTextureManager();

    	// Load textures
    	textures.addTexture( "company", new qs.cardice.Texture( m_core, R.drawable.cardice1280x800 ) );
    	textures.addTexture( "back1", new qs.cardice.Texture( m_core, R.drawable.back1 ) );
    	textures.addTexture( "side", new qs.cardice.Texture( m_core, R.drawable.cardsidebig ) );
    	textures.addTexture( "adventurer", new qs.cardice.Texture( m_core, R.drawable.adventurer ) );
    	textures.addTexture( "copper", new qs.cardice.Texture( m_core, R.drawable.copper ) );
    	textures.addTexture( "silver", new qs.cardice.Texture( m_core, R.drawable.silver ) );
    	textures.addTexture( "gold", new qs.cardice.Texture( m_core, R.drawable.gold ) );
    	
    	m_cardStock = new qs.cardice.rules.CardStock();
    	m_cardStock.addCard( "gold", new qs.cardice.rules.Card( textures.findTexture( "gold" ), textures.findTexture( "back1" ) ) );
    	m_cardStock.addCard( "silver", new qs.cardice.rules.Card( textures.findTexture( "silver" ), textures.findTexture( "back1" ) ) );
    	m_cardStock.addCard( "copper", new qs.cardice.rules.Card( textures.findTexture( "copper" ), textures.findTexture( "back1" ) ) );
    	m_cardStock.addCard( "adventurer", new qs.cardice.rules.Card( textures.findTexture( "adventurer" ), textures.findTexture( "back1" ) ) );
    	
    	
    	// Rules
    	{
    		qs.cardice.rules.Deck deck = new qs.cardice.rules.Deck( textures.findTexture( "side" ) );
			deck.addCardTop( m_cardStock.getCard( "copper" ) );
			deck.addCardTop( m_cardStock.getCard( "adventurer" ) );
			deck.addCardTop( m_cardStock.getCard( "copper" ) );
			deck.setFaceDown( true );
			m_store.addDeck( "deck1", deck );
		}

		{
			qs.cardice.rules.Deck deck2 = new qs.cardice.rules.Deck( textures.findTexture( "side" ) );
			deck2.addCardTop( m_cardStock.getCard( "copper" ) );
			deck2.addCardTop( m_cardStock.getCard( "silver" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "gold" ) );
			deck2.addCardTop( m_cardStock.getCard( "adventurer" ) );
			m_store.addDeck( "deck2", deck2 );
		}
		
		{
			qs.cardice.rules.Deck deck3 = new qs.cardice.rules.Deck( textures.findTexture( "side" ) );
			deck3.addCardTop( m_cardStock.getCard( "silver" ) );
			m_store.addDeck( "deck3", deck3 );
		}
		
		{
			qs.cardice.rules.Deck hand = new qs.cardice.rules.Deck( textures.findTexture( "side") );
			hand.addCardBottom( m_cardStock.getCard( "adventurer" ) );
			hand.addCardTop( m_cardStock.getCard( "gold" ) );
			hand.addCardTop( m_cardStock.getCard( "silver" ) );
			m_store.addDeck( "hand", hand );
		}

		
		// Graphics
		
		qs.cardice.Scene3D scene3D = new qs.cardice.Scene3D( m_core );
    	scene3D.getCamera().getFrame().translate( 0, 100, 100 );
    	scene3D.getCamera().getFrame().lookAt( qs.cardice.V3.make( 0, 0, 0 ) );
    	m_core.getSceneManager().addScene( "game", scene3D );

    	qs.cardice.Object3D deckGroup = new qs.cardice.Object3D( m_core );
		scene3D.addObject( "deckgroup", deckGroup );		
		
		{
			qs.cardice.dominion.DeckGraphic deckGraphic = new qs.cardice.dominion.DeckGraphic( m_core, m_store.getDeck("deck1") );
			qs.cardice.Object3D object = new qs.cardice.Object3D( m_core );
			object.setGraphic( deckGraphic );
			object.getFrame().translate( -20, 0, 0 );
			deckGroup.addObject( "deck", object );
		}		

		{
			qs.cardice.dominion.DeckGraphic deckGraphic = new qs.cardice.dominion.DeckGraphic( m_core, m_store.getDeck("deck2") );
			qs.cardice.Object3D object = new qs.cardice.Object3D( m_core );
			object.setGraphic( deckGraphic );
			object.getFrame().translate( 20, 0, 0 );
			deckGroup.addObject( "deck2", object );
		}
		
		{
			qs.cardice.dominion.DeckGraphic deckGraphic = new qs.cardice.dominion.DeckGraphic( m_core, m_store.getDeck("deck3") );
			qs.cardice.Object3D object = new qs.cardice.Object3D( m_core );
			object.setGraphic( deckGraphic );
			object.getFrame().translate( 60, 0, 0 );
			deckGroup.addObject( "deck3", object );
		}
		
		{
			qs.cardice.dominion.DeckFanView handGraphic = new qs.cardice.dominion.DeckFanView( m_core, m_store.getDeck( "hand" ) );
			qs.cardice.Object3D object = new qs.cardice.Object3D( m_core );
			object.setGraphic( handGraphic );
			object.getFrame().translate( 0, 30, 30 );
			//object.getFrame().rotate( 180.0f, 0, 1, 0 );
			//object.getFrame().rotate( -45.0f, 1, 0, 0 ); 
			scene3D.addObject( "hand", object );
			object.setEnabled( true );
		}
		
		m_action = new BeginMoveAction( scene3D );
	}
	
	public void onDrawFrame( GL10 gl )
	{
		gl.glClear( GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT );
	
		long currentTimeNS = System.nanoTime();
		float elapsed = (float)( ( currentTimeNS - m_lastTimeNS ) * 0.000000001 );
		m_lastTimeNS = currentTimeNS;
		
		if ( m_action != null )
		{
			m_action = m_action.update( elapsed );
		}
		
		qs.cardice.Scene3D scene3D = (qs.cardice.Scene3D)m_core.getSceneManager().findScene( "game" );
		qs.cardice.Object3D deckGroup = scene3D.findObject( "deckgroup" );
		deckGroup.getFrame().rotate( qs.cardice.V3.make( 0, 1.0f, 0 ), elapsed * 30.0f );
				
		m_core.getSceneManager().update( elapsed );
		m_core.getSceneManager().render();
	}
	
	public void onSurfaceChanged( GL10 gl, int width, int height )
	{
		gl.glViewport( 0, 0, width, height );
		m_core.getSceneManager().changed( width, height );
	}
}
