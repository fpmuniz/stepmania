#include "stdafx.h"
#include "MusicList.h"
#include "ThemeManager.h"

#define TITLES_START_X		THEME->GetMetricF("SelectGroup","TitlesStartX")
#define TITLES_SPACING_X	THEME->GetMetricF("SelectGroup","TitlesSpacingX")
#define TITLES_START_Y		THEME->GetMetricF("SelectGroup","TitlesStartY")
#define TITLES_COLUMNS		THEME->GetMetricI("SelectGroup","TitlesColumns")
#define TITLES_ROWS			THEME->GetMetricI("SelectGroup","TitlesRows")

MusicList::MusicList()
{
	CurGroup = 0;

	for( int i=0; i<TITLES_COLUMNS; i++ )
	{
		m_textTitles[i].LoadFromFont( THEME->GetPathTo("Fonts","normal") );
		m_textTitles[i].SetXY( TITLES_START_X + i*TITLES_SPACING_X, TITLES_START_Y );
		m_textTitles[i].SetHorizAlign( Actor::align_left );
		m_textTitles[i].SetVertAlign( Actor::align_top );
		m_textTitles[i].SetZoom( 0.5f );
		m_textTitles[i].SetShadowLength( 2 );
		this->AddSubActor( &m_textTitles[i] );
	}
}

void MusicList::AddGroup()
{
	m_ContentsText.Add(group());
}

void MusicList::AddSongsToGroup(const CArray<Song*,Song*> &Songs)
{
	// Generate what text will show in the contents for each group
	int group = m_ContentsText.GetSize()-1;

	m_ContentsText[group].m_iNumSongsInGroup = Songs.GetSize();

	for( int c=0; c<TITLES_COLUMNS; c++ )	// foreach col
	{
		CString sText;
		for( int r=0; r<TITLES_ROWS; r++ )	// foreach row
		{
			int iIndex = c*TITLES_ROWS + r;
			if( iIndex >= Songs.GetSize() )
				continue;

			if( c == TITLES_COLUMNS-1  &&  r == TITLES_ROWS-1 )
			{
				sText += ssprintf( "%d more.....", Songs.GetSize() - TITLES_COLUMNS * TITLES_ROWS - 1 );
				continue;
			}

			CString sTitle = Songs[iIndex]->GetFullTitle();
			// TODO:  Move this crop threshold into a theme metric or make automatic based on column width
			if( sTitle.GetLength() > 40 )
			{
				sTitle = sTitle.Left( 37 );
				sTitle += "...";
			}
			sText += sTitle + "\n";
		}
		m_ContentsText[m_ContentsText.GetSize()-1].ContentsText[c] = sText;
	}

}

/* TODO: tween? */
void MusicList::SetGroupNo(int group)
{
	CurGroup = group;

	for( int c=0; c<TITLES_COLUMNS; c++ )
	{
		m_textTitles[c].SetText( m_ContentsText[CurGroup].ContentsText[c] );
		m_textTitles[c].CropToWidth( int(TITLES_SPACING_X/m_textTitles[c].GetZoom()) );
	}
}

void MusicList::TweenOnScreen()
{
	for( int i=0; i<TITLES_COLUMNS; i++ )
	{
		m_textTitles[i].SetDiffuseColor( D3DXCOLOR(1,1,1,0) );
		m_textTitles[i].BeginTweeningQueued( 0.5f );
		m_textTitles[i].BeginTweeningQueued( 0.5f );
		m_textTitles[i].SetTweenDiffuseColor( D3DXCOLOR(1,1,1,1) );
	}
}

void MusicList::TweenOffScreen()
{
	for( int i=0; i<TITLES_COLUMNS; i++ )
	{
		m_textTitles[i].BeginTweeningQueued( 0.7f );
		m_textTitles[i].BeginTweeningQueued( 0.5f );
		m_textTitles[i].SetTweenDiffuseColor( D3DXCOLOR(1,1,1,0) );
	}
}
