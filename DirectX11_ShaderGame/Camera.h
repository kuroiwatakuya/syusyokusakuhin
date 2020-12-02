#pragma once
#include "game_object.h"
//========================================
//カメラクラス
//========================================
class CCamera:public CGameObject
{
private:
	D3DXVECTOR3 m_Target;	//注視点

	D3DXMATRIX m_Matrix;	
	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjectionMatrix;

public:
	CCamera() {};
	~CCamera() {};
	
	void Init();			//初期化処理
	void Uninit();		//終了処理
	void Update();	//更新処理
	void Draw();		//描画処理
	
	D3DXMATRIX GetViewMatrix() { return m_Matrix; }
};
