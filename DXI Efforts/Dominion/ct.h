#pragma once

#include <unify/Parameters.h>


namespace ct // cardice test
{
    // notes:
    // Cards are drawn from deck to deck, and between areas and decks.
    // This is accomplished by removing the card from the deck or area, then inserting into a deck or area.

    class Player;

    // An instance of a card.
    class Card
    {
    public:
        Card( unify::Size< float > size, dxi::Texture::shared_ptr textureFront, dxi::Texture::shared_ptr textureBack )
            : m_size( size )
            , m_textureFront( textureFront )
            , m_textureBack( textureBack )
        {
        }

        unify::Size< float > Size() const
        {
            return m_size;
        }

        dxi::Texture::shared_ptr TextureFront() const
        {
            return m_textureFront;
        }

        dxi::Texture::shared_ptr TextureBack() const
        {
            return m_textureBack;
        }

        ~Card()
        {
        }

    private:
        unify::Size< float > m_size;
        dxi::Texture::shared_ptr m_textureFront;
        dxi::Texture::shared_ptr m_textureBack;
    };

    // A set of cards.
    class CardSet
    {
    public:
        CardSet()
        {
        }

        void InsertTop( Card card )
        {
            m_cards.push_back( card );
        }

        void InsertBottom( Card card )
        {
            m_cards.push_front( card );
        }

        void Insert( Card card, size_t destination )
        {
            size_t place = 0;
            std::list< Card >::iterator itr = m_cards.begin();
            while ( place != destination )
            {
                ++itr;
            }
            m_cards.insert( itr, card );
        }

        Card RemoveTop()
        {
            Card card = *m_cards.rend();
            m_cards.pop_back();
            return card;
        }

        Card RemoveBottom()
        {
            Card card = *m_cards.begin();
            m_cards.pop_front();
            return card;        
        }

        Card Remove( size_t from )
        {
            size_t place = 0;
            std::list< Card >::iterator itr = m_cards.begin();
            while ( place != from )
            {
                ++itr;
            }
            Card card = *itr;
            m_cards.erase( itr );
            return card;
        }

        const Card & TopCard() const
        {
            return *m_cards.rbegin();
        }

        const Card & BottomCard() const
        {
            return *m_cards.begin();
        }

        const Card & PeekCard( size_t index ) const
        {
            size_t place = 0;
            std::list< Card >::const_iterator itr = m_cards.begin();
            for ( ; place != index; ++place, ++itr )
            {
            }
            return *itr;
        }

        size_t Size() const
        {
            return m_cards.size();
        }

    private:
        std::list< Card > m_cards;
    };

    struct PlaceType
    {
        enum TYPE
        {
            Deck,
            Hand
        };
    };

    struct Place
    {
        unify::Matrix matrix;
        CardSet cards;
        PlaceType::TYPE type;
        float width;

        static Place PlaceDeck( unify::Matrix & matrix )
        {
            Place place;
            place.matrix = matrix;
            place.type = PlaceType::Deck;
            place.width = 0;
            return place;
        }

        static Place PlaceHand( unify::Matrix & matrix, float width )
        {
            Place place;
            place.matrix = matrix;
            place.type = PlaceType::Hand;
            place.width = width;
            return place;
        }
    };

    // An area is a grouping of physical game pieces.
    class Area
    {
    public:
        Area()
        {
        }

        void AddPlace( std::string name, Place & place )
        {
            m_placeVector.push_back( place );
            m_placeMap[ name ] = m_placeVector.size() - 1;
        }

        Place & FindPlace( const std::string name )
        {
            return m_placeVector[ m_placeMap[ name ] ];
        }

        size_t NumberOfPlaces() const
        {
            return m_placeVector.size();
        }

        const Place & GetPlace( size_t index ) const
        {
            return m_placeVector[ index ];
        }

    private:
        std::vector< Place > m_placeVector;
        std::map< std::string, size_t > m_placeMap;
    };

