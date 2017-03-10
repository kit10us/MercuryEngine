LoadTexture( grasstile, grass_tile );

		pGeo = new QTerra( 1, 10, 10 );
		g_pGrass = (QTerra*)pGeo;
		RefAssign( m_Terrain[uTerrain].pGeometryArt[0], pGeo );

		iLevel = 0;

		((QTerra*)pGeo)->CreateLevel( iLevel, QVERTEX, 1, 1, 2.5f, &TTexCoords(0,0), &TTexCoords(fTextureRepeat,fTextureRepeat) );
		((QTerra*)pGeo)->SetTexture( iLevel, 0, idGrassTile );
