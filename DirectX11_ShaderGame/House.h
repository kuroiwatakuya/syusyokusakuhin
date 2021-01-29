#pragma once

class CHouse :public CGameObject
{
private:
	CModel* m_Model;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

public:
	CHouse() {}		//�R���X�g���N�^
	~CHouse() {}		//�f�X�g���N�^


	void Init();
	void Uninit();
	void Update();
	void Draw();
};

