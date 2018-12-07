// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/ui/Control.h>
#include <CommCtrl.h>
#include <string>

namespace ui
{
	class ListView : public Control
	{
		void SetExStyle( DWORD style, bool enable );

	public:		   			
		ListView( HWND parent, HWND handle, create::IControl * createControl );

		virtual ~ListView();

		bool AddColumn( int column, std::string text, int width );
		bool AddColumn( int column, std::wstring text, int width );

		bool InsertItem( int column, int index, std::string text );
		bool InsertItem( int column, int index, std::wstring text );

		bool SetItemText( std::wstring text );

		int GetSelectedItem() const;

	public: // WinApi functions...	
		// Styles...
		void SetGridLines( bool enable );
		void SetSubItemImages( bool enable );
		void SetCheckBoxes( bool enable );
		void SetTrackSelect( bool enable );
		void SetHeaderDrawDrop( bool enable );	
		void SetFullRowSelect( bool enable );	
		void SetOnClickActivate( bool enable );	
		void SetTwoClickActivate( bool enable );	
		void SetFlatSB( bool enable );	
		void SetRegional( bool enable );	
		void SetInfoTip( bool enable );	
		void SetUnderlineHot( bool enable );	
		void SetUnderlineCold( bool enable );	
		void SetMultiWorkAreas( bool enable );	
		void SetLabelTop( bool enable );	
		void SetBorderSelect( bool enable );	
		void SetDoubleBuffer( bool enable );	
		void SetHideLabels( bool enable );	
		void SetSIngleRow( bool enable );	
		void SetSnapToGrid( bool enable );	
		void SetSimpleSelect( bool enable );	
		void SetJustifyColumns( bool enable );	
		void SetTransparentBkgnd( bool enable );	
		void SetTransparentShadowText( bool enable );	
		void SetAutoArrange( bool enable );	
		void SetHeaderInAllViews( bool enable );	
		void SetAutoCheckSelect( bool enable );	
		void SetAutoSizeColumns( bool enable );	
		void SetColumnSnapPoints( bool enable );	
		void SetColumnOverFlow( bool enable );	 

		enum class LVIR {
			Bounds = LVIR_BOUNDS,
			Icon = LVIR_ICON,
			Label = LVIR_LABEL,
			SelectBounds = LVIR_SELECTBOUNDS
		};

		enum class Notify : unsigned int
		{
			ItemChanging = LVN_ITEMCHANGING,
			ItemChanged = LVN_ITEMCHANGED,
			InsertItem = LVN_INSERTITEM,
			DeleteItem = LVN_DELETEITEM,
			DeleteAllItems = LVN_DELETEALLITEMS,
			BeginLabelEditA = LVN_BEGINLABELEDITA,
			BeginLabelEditW = LVN_BEGINLABELEDITW,
			EndLabelEditA = LVN_ENDLABELEDITA,
			EndLabelEditW = LVN_ENDLABELEDITW,
			ColumnClick = LVN_COLUMNCLICK,
			BeginDrag = LVN_BEGINDRAG,
			BeginRDrag = LVN_BEGINRDRAG,
			ODCacheHint = LVN_ODCACHEHINT,
			ODFindItemA = LVN_ODFINDITEMA,
			ODFindItemW = LVN_ODFINDITEMW,
			ItemActive = LVN_ITEMACTIVATE,
			ODStateChanged = LVN_ODSTATECHANGED,
			HotTrack = LVN_HOTTRACK,
			GetDispInfoA = LVN_GETDISPINFOA,
			GetDispInfoW = LVN_GETDISPINFOW,
			SetDispInfoA = LVN_SETDISPINFOA,
			SetDispInfoW = LVN_SETDISPINFOW,
		};


		int DeleteAllItems();
		unsigned int GetSelectedColumn() const;
		int GetNextItem( int start, UINT flags ) const;
		IControl::ptr GetEditControl();
		bool GetItemRect( int item, RECT* rect, LVIR code ) const;
		bool GetSubItemRect( int item, int subItem, RECT* rect, LVIR code ) const;
		bool SetColumnOrderArray( int count, int * array );
	};
}