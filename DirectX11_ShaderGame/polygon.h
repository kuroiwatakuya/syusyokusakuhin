#pragma once
#include "game_object.h"
//�|���S���N���X
class CPolygon:public CGameObject
{
private:
	//�����o�ϐ�
	ID3D11Buffer*							m_VertexBuffer = NULL; 
	ID3D11ShaderResourceView*		m_Texture = NULL; 

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

public:
	CPolygon* m_polygon;

	//���\�b�h
	CPolygon() {}
	~CPolygon() {}

	void Init();			//������
	void Uninit();		//�I������
	void Update();	//�X�V����
	void Draw();		//�`�揈��
};
