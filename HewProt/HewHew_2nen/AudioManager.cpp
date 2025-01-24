#include "AudioManager.h"

#pragma comment (lib, "DirectXTK.lib") 

using namespace DirectX;
AudioManager::AudioManager() {

}

AudioManager::~AudioManager() {

}

void AudioManager::Init()
{
	m_AudioEngine = std::make_unique<AudioEngine>();
}

void AudioManager::Update()
{
	if (m_AudioEngine) {
		m_AudioEngine->Update();
	}
}

void AudioManager::LoadSound(std::string _fileName, int _soundId)
{
	std::wstring wstr(_fileName.begin(), _fileName.end());
	m_SoundEffect[_soundId] = std::make_unique<SoundEffect>(m_AudioEngine.get(), wstr.c_str());
	m_SoundEffectInstance[_soundId] = m_SoundEffect[_soundId]->CreateInstance();
}

void AudioManager::LoadSounds()
{
	//Player
	LoadSound("asset/SE/Player/drag_roop.wav", SE_DRAG_ROOP);
	LoadSound("asset/SE/Player/attack_flame.wav", SE_ATTACK_FLAME);
	LoadSound("asset/SE/Player/attck_normal.wav", SE_ATTACK_NORMAL);
	LoadSound("asset/SE/Player/drag.wav", SE_DRAG);
	LoadSound("asset/SE/Player/jump.wav", SE_JUMP);
	LoadSound("asset/SE/Player/Voice/attack_Normal.wav", SE_VOICE_ATTACK_NORMAL);
	LoadSound("asset/SE/Player/Voice/attack_BigAttack1.wav", SE_VOICE_ATTACK_BIGATTACK1);
	LoadSound("asset/SE/Player/Voice/attack_BigAttack2.wav", SE_VOICE_ATTACK_BIGATTACK2);
	LoadSound("asset/SE/Player/Voice/damage_Normal1.wav", SE_VOICE_DAMAGE_NORMAL1);

	//Enemy
	LoadSound("asset/SE/Enemy/bullet.wav", SE_BULLET);
		

	LoadSound("asset/SE/hit.wav", SE_HIT);
	LoadSound("asset/SE/destroy.wav", SE_DESTROY);
}

void AudioManager::PlayAudio(int _soundId, bool _roop, float _volume) {
	if (m_SoundEffect[_soundId]) {

		// 再生中の場合、一度停止してリセット
		if (m_SoundEffectInstance[_soundId]->GetState() == SoundState::STOPPED) {
			m_SoundEffectInstance[_soundId]->Stop();
		}
		m_SoundEffectInstance[_soundId]->SetVolume(_volume * m_MasterVolume);
		m_SoundEffectInstance[_soundId]->Play(_roop);
	}
}


void AudioManager::SetPlayAudioVolume(int _soundId, float _volume)
{
	if (m_SoundEffect[_soundId]) {
		m_SoundEffectInstance[_soundId]->SetVolume(_volume * m_MasterVolume);
	}
}

void AudioManager::StopAudio(int _soundId) {
	if (m_SoundEffect[_soundId]) {
		m_SoundEffectInstance[_soundId]->Stop();
	}
}

void AudioManager::StopAllAudio() {
	for (auto& pair : m_SoundEffectInstance)
	{
		pair.second->Stop();
	}
}
void AudioManager::UnInit()
{
	for (auto& pair : m_SoundEffectInstance)
	{
		pair.second.reset();
	}
	for (auto& pair : m_SoundEffect)
	{
		pair.second.reset();
	}

	m_AudioEngine.reset();
}

//マスタボリュームの設定
void AudioManager::SetMasterVolume(float volume)
{
	volume = (volume < 0.0f) ? 0.0f : volume;
	volume = (volume > 1.0f) ? 1.0f : volume;

	m_MasterVolume = volume;
}

float AudioManager::GetMasterVolume() const
{
	return m_MasterVolume;
}