#pragma once
#include "Texture2D.h"
#include <deque>
class DrawNumbers
{
public:
	DrawNumbers(Camera* _camera);//�R���X�g���N�^
	~DrawNumbers();//�f�X�g���N�^

	void Init();
	void Update();
	void Draw();
	void Uninit();
	//numInfo�ɂ�"Player_HP"�Ƃ�������������99999�܂ł����`��ł��Ȃ�
	void SetNum(std::string numInfo, int _num, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scale);
	void UpdateNum(std::string numInfo, int _num);
	void SetPosisiton(std::string numInfo, DirectX::SimpleMath::Vector3 pos);
	void CheckZero(std::deque<int>& _numbers);//�����̍ŏ����O��������|�b�v
private:
	Camera* camera;
	std::unordered_map<std::string, std::deque<std::shared_ptr<Texture2D>>> drawNumDatabase;

	// UV���W�̏��
	float m_NumU;
	float m_NumV;
	float m_SplitX;
	float m_SplitY;
};
