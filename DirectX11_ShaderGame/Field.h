#pragma once
#include "game_object.h"
//========================================
//フィールド
//※今後同じ役割の変数などは同じ名前にすること
//========================================
class CField:public CGameObject
{
private:
	ID3D11Buffer*							m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*		m_Texture = NULL;

	ID3D11ShaderResourceView* m_TextureNormal ;			//法線マップテクスチャ
	ID3D11ShaderResourceView* m_TextureHeight;			//ハイトマップテクスチャ

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	
public:
	
	void Init();			//初期化
	void Uninit();		//終了処理
	void Update();	//更新処理
	void Draw();		//描画処理
};
