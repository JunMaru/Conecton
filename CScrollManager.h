/*-------------------------------------------------------------------------------------------
		CScrollManager
		author HaradaYuto
		since 2015515
--------------------------------------------------------------------------------------------*/

//スクロールの管理を行います

//描画的には 座標 - スクロール値でスクリーン座標がだせますね
//

// ２重インクルード防止
#pragma once
#include "Main.h"

/*-------------------------------------------------------------------------------------------
	include
--------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------
	class
--------------------------------------------------------------------------------------------*/

class CScrollManager
{
	public:
		CScrollManager(){}
		~CScrollManager(){}

		/*-------------------------------------------------------------------------------------------
			スクロール値を取得する
			return スクロール値
		--------------------------------------------------------------------------------------------*/

		static D3DXVECTOR2 GetScrollWorld(){ return m_scrollWorld; }

		/*-------------------------------------------------------------------------------------------
			スクロール値を設定する
			param スクロール値
		--------------------------------------------------------------------------------------------*/

		static void SetScrollWorld( const D3DXVECTOR2 &scroll_val ){ m_scrollWorld  =scroll_val; }

		/*--------------------------------------------------------------------------------------------
			スクロール値を計算する
			param スクロール加算（減算）値 
		--------------------------------------------------------------------------------------------*/
		static void CalScrollWorld( const D3DXVECTOR2 &scroll_val ){ m_scrollWorld += scroll_val; }

		static void Init(void){ m_scrollWorld = D3DXVECTOR2(0, 0); }

	private:
		static D3DXVECTOR2 m_scrollWorld;//スクロール値
		
};