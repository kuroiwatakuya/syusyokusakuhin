#pragma once
#include "game_object.h"
#include "model.h"
#include "animation_model.h"

class CPlayer:public CGameObject //�p��
{
private:
	CAnimationModel* m_Model;
	int m_Frame;
	float m_BlendRate;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	ID3D11VertexShader* m_VertexShader2;
	ID3D11PixelShader* m_PixelShader2;
	ID3D11InputLayout* m_VertexLayout2;

public:
	
	CPlayer(){}	//�R���X�g���N�^	
	~CPlayer(){} //�f�X�g���N�^

	void Init();
	void Uninit();
	void Update();
	void Draw();
};
