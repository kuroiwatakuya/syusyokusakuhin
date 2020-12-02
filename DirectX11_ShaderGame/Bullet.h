#pragma once
#include "game_object.h"
//==========================================
//Bullet.h
//==========================================
class CBullet :public CGameObject
{
private:
	static class CModel* m_Model;
	//前方宣言(ポインタ変数の有効)

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;

public:
	CBullet() {}
	~CBullet() {}

	static void Load();
	static void UnLoad();

	void Init();
	void Uninit();
	void Update();
	void Draw();
};
