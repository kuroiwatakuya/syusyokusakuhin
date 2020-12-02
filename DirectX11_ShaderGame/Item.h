#pragma once
//=======================================
//アイテムのヘッダー
//
//説明:プレイヤーがアイテムを取ると違う弾を打てるようになる
//CGameobjectを継承する
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
	CItem() {}    //コンストラクタ
	~CItem() {}  //デストラクタ

	void Init();
	void Uninit();
	void Update();
	void Draw();
};
