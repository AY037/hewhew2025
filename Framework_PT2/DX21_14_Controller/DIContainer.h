#pragma once
#include <map>
#include <type_traits>
#include <concepts>
#include <functional>
#include "Game.h"
//未実装オーバーエンジニアリングかもだから、やるかわからない
class DIContainer
{
	using Provider = std::function<void* (DIContainer&)>;//DIContainer（生成関数）を受け取り生成したオブジェクトのポインタを返す
	using TypeId = size_t;
public:
	void Init();
	// Keyの生成設定を登録する関数
	template<typename Key, typename T>//autoの最強版マジでなんでも入る
	void Register()
	{
		Register<Key>([](auto c) { return new T; });
	}

	// Keyの生成設定を登録する関数
	template<typename Key>
	void Register(Provider&& func)//Register関数のオーバーロード、引数で判別
	{
		TypeId typeId = typeid(Key).hash_code();//生成関数に識別IDを割り当て
		TypeId hasType = m_providerMap.find(typeId) != m_providerMap.end();//mapからtypeIdに対応する要素があるか探す

		// 二重登録は不可
		if (hasType == true)
		{
			printf("DIContainer: 既に登録されたクラスです\n");
			return;
		}

		m_providerMap[typeId] = func;
	}

	// Keyから実際にオブジェクトを取得する関数
	template<typename Key>
	Key* Resolve()
	{
		TypeId typeId = typeid(Key).hash_code();
		TypeId hasType = m_providerMap.find(typeId) != m_providerMap.end();


		if (hasType == false)
		{
			printf("DIContainer: 既に登録されたクラスです\n");
			return nullptr;
		}

		return static_cast<Key*>(m_providerMap[typeId](*this));//void*からkey*にキャスト
	}

private:

	// 生成クラスIdとオブジェクトの生成関数へのマップ
	std::map<TypeId, Provider> m_providerMap;//生成関数(Provider)とTypeIdを紐づけTypeIDから高速に検索できる
};
