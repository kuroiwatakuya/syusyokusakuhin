#pragma once
#include "main.h"
#include "model.h"
#include "game_object.h"

class CEnemy:public CGameObject
{
private:
	CModel* m_Model;
	//�O���錾(�|�C���^�ϐ��̗L��)

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	ID3D11ShaderResourceView* m_Texture;							//�A���x�h�e�N�X�`��
	ID3D11ShaderResourceView* m_TextureNormal;					//�@���}�b�v�e�N�X�`��
	ID3D11ShaderResourceView* m_TextureMetall;					//���^���b�N�e�N�X�`��
	ID3D11ShaderResourceView* m_TextureRoughness;			//���t�l�X�e�N�X�`��

public:
	CEnemy(){}
	~CEnemy(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();
};
