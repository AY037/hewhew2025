#pragma once
#include "Audio.h"
#include <unordered_map>

typedef enum
{
	SE_DRAG_ROOP = 0,
	SE_ATTACK_FLAME,
	SE_ATTACK_NORMAL,
	SE_DRAG,
	SE_HIT,
	SE_JUMP,
	SE_DESTROY,
	SE_BULLET,
	SE_VOICE_ATTACK_NORMAL,
	SE_VOICE_ATTACK_BIGATTACK1,
	SE_VOICE_ATTACK_BIGATTACK2,
	SE_VOICE_DAMAGE_NORMAL1
};

class AudioManager
{
public:
	static AudioManager& GetInstance()
	{
		static AudioManager instance;
		return instance;
	}
	void Init();
	void Update();
	void UnInit();
	//�T�E���h�̃��[�h
	void LoadSound(std::string _fileName, int _soundId);
	void LoadSounds();
	//�Đ�
	void PlayAudio(int _soundId, bool _roop, float _volume = 1.0f);
	//�T�E���h�̃{�����[������
	void SetPlayAudioVolume(int _soundId, float _volume);
	//�Đ���~
	void StopAudio(int _soundId);
	void StopAllAudio();
	//�}�X�^�{�����[���̐ݒ�
	void SetMasterVolume(float volume);
	float GetMasterVolume() const;
private:
	AudioManager();
	~AudioManager();
	std::unique_ptr<DirectX::AudioEngine> m_AudioEngine;
	std::unordered_map<int, std::unique_ptr<DirectX::SoundEffect>> m_SoundEffect;//SE
	std::unordered_map<int, std::unique_ptr<DirectX::SoundEffectInstance>> m_SoundEffectInstance;//BGM

	float m_MasterVolume = 0.5f;
};

