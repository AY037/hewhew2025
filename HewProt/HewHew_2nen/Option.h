#pragma once
#include "Texture2D.h"
class Option
{
public:
	static Option& GetInstance(Camera* _camera) {
		static Option instance(_camera);
		return instance;
	}

	void Init();
	void Update();
	void Draw();
	void Uninit();

	void SetDrawActive(const bool _drawActive);
	bool GetDrawActive()const;
private:
	Option(Camera* _camera);//コンストラクタ
	~Option();//デストラクタ

	Camera* camera;
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> texturesParent;
	std::vector<std::shared_ptr<Texture2D>> textures;

	float screenLeft = 0;
	float screenTop = 0;

	DirectX::SimpleMath::Vector3 topPingPosition = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 bottomPingPosition = DirectX::SimpleMath::Vector3::Zero;
	bool drawActive = false;//描画するか
};
