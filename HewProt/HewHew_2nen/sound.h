#pragma once

#include <xaudio2.h>
// �T�E���h�t�@�C��
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// �T���v��BGM
	SOUND_LABEL_BGM001,			// �T���v��BGM
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
	// �p�����[�^�\����
	typedef struct
	{
		LPCSTR filename;	// �����t�@�C���܂ł̃p�X��ݒ�
		bool bLoop;			// true�Ń��[�v�B�ʏ�BGM��ture�ASE��false�B
	} PARAM;

	PARAM m_param[SOUND_LABEL_MAX] =
	{
		{"asset/BGM/sample000.wav", true},	// �T���v��BGM�i���[�v������̂�true�ݒ�j
		{"asset/BGM/Get_Tangled.wav", true},	// �T���v��BGM
		{"asset/SE/drag_roop.wav", true},
		{"asset/SE/attack_flame.wav", false},  // �T���v��SE
		{"asset/SE/attck_normal.wav", false},
		{"asset/SE/drag.wav", false},// �T���v��SE�i���[�v���Ȃ��̂�false�ݒ�j
		{"asset/SE/hit.wav", false},
		{"asset/SE/spark.wav", false},
		{"asset/SE/spark_roop.wav", true},
		{"asset/SE/jump.wav", false},
		{"asset/SE/destroy.wav", false},
	};

	IXAudio2* m_pXAudio2 = NULL;
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;
	IXAudio2SourceVoice* m_pSourceVoice[SOUND_LABEL_MAX];
	WAVEFORMATEXTENSIBLE m_wfx[SOUND_LABEL_MAX]; // WAV�t�H�[�}�b�g
	XAUDIO2_BUFFER m_buffer[SOUND_LABEL_MAX];
	BYTE* m_DataBuffer[SOUND_LABEL_MAX];

	HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);
	HRESULT ReadChunkData(HANDLE, void*, DWORD, DWORD);

public:
	static Sound& GetInstance() {
		static Sound instance;  // �ÓI�ȃC���X�^���X���쐬�i����A�N�Z�X���ɐ����j
		return instance;            // ��ɓ����C���X�^���X��Ԃ�
	}
	// �Q�[�����[�v�J�n�O�ɌĂяo���T�E���h�̏���������
	HRESULT Init(void);

	// �Q�[�����[�v�I����ɌĂяo���T�E���h�̉������
	void Uninit(void);

	// �����Ŏw�肵���T�E���h���Đ�����
	void Play(SOUND_LABEL label);

	// �����Ŏw�肵���T�E���h���~����
	void Stop(SOUND_LABEL label);

	// �����Ŏw�肵���T�E���h�̍Đ����ĊJ����
	void Resume(SOUND_LABEL label);

};