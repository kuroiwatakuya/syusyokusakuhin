#pragma once
//============================================
//�G�t�F�N�g�N���X
//============================================
class CEffect:public CGameObject
{
private:
	//�O���錾(�|�C���^�ϐ��̗L��)
	ID3D11Buffer* m_VertexBuffer;
	ID3D11ShaderResourceView* m_Texture;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	int m_Count;

public:

	static void Load();
	static void UnLoad();

	void Init();
	void Uninit();
	void Update();
	void Draw();
};
