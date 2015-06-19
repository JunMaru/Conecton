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
			SOUND_LABEL_BGM_000,
			SOUND_LABEL_SE_SHOT_000,
			SOUND_LABEL_MAX,
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
		IXAudio2SourceVoice *m_pSourceVoice[SOUND_LABEL_MAX];
		// オーディオデータ
		BYTE *m_pDataAudio[SOUND_LABEL_MAX];
		// オーディオデータサイズ
		DWORD m_sizeAudio[SOUND_LABEL_MAX];

		// メディアのパラメータ( PARAM構造体のフォーマット )
		static const PARAM m_param[SOUND_LABEL_MAX];
};
