#pragma once
//=======================================
//�A�C�e���̃w�b�_�[
//
//����:�v���C���[���A�C�e�������ƈႤ�e��łĂ�悤�ɂȂ�
//CGameobject���p������
//=======================================
class CItem:public CGameObject
{
private:
	CModel* m_Model;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	ID3D11ShaderResourceView* m_TextureEnv = NULL;

public:
	CItem() {}    //�R���X�g���N�^
	~CItem() {}  //�f�X�g���N�^

	void Init();
	void Uninit();
	void Update();
	void Draw();
};
