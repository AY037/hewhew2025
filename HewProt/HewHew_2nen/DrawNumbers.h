#pragma once
#include "UI.h"
#include <deque>
#include <SimpleMath.h>

class DrawNumbers
{
public:
	DrawNumbers();//コンストラクタ
	~DrawNumbers();//デストラクタ

	void Init();
	void Update();
	void Draw(DirectX::XMMATRIX& _pm);
	void Uninit();
	//numInfoには"Player_HP"といった情報を入れる99999までしか描画できない
	void SetNum(std::string numInfo, int _num, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scale);
	void UpdateNum(std::string numInfo, int _num);
	void SetPosisiton(std::string numInfo, DirectX::SimpleMath::Vector3 pos);
	void CheckZero(std::deque<int>& _numbers);//数字の最初が０だったらポップ
private:
	std::unordered_map<std::string, std::deque<std::shared_ptr<UI>>> drawNumDatabase;

	// UV座標の情報
	float m_NumU;
	float m_NumV;
	float m_SplitX;
	float m_SplitY;
};
