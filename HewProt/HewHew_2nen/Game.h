#pragma once
#include "SceneManager.h"
/// <summary>
/// �Q�[���S�̂��Ǘ�����N���X�ł��B
/// �Q�[�����[�v�̐����A��v�ȃQ�[���I�u�W�F�N�g�̏������E�X�V�E�I���������s���܂��B
/// </summary>
class Game{

private:
	SceneManager& sceneManager = SceneManager::GetInstance();// �V�[�����Ǘ����邽�߂�

	bool isRunning;// �Q�[�������s�����ǂ����𔻒肷��t���O�B

	Input& input = Input::GetInstance();
public:
	Game() {
		isRunning = false;
	}
	~Game(){}
	void Init(HWND hWnd); //������
	void Update(void);    //�X�V
	void Draw();          //�`��
	void Uninit();        //�I��
};