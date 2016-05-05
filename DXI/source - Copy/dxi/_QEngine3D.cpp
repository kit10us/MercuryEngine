#include "dxi\QEngine3D.h"

// **************************************************
// * QEngine3D
// **************************************************

QEngine3D::QEngine3D()
{
}

QEngine3D::~QEngine3D()
{
}

QSCRETURN QEngine3D::TakeCommand( const QParse* pCommand )
{
	QVar var;

	if( pCommand->FuncIs( "LISTRENDERSTATES" ) )
	{
		ListRenderStates();
		return ClearCommand();
	}

	if( pCommand->FuncIs( "SETSTATE" ) )
	{
		ARGCHECK_IS(1);
		SUBCHECK();

		// Find the state
		for( TInt32 iState = 0; g_dwD3DRS[iState] != -1 && !pCommand->SubIs( g_szD3DRS[iState] ); iState++ );
		if( g_dwD3DRS[iState] == -1 )
			return SendError( "Not a valid D3D Render State!", QSC_FAILED );

		TInt32 iValue;
		ASSIGN_INT( iValue, var, pCommand->Arg(0) );
		
		HRESULT hr;
		hr = GetDevice()->SetRenderState( (D3DRENDERSTATETYPE)g_dwD3DRS[iState], iValue );

		if( FAILED(hr) )
			return SendError( "Failed to set render state! (check value)", QSC_FAILED );

		return ClearCommand();
	}

	return ME_CLIENT::TakeCommand( pCommand );
}

void QEngine3D::ListCommands()
{
	SendConsole( "'   SetRenderState.State = Value" );
	SendConsole( "'   ListRenderStates" );
}

void QEngine3D::ListRenderStates()
{
	SendConsole( "'Listing Render States..." ) ;
	TInt32 iNumStates = sizeof( g_dwD3DRS ) / sizeof( TDWord );

	TChar* pszTemp = NullCreate();
	for( TInt32 iState = 0; g_dwD3DRS[iState] != -1; iState++ )
	{
		NullCopyVar( pszTemp, "'   ", g_szD3DRS[iState] );
		SendConsole( pszTemp );
		NullDelete( pszTemp );
	}
	SendConsole( "'" );
}

