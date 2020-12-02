#pragma once
class CGameObject
{
protected:		//アクセス指定子
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation; 
	D3DXVECTOR3 m_Scale;

	bool m_Destoroy = false;

public:
	CGameObject(){}
	virtual ~CGameObject(){}	//virtualを入れないと継承先でUninitが呼ばれない

	virtual void Init() = 0;		//純粋仮想関数(仮想関数にするとInitの中に機能を入れなければならない)
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//ポジションのゲッター
	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	D3DXVECTOR3 GetScale() { return m_Scale; }

	//ベクトル取得
	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);	

		D3DXVECTOR3 forward;
		forward.x = rot._31;	//_数字は要素名
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	//ポジションのセッター
	void SetPosition(D3DXVECTOR3 Position) { m_Position = Position; }
	void SetRotation(D3DXVECTOR3 Rotation) { m_Rotation = Rotation; }
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }

	void SetDestoroy()
	{
		m_Destoroy = true;
	}

	bool Destoroy()
	{
		if (m_Destoroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}
};
