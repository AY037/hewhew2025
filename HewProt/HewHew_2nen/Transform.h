#pragma 
#include "Component.h"
template <class T>
class Transform :public Component
{
public:
	Transform() = default;
	~Transform() = default;

	void Update()override
	{
		T* obj = this->m_obj;
	}
	inline void SetPosition(DirectX::XMFLOAT3& _pos) {
		m_position = _pos;
	}
	inline void SetPosition(float _x, float _y, float _z) {
		m_position = DirectX::XMFLOAT3(_x, _y, _z);
	};
	inline void SetRotation(DirectX::XMFLOAT3& _rot) {
		m_rotation = _rot;
	};
	inline void SetRotation(float _x, float _y, float _z) {
		m_rotation = DirectX::XMFLOAT3(_x, _y, _z);
	}
	inline void SetScale(DirectX::XMFLOAT3& _scale) {
		m_scale = _scale;
	}
	inline DirectX::XMFLOAT3 GetPosition() const;
	inline DirectX::XMFLOAT3 GetRotation() const;
	inline DirectX::XMFLOAT3 GetScale() const;

private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMFLOAT3 m_scale;
};

