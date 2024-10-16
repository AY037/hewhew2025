#pragma once
#include <map>
#include <type_traits>
#include <concepts>
#include <functional>
#include "Game.h"
//�������I�[�o�[�G���W�j�A�����O����������A��邩�킩��Ȃ�
class DIContainer
{
	using Provider = std::function<void* (DIContainer&)>;//DIContainer�i�����֐��j���󂯎�萶�������I�u�W�F�N�g�̃|�C���^��Ԃ�
	using TypeId = size_t;
public:
	void Init();
	// Key�̐����ݒ��o�^����֐�
	template<typename Key, typename T>//auto�̍ŋ��Ń}�W�łȂ�ł�����
	void Register()
	{
		Register<Key>([](auto c) { return new T; });
	}

	// Key�̐����ݒ��o�^����֐�
	template<typename Key>
	void Register(Provider&& func)//Register�֐��̃I�[�o�[���[�h�A�����Ŕ���
	{
		TypeId typeId = typeid(Key).hash_code();//�����֐��Ɏ���ID�����蓖��
		TypeId hasType = m_providerMap.find(typeId) != m_providerMap.end();//map����typeId�ɑΉ�����v�f�����邩�T��

		// ��d�o�^�͕s��
		if (hasType == true)
		{
			printf("DIContainer: ���ɓo�^���ꂽ�N���X�ł�\n");
			return;
		}

		m_providerMap[typeId] = func;
	}

	// Key������ۂɃI�u�W�F�N�g���擾����֐�
	template<typename Key>
	Key* Resolve()
	{
		TypeId typeId = typeid(Key).hash_code();
		TypeId hasType = m_providerMap.find(typeId) != m_providerMap.end();


		if (hasType == false)
		{
			printf("DIContainer: ���ɓo�^���ꂽ�N���X�ł�\n");
			return nullptr;
		}

		return static_cast<Key*>(m_providerMap[typeId](*this));//void*����key*�ɃL���X�g
	}

private:

	// �����N���XId�ƃI�u�W�F�N�g�̐����֐��ւ̃}�b�v
	std::map<TypeId, Provider> m_providerMap;//�����֐�(Provider)��TypeId��R�Â�TypeID���獂���Ɍ����ł���
};
