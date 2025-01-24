#include "DrawNumbers.h"
#include "GameManager.h"
using namespace DirectX::SimpleMath;

DrawNumbers::DrawNumbers()//コンストラクタ
{
	m_NumU = 0;
	m_NumV = 0;
	m_SplitX = 10;
	m_SplitY = 1;
}

DrawNumbers::~DrawNumbers()//デストラクタ
{
	Uninit();
}

void DrawNumbers::Init()
{
}

void DrawNumbers::Update()
{

}

void DrawNumbers::Draw(DirectX::XMMATRIX& _pm)
{
	for (auto& pair : drawNumDatabase)
	{
		if(!pair.second.empty())
		{
			for (auto& number : pair.second)
			{
				if(number->GetActiveDraw())
				number->Draw(_pm);
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
	//有効な数字で無い場合
	_num = (_num > 99999) ? 99999 : _num;
	_num = (_num < 0) ? 0 : _num;
	int digit = 10000;
	int digits = 5;//digitの桁数
	float numSize = scale.x / digits;
	for (int i = 0; i < digits; ++i)
	{
		int num = static_cast<int>(_num / digit);
		std::shared_ptr<UI> numTex = std::make_shared<UI>();
		numTex->SetObjectTexName("asset/number.png");//画像の指定
		numTex->SetPos(pos.x + (numSize * i), pos.y, 0.0f);//位置を指定
		numTex->SetSize(numSize, scale.y, 0.0f);//大きさを指定
		numTex->Init();
		numTex->SetUV(num, m_NumV, m_SplitX, m_SplitY);//UV座標を指定
		drawNumDatabase[numInfo].push_back(numTex);
		_num -= num * digit;
		digit /= 10;
	}
}

void DrawNumbers::UpdateNum(std::string numInfo, int _num)
{
	//有効な数字で無い場合
	_num = (_num > 99999) ? 99999 : _num;
	_num = (_num < 0) ? 0 : _num;
	int digit = 10000;
	int digits = 5;//digitの桁数
	std::deque<int> numbers;
	for (int i = 0; i < digits; ++i)
	{
		int num = static_cast<int>(_num / digit);
		numbers.push_back(num);
		_num -= num * digit;
		digit /= 10;
	}

	//先頭が０じゃなくなるまでポップ
	CheckZero(numbers);
	//サイズを合わせる
	int sizeDif = drawNumDatabase[numInfo].size()-numbers.size();
	//数字の更新
	digits = numbers.size();
	for (int i = 0; i < drawNumDatabase[numInfo].size(); ++i)
	{
		if(i>= sizeDif)
		{
			drawNumDatabase[numInfo][i]->SetActiveDraw(true);
			drawNumDatabase[numInfo][i]->SetUV(numbers[i- sizeDif], m_NumV, m_SplitX, m_SplitY);//UV座標を指定
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
	int digits = drawNumDatabase[numInfo].size();//digitの桁数
	float numSizeX = drawNumDatabase[numInfo][0]->GetSize().x / digits;
	for (int i = 0; i < digits; ++i)
	{
		drawNumDatabase[numInfo][i]->SetPos(pos.x + (numSizeX * i), pos.y, 0.0f);//位置を指定
	}
}

void DrawNumbers::CheckZero(std::deque<int>& _numbers)//数字の最初が０だったらポップ
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