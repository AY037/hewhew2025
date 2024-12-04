#include "TextStream.h"
void TextStream::SaveScene(std::string fileName, const std::unordered_map<int, std::shared_ptr<GameObject>>& tiles)
{
	//ファイルオープン処理
	fout.open(fileName, std::ios::out);
	if (!fout.is_open())
	{
		//ファイルオープンエラー
	}
	else
	{
		for (const auto& iter : tiles)
		{
			auto _tiles = iter.second;
			fout << _tiles->GetName() << " :{" << std::endl;
			fout << "\tPosition: {\n\t\tX: " << _tiles->GetPos().x << "\n\t\tY: " << _tiles->GetPos().y << "\n\t\tZ: " << _tiles->GetPos().z << "\n\t}" << std::endl;
			fout << "\tSize: {\n\t\tX: " << _tiles->GetSize().x << "\n\t\tY: " << _tiles->GetSize().y << "\n\t\tZ: " << _tiles->GetSize().z << "\n\t}" << std::endl;
			fout << "\tAngle: {\n\t\tX: " << 0.0f << "\n\t\tY: " << 0.0f << "\n\t\tZ: " << _tiles->GetAngle() << "\n\t}" << std::endl;
			fout << "};" << std::endl;
		}
	}
	//ファイルクローズ処理
	fout.close();
}
void TextStream::LoadScene(std::string fileName, std::unordered_map<int, std::shared_ptr<GameObject>>& tiles)
{
	std::string readBuffer;//読み出しデータ保存用
	fin.open(fileName, std::ios::in);

	if (!fin.is_open())
	{
		//ファイルオープンエラー
	}
	else
	{
		std::string tileName;
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 size;
		DirectX::XMFLOAT3 angle;

		bool nt = false;
		bool ot = false;
		//ファイルからの読み出し
		while (!fin.eof())
		{
			std::string tempBuffer;//一時読み出し用
			tempBuffer.clear();//バッファクリア
			fin >> tempBuffer;//一行分のテータ読み出し

			if (nt == false && ot == false)
			{
				tileName = tempBuffer;
				nt = true;
			}

			if (tempBuffer == "Position:")
			{
				while (tempBuffer != "}")
				{
					fin >> tempBuffer;//一行分のテータ読み出し
					if (tempBuffer == "X:")
					{
						fin >> tempBuffer;
						pos.x = std::stoi(tempBuffer);
					}
					if (tempBuffer == "Y:")
					{
						fin >> tempBuffer;
						pos.y = std::stoi(tempBuffer);
					}
					if (tempBuffer == "Z:")
					{
						fin >> tempBuffer;
						pos.z = std::stoi(tempBuffer);
					}
				}
			}
			if (tempBuffer == "Size:")
			{
				while (tempBuffer != "}")
				{
					fin >> tempBuffer;//一行分のテータ読み出し
					if (tempBuffer == "X:")
					{
						fin >> tempBuffer;
						size.x = std::stoi(tempBuffer);
					}
					if (tempBuffer == "Y:")
					{
						fin >> tempBuffer;
						size.y = std::stoi(tempBuffer);
					}
					if (tempBuffer == "Z:")
					{
						fin >> tempBuffer;
						size.z = std::stoi(tempBuffer);
					}
				}
			}
			if (tempBuffer == "Angle:")
			{
				while (tempBuffer != "}")
				{
					fin >> tempBuffer;//一行分のテータ読み出し
					if (tempBuffer == "X:")
					{
						fin >> tempBuffer;
						angle.x = std::stoi(tempBuffer);
					}
					if (tempBuffer == "Y:")
					{
						fin >> tempBuffer;
						angle.y = std::stoi(tempBuffer);
					}
					if (tempBuffer == "Z:")
					{
						fin >> tempBuffer;
						angle.z = std::stoi(tempBuffer);
					}
				}
			}
			if (tempBuffer == "};")
			{
				std::shared_ptr<GameObject> tmpObj = gom.GetObj(tileName);
				tmpObj->SetName(tileName);
				tmpObj->SetPos(pos.x, pos.y, pos.z);
				tmpObj->SetSize(size.x, size.y, size.z);
				tmpObj->SetAngle(angle.z);
				int objID = tiles.size();
				tmpObj->SetObjID(objID);
				tiles[objID] = tmpObj;

				nt = false;
				ot = false;
			}
		}

	}

	//ファイルクローズ処理
	fin.close();
}