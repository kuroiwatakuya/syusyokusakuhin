#pragma once
#include "game_object.h"
//========================================
//�J�����N���X
//========================================
class CCamera:public CGameObject
{
private:
	D3DXVECTOR3 m_Target;	//�����_

	D3DXMATRIX m_Matrix;	
	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjectionMatrix;

public:
	CCamera() {};
	~CCamera() {};
	
	void Init();			//����������
	void Uninit();		//�I������
	void Update();	//�X�V����
	void Draw();		//�`�揈��
	
	D3DXMATRIX GetViewMatrix() { return m_Matrix; }
};