    class Player
    {
    public:
        Player()
        {
        }

        Area & GetArea()
        {
            return m_area;
        }

    private:
        Area m_area;

    };

    void Render( const Card & card, unify::Matrix matrix )
    {
        dxi::core::Win32Game * game = dxi::core::Win32Game::GetGameInstance();
        unify::Size< float > size = card.Size();
        struct CardVertex {
            float x, y, z;
            float u, v;
        };
        
         CardVertex cardBackVB[] =
        {
            { size.width * -0.5f, 0.0f, size.height *  0.5f, 0.0f, 0.0f },
            { size.width *  0.5f, 0.0f, size.height *  0.5f, 1.0f, 0.0f },
            { size.width * -0.5f, 0.0f, size.height * -0.5f, 0.0f, 1.0f },
            { size.width *  0.5f, 0.0f, size.height * -0.5f, 1.0f, 1.0f },
        };

        CardVertex cardFrontVB[] =
        {
            { size.width *  0.5f, 0.0f, size.height *  0.5f, 0.0f, 0.0f },
            { size.width * -0.5f, 0.0f, size.height *  0.5f, 1.0f, 0.0f },
            { size.width *  0.5f, 0.0f, size.height * -0.5f, 0.0f, 1.0f },
            { size.width * -0.5f, 0.0f, size.height * -0.5f, 1.0f, 1.0f },
        };

        dxi::Transform::Set( dxi::Transform::Index::World, matrix );
        game->GetDxDevice()->SetFVF( D3DFVF_XYZ | D3DFVF_TEX1 );

        card.TextureFront()->UseTexture( 0 );
        game->GetDxDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, cardFrontVB, sizeof( CardVertex ) );

