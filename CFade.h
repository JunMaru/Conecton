/*-----------------------------------------------------------------------------
	フェードクラス
	author	OkadaMoeto
	since	20140709
-----------------------------------------------------------------------------*/

// ２重インクルード防止
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "CScene2D.h"

/*-----------------------------------------------------------------------------
	フェードクラス
-----------------------------------------------------------------------------*/
class CFade : public CScene2D
{
	public:
		// フェード種別
		typedef enum
		{
			FADETYPE_NONE = 0,
			FADETYPE_IN,
			FADETYPE_UNIN,
			FADETYPE_OUT,
			FADETYPE_UNOUT,
			FADETYPE_INFINITY,
			FADETYPE_MAX
		}FADETYPE;

		CFade(
				int nPriority = 7,
				CScene::OBJTYPE objtype = CScene::OBJTYPE_FADE);
		virtual ~CFade();

		/*-----------------------------------------------------------------------------
			2Dポリゴンのフェード生成
			return	CFade*	CFadeのインスタンスを返す
			param	pos		ポリゴン中心表示座標
			param	rot		向き
			param	col		表示色
		-----------------------------------------------------------------------------*/
		static CFade* Create(
								D3DXVECTOR3 pos,
								D3DXVECTOR3 rot,
								D3DXCOLOR color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col);
		void Uninit(void);
		void Update(void);
		void Draw(void);

		/*-----------------------------------------------------------------------------
			フェードの開始
			param	type		フェードタイプ
			param	time_frame	フェード時間
			param	color		フェード色
		-----------------------------------------------------------------------------*/
		void Start(FADETYPE type, float time_frame, D3DXCOLOR color);

		/*-----------------------------------------------------------------------------
			フェード種別の取得
			return	FADETYPE		現在のフェード種別
		-----------------------------------------------------------------------------*/
		FADETYPE GetFadetype(void){ return m_fadetype; }
		/*-----------------------------------------------------------------------------
			フェード種別の設定
			param	type		フェード種別の設定値
		-----------------------------------------------------------------------------*/
		void GetFadetype(FADETYPE type){ m_fadetype = type; }

	private:
		// フェード色
		D3DXCOLOR m_color;
		// フェード時間(フレーム単位)
		float m_timeFrame;
		// フェードの使用フラグ
		bool m_bUse;
		// フェードの種類
		FADETYPE m_fadetype;
		// フェードの透明度
		float m_alpha;
};
