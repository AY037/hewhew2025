#pragma once
#include "Texture2D.h"
#include <deque>
class DrawNumbers
{
public:
	DrawNumbers(Camera* _camera);//コンストラクタ
	~DrawNumbers();//デストラクタ

	void Init();
	void Update();
	void Draw();
	void Uninit();
	//numInfoには"Player_HP"といった情報を入れる99999までしか描画できない
	void SetNum(std::string numInfo, int _num, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scale);
	void UpdateNum(std::string numInfo, int _num);
	void SetPosisiton(std::string numInfo, DirectX::SimpleMath::Vector3 pos);
	void CheckZero(std::deque<int>& _numbers);//数字の最初が０だったらポップ
private:
	Camera* camera;
	std::unordered_map<std::string, std::deque<std::shared_ptr<Texture2D>>> drawNumDatabase;

	// UV座標の情報
	float m_NumU;
	float m_NumV;
	float m_SplitX;
	float m_SplitY;
};
