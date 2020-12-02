#pragma once
#include "main.h"
#include "model.h"
#include "game_object.h"

class CEnemy:public CGameObject
{
private:
	CModel* m_Model;
	//前方宣言(ポインタ変数の有効)

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

public:
	CEnemy(){}
	~CEnemy(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();
};
