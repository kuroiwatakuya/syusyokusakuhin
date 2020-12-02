#pragma once
//============================================
//エフェクトクラス
//============================================
class CEffect:public CGameObject
{
private:
	//前方宣言(ポインタ変数の有効)
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
