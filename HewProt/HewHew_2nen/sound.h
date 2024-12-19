#pragma once

#include <xaudio2.h>
// サウンドファイル
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// サンプルBGM
	SOUND_LABEL_BGM001,			// サンプルBGM
	SE_DRAG_ROOP,
	SE_ATTACK_FLAME,
	SE_ATTACK_NORMAL,
	SE_DRAG,
	SE_HIT,
	SE_SPARK,
	SE_SPARK_ROOP,
	SE_JUMP,
	SE_DESTROY,



	SOUND_LABEL_MAX,
} SOUND_LABEL;

class Sound {
private:
	// パラメータ構造体
	typedef struct
	{
		LPCSTR filename;	// 音声ファイルまでのパスを設定
		bool bLoop;			// trueでループ。通常BGMはture、SEはfalse。
	} PARAM;

	PARAM m_param[SOUND_LABEL_MAX] =
	{
		{"asset/BGM/sample000.wav", true},	// サンプルBGM（ループさせるのでtrue設定）
		{"asset/BGM/Get_Tangled.wav", true},	// サンプルBGM
		{"asset/SE/drag_roop.wav", true},
		{"asset/SE/attack_flame.wav", false},  // サンプルSE
		{"asset/SE/attck_normal.wav", false},
		{"asset/SE/drag.wav", false},// サンプルSE（ループしないのでfalse設定）
		{"asset/SE/hit.wav", false},
		{"asset/SE/spark.wav", false},
		{"asset/SE/spark_roop.wav", true},
		{"asset/SE/jump.wav", false},
		{"asset/SE/destroy.wav", false},
	};

	IXAudio2* m_pXAudio2 = NULL;
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;
	IXAudio2SourceVoice* m_pSourceVoice[SOUND_LABEL_MAX];
	WAVEFORMATEXTENSIBLE m_wfx[SOUND_LABEL_MAX]; // WAVフォーマット
	XAUDIO2_BUFFER m_buffer[SOUND_LABEL_MAX];
	BYTE* m_DataBuffer[SOUND_LABEL_MAX];

	HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);
	HRESULT ReadChunkData(HANDLE, void*, DWORD, DWORD);

public:
	static Sound& GetInstance() {
		static Sound instance;  // 静的なインスタンスを作成（初回アクセス時に生成）
		return instance;            // 常に同じインスタンスを返す
	}
	// ゲームループ開始前に呼び出すサウンドの初期化処理
	HRESULT Init(void);

	// ゲームループ終了後に呼び出すサウンドの解放処理
	void Uninit(void);

	// 引数で指定したサウンドを再生する
	void Play(SOUND_LABEL label);

	// 引数で指定したサウンドを停止する
	void Stop(SOUND_LABEL label);

	// 引数で指定したサウンドの再生を再開する
	void Resume(SOUND_LABEL label);

};