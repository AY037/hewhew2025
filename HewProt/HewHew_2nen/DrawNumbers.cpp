#include "DrawNumbers.h"
#include "GameManager.h"
using namespace DirectX::SimpleMath;

DrawNumbers::DrawNumbers(Camera* _camera)//�R���X�g���N�^
{
	camera = _camera;
	m_NumU = 0;
	m_NumV = 0;
	m_SplitX = 10;
	m_SplitY = 1;
}

DrawNumbers::~DrawNumbers()//�f�X�g���N�^
{
	Uninit();
}

void DrawNumbers::Init()
{
	for (auto& pair : drawNumDatabase)
	{
		for (auto& obj : pair.second)
		{
			obj->SetCamera(camera);
		}
	}

	const auto& gameManager = GameManager::GetInstance();
}

void DrawNumbers::Update()
{

}

void DrawNumbers::Draw()
{
	for (auto& pair : drawNumDatabase)
	{
		if(!pair.second.empty())
		{
			for (auto& number : pair.second)
			{
				if(number->GetActiveDraw())
				number->Draw();
			}
		}
	}
}

void DrawNumbers::Uninit()
{
	for (auto& pair : drawNumDatabase)
	{
		for (auto& obj : pair.second)
		{
			obj->Uninit();
		}
	}
	drawNumDatabase.clear();
}

void DrawNumbers::SetNum(std::string numInfo, int _num, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scale)
{
	//�L���Ȑ����Ŗ����ꍇ
	_num = (_num > 99999) ? 99999 : _num;
	_num = (_num < 0) ? 0 : _num;
	int digit = 10000;
	int digits = 5;//digit�̌���
	float numSize = scale.x / digits;
	for (int i = 0; i < digits; ++i)
	{
		int num = static_cast<int>(_num / digit);
		std::shared_ptr<Texture2D> numTex = std::make_shared<Texture2D>(camera);
		numTex->SetTexture("assets/texture/number.png");//�摜�̎w��
		numTex->SetPosition(pos.x + (numSize * i), pos.y, 0.0f);//�ʒu���w��
		numTex->SetScale(numSize, scale.y, 0.0f);//�傫�����w��
		numTex->SetUV(num, m_NumV, m_SplitX, m_SplitY);//UV���W���w��
		numTex->Init();
		drawNumDatabase[numInfo].push_back(numTex);
		_num -= num * digit;
		digit /= 10;
	}
}

void DrawNumbers::UpdateNum(std::string numInfo, int _num)
{
	//�L���Ȑ����Ŗ����ꍇ
	_num = (_num > 99999) ? 99999 : _num;
	_num = (_num < 0) ? 0 : _num;
	int digit = 10000;
	int digits = 5;//digit�̌���
	std::deque<int> numbers;
	for (int i = 0; i < digits; ++i)
	{
		int num = static_cast<int>(_num / digit);
		numbers.push_back(num);
		_num -= num * digit;
		digit /= 10;
	}

	//�擪���O����Ȃ��Ȃ�܂Ń|�b�v
	CheckZero(numbers);
	//�T�C�Y�����킹��
	int sizeDif = drawNumDatabase[numInfo].size()-numbers.size();
	//�����̍X�V
	digits = numbers.size();
	for (int i = 0; i < drawNumDatabase[numInfo].size(); ++i)
	{
		if(i>= sizeDif)
		{
			drawNumDatabase[numInfo][i]->SetActiveDraw(true);
			drawNumDatabase[numInfo][i]->SetUV(numbers[i- sizeDif], m_NumV, m_SplitX, m_SplitY);//UV���W���w��
		}
		else
		{
			drawNumDatabase[numInfo][i]->SetActiveDraw(false);
		}
	}
}

void DrawNumbers::SetPosisiton(std::string numInfo, DirectX::SimpleMath::Vector3 pos)
{

	int digit = 10000;
	int digits = drawNumDatabase[numInfo].size();//digit�̌���
	float numSizeX = drawNumDatabase[numInfo][0]->GetScale().x / digits;
	for (int i = 0; i < digits; ++i)
	{
		drawNumDatabase[numInfo][i]->SetPosition(pos.x + (numSizeX * i), pos.y, 0.0f);//�ʒu���w��
	}
}

void DrawNumbers::CheckZero(std::deque<int>& _numbers)//�����̍ŏ����O��������|�b�v
{
	if (!_numbers.empty())
	{
		if (_numbers.front() == 0)
		{
			_numbers.pop_front();
			CheckZero(_numbers);
		}
	}
}