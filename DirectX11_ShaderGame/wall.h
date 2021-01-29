#pragma once

class CWall :public CGameObject
{
private:
	CModel* m_Model;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	ID3D11ShaderResourceView* m_Texture;							//�e�N�X�`��
	ID3D11ShaderResourceView* m_TextureNormal;					//�@���}�b�v�e�N�X�`��
	ID3D11ShaderResourceView* m_TextureHeight;

public:
	CWall() {}		//�R���X�g���N�^
	~CWall() {}		//�f�X�g���N�^


	void Init();
	void Uninit();
	void Update();
	void Draw();
};
