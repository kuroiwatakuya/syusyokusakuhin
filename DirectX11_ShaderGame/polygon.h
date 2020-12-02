#pragma once
#include "game_object.h"
//ポリゴンクラス
class CPolygon:public CGameObject
{
private:
	//メンバ変数
	ID3D11Buffer*							m_VertexBuffer = NULL; 
	ID3D11ShaderResourceView*		m_Texture = NULL; 

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

public:
	CPolygon* m_polygon;

	//メソッド
	CPolygon() {}
	~CPolygon() {}

	void Init();			//初期化
	void Uninit();		//終了処理
	void Update();	//更新処理
	void Draw();		//描画処理
};