        card.TextureBack()->UseTexture( 0 );
        game->GetDxDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, cardBackVB, sizeof( CardVertex ) );
    }

    void RenderDeck( const CardSet & set, unify::Matrix matrix, dxi::Texture::shared_ptr side )
    {
        size_t count = set.Size();
        if ( count == 0 )
        {
            return;
        }

        if ( count == 1 )
        {
            Render( set.TopCard(), matrix );
            return;
        }

        dxi::Transform::Set( dxi::Transform::Index::World, matrix );
        dxi::core::Win32Game * game = dxi::core::Win32Game::GetGameInstance();

        //game->GetDxDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW  );

        float deckHeight = set.Size() / 50.0f;
        unify::Size< float > size = set.TopCard().Size();

        struct CardVertex {
            float x, y, z;
            float u, v;
        };
        
        CardVertex cardTopVB[] =
        {
            { size.width * -0.5f, deckHeight, size.height *  0.5f, 0.0f, 0.0f },
            { size.width *  0.5f, deckHeight, size.height *  0.5f, 1.0f, 0.0f },
            { size.width * -0.5f, deckHeight, size.height * -0.5f, 0.0f, 1.0f },
            { size.width *  0.5f, deckHeight, size.height * -0.5f, 1.0f, 1.0f },
        };

        CardVertex cardBottomVB[] =
        {
            { size.width *  0.5f, 0.0f, size.height *  0.5f, 0.0f, 0.0f },
            { size.width * -0.5f, 0.0f, size.height *  0.5f, 1.0f, 0.0f },
            { size.width *  0.5f, 0.0f, size.height * -0.5f, 0.0f, 1.0f },
            { size.width * -0.5f, 0.0f, size.height * -0.5f, 1.0f, 1.0f },
        };

        CardVertex cardSidesVB[] =
        {
            // Top   
            { size.width * -0.5f, deckHeight, size.height *  0.5f, 0.0f, 0.0f },
            { size.width * -0.5f, 0.0f, size.height *  0.5f, 1.0f, 0.0f },
            { size.width *  0.5f, deckHeight, size.height *  0.5f, 0.0f, 1.0f },

            { size.width * -0.5f, 0.0f, size.height *  0.5f, 1.0f, 0.0f },
            { size.width *  0.5f, 0.0f, size.height *  0.5f, 1.0f, 1.0f },
            { size.width *  0.5f, deckHeight, size.height *  0.5f, 0.0f, 1.0f },

            // Right
            { size.width * 0.5f, deckHeight, size.height *  0.5f, 0.0f, 0.0f },
            { size.width * 0.5f, 0.0f, size.height *  0.5f, 1.0f, 0.0f },
            { size.width * 0.5f, deckHeight, size.height * -0.5f, 0.0f, 1.0f },

            { size.width * 0.5f, 0.0f, size.height *  0.5f, 0.0f, 0.0f },
            { size.width * 0.5f, 0.0f, size.height * -0.5f, 1.0f, 0.0f },
            { size.width * 0.5f, deckHeight, size.height * -0.5f, 0.0f, 1.0f },

            // Bottom             
            { size.width * -0.5f, 0.0f, size.height * -0.5f, 0.0f, 0.0f },
            { size.width * -0.5f, deckHeight, size.height * -0.5f, 1.0f, 0.0f },
            { size.width *  0.5f, 0.0f, size.height * -0.5f, 0.0f, 1.0f },

            { size.width * -0.5f, deckHeight, size.height * -0.5f, 1.0f, 0.0f },
            { size.width *  0.5f, deckHeight, size.height * -0.5f, 1.0f, 1.0f },
            { size.width *  0.5f, 0.0f, size.height * -0.5f, 0.0f, 1.0f },

            // Left
            { size.width * -0.5f, 0.0f, size.height *  0.5f, 0.0f, 0.0f },
            { size.width * -0.5f, deckHeight, size.height *  0.5f, 1.0f, 0.0f },
            { size.width * -0.5f, 0.0f, size.height * -0.5f, 0.0f, 1.0f },

            { size.width * -0.5f, deckHeight, size.height *  0.5f, 0.0f, 0.0f },
            { size.width * -0.5f, deckHeight, size.height * -0.5f, 1.0f, 0.0f },
            { size.width * -0.5f, 0.0f, size.height * -0.5f, 0.0f, 1.0f },
        };

        game->GetDxDevice()->SetFVF( D3DFVF_XYZ | D3DFVF_TEX1 );

        set.TopCard().TextureBack()->UseTexture( 0 );
        game->GetDxDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, cardTopVB, sizeof( CardVertex ) );

        set.BottomCard().TextureFront()->UseTexture( 0 );
        game->GetDxDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, cardBottomVB, sizeof( CardVertex ) );

        side->UseTexture( 0 );
        game->GetDxDevice()->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 8, cardSidesVB, sizeof( CardVertex ) );
    }

    void RenderHand( const CardSet & set, unify::Matrix matrix, float width )
    {
        for ( size_t i = 0; i != set.Size(); ++i )
        {
            float ratio = (float)i / (float)(set.Size() - 1);
            unify::V3< float > cardPosition = unify::V3< float >::V3Interpolate( unify::V3< float >( width * -0.5f, 0, 0 ), unify::V3< float >( width * 0.5f, 0, 0 ), ratio );
            unify::Matrix cardMatrix = unify::Matrix::MatrixTranslate( cardPosition ) * matrix;
            Render( set.PeekCard( i ), cardMatrix );
        }
    }

    void RenderArea( const Area & area, unify::Matrix matrix, dxi::Texture::shared_ptr side )
    {
        for ( size_t p = 0; p != area.NumberOfPlaces(); ++p )
        {
            const Place & place = area.GetPlace( p );
            switch ( place.type )
            {
            case PlaceType::Deck:
                RenderDeck( place.cards, place.matrix * matrix, side );
                break;
            case PlaceType::Hand:
                RenderHand( place.cards, place.matrix * matrix, place.width );
                break;
            }
        }
    }
}
