#pragma once
class CTower :public CGameObject
{
private:
	CModel* m_Model;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	ID3D11ShaderResourceView* m_Texture;							//アルベドテクスチャ
	ID3D11ShaderResourceView* m_TextureNormal;					//法線マップテクスチャ
	ID3D11ShaderResourceView* m_TextureMetall;					//メタリックテクスチャ
	ID3D11ShaderResourceView* m_TextureRoughness;			//ラフネステクスチャ

public:
	CTower() {}		//コンストラクタ
	~CTower() {}		//デストラクタ


	void Init();
	void Uninit();
	void Update();
	void Draw();
};