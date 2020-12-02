#pragma once
class CGameObject
{
protected:		//�A�N�Z�X�w��q
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation; 
	D3DXVECTOR3 m_Scale;

	bool m_Destoroy = false;

public:
	CGameObject(){}
	virtual ~CGameObject(){}	//virtual�����Ȃ��ƌp�����Uninit���Ă΂�Ȃ�

	virtual void Init() = 0;		//�������z�֐�(���z�֐��ɂ����Init�̒��ɋ@�\�����Ȃ���΂Ȃ�Ȃ�)
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//�|�W�V�����̃Q�b�^�[
	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	D3DXVECTOR3 GetScale() { return m_Scale; }

	//�x�N�g���擾
	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);	

		D3DXVECTOR3 forward;
		forward.x = rot._31;	//_�����͗v�f��
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	//�|�W�V�����̃Z�b�^�[
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
