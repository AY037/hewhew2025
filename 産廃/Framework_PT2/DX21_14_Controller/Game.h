#pragma once
#include "SceneManager.h"
/// <summary>
/// �Q�[���S�̂��Ǘ�����N���X�ł��B
/// �Q�[�����[�v�̐����A��v�ȃQ�[���I�u�W�F�N�g�̏������E�X�V�E�I���������s���܂��B
/// </summary>
class Game{

private:
	SceneManager* sceneManager;// �V�[�����Ǘ����邽�߂̃I�u�W�F�N�g�B

	bool isRunning;// �Q�[�������s�����ǂ����𔻒肷��t���O�B

	Input& input = Input::GetInstance();
public:
	Game() {
		isRunning = false;
		sceneManager= nullptr;
	}
	~Game(){}
	void Init(HWND hWnd); //������
	void Update(void);    //�X�V
	void Draw();          //�`��
	void Uninit();        //�I��
};