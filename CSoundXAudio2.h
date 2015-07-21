/*-----------------------------------------------------------------------------
	サウンドユーティリティ
	author	OkadaMoeto
	since	20140616
-----------------------------------------------------------------------------*/

// インクルードガード( 二重インクルード防止 )
#pragma once

/*-----------------------------------------------------------------------------
	インクルードファイル
-----------------------------------------------------------------------------*/
#include "Main.h"

/*-----------------------------------------------------------------------------
	パラメータ構造体
-----------------------------------------------------------------------------*/
typedef struct
{
	char *pFilename;	// サウンドファイル名
	bool bLoop;			// 再生時のループフラグ
}PARAM;

/*-----------------------------------------------------------------------------
	サウンド(XAudio2)クラス
-----------------------------------------------------------------------------*/
class CSoundXAudio2
{
	public:
		// サウンドファイルラベル名（m_paramに同順にファイルパスを記述）
		typedef enum
		{
			SL_BGM_TEST,
			SL_SE_SHOT_TEST,
			SL_BGM_GAME,
			SL_BGM_STAGESELECT,
			SL_BGM_TITLE,
			SL_JINGLE_CLEAR,
			SL_JINGLE_OVER,
			SL_SE_BEECON_CONNECT,
			SL_SE_BLOCK_BRAKE,
			SL_SE_BOX_SLIP,
			SL_SE_DAMAGE,
			SL_SE_KERNEL,
			SL_SE_LASER_END,
			SL_SE_LASER_START,
			SL_SE_MAGNET,
			SL_SE_PICON,
			SL_SE_POWERUP_METAL,
			SL_SE_POWERUP_MINI,
			SL_SE_POWERUP_POWERFUL,
			SL_SE_ROT_ARROW,
			SL_SE_SANDSTORM,
			SL_SE_ENTER,
			SL_SE_WALK_NORMAL,
			SL_SE_WALK_METAL,
			SL_SE_WALK_MINI,
			SL_SE_WALK_POWERFUL,
			SL_SE_WARP,
			SL_MAX,
		}SOUND_LABEL;

		CSoundXAudio2();
		~CSoundXAudio2();

		/*-----------------------------------------------------------------------------
			XAudio2の初期化
			param	hWnd	ウィンドウのハンドル
		-----------------------------------------------------------------------------*/
		HRESULT Init(HWND hWnd);
		void Uninit(void);

		/*-----------------------------------------------------------------------------
			サウンド再生
			param	label	サウンドID(SOUNDLABLE_xx)
		-----------------------------------------------------------------------------*/
		HRESULT Play(SOUND_LABEL label);

		/*-----------------------------------------------------------------------------
			サウンド停止
			param	label	サウンドID(SOUNDLABLE_xx)
		-----------------------------------------------------------------------------*/
		void Stop(SOUND_LABEL label);

		// サウンド全停止
		void AllStop(void);

	private:
		/*-----------------------------------------------------------------------------
			チャンクのチェック
			return	HRESULT				チェック結果
			param	hFile				ファイルハンドル
			param	format				フォーマット
			param	pChunkSize			チャンクのサイズ
			param	pChunkDataPosition	チャンクのデータのディレクトリ
		-----------------------------------------------------------------------------*/
		HRESULT CheckChunk(
							HANDLE hFile,
							DWORD format,
							DWORD *pChunkSize,
							DWORD *pChunkDataPosition);

		/*-----------------------------------------------------------------------------
			チャンクデータの読み込み
			return	HRESULT			読込み結果
			param	hFile			ファイルハンドル
			param	pBuffer			バッファのポインタ
			param	dwBuffersize	バッファのサイズ
			param	dwBufferoffset	バッファのポジション
		-----------------------------------------------------------------------------*/
		HRESULT ReadChunkData(
								HANDLE hFile,
								void *pBuffer,
								DWORD dwBuffersize,
								DWORD dwBufferoffset);

		// XAudio2オブジェクトへのインターフェイス
		IXAudio2 *m_pXAudio2;
		// マスターボイス
		IXAudio2MasteringVoice *m_pMasteringVoice;
		// ソースボイス
		IXAudio2SourceVoice *m_pSourceVoice[SL_MAX];
		// オーディオデータ
		BYTE *m_pDataAudio[SL_MAX];
		// オーディオデータサイズ
		DWORD m_sizeAudio[SL_MAX];

		// メディアのパラメータ( PARAM構造体のフォーマット )
		static const PARAM m_param[SL_MAX];
};
