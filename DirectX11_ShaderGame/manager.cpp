#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"

CScene* CManager::m_Scene = NULL;

//========================================
//������
//========================================
void CManager::Init()
{
	CRenderer::Init();

	//Renderer��Init�̂��ƂɃ|���S�����������Ăяo��
	CInput::Init();

	m_Scene = new CScene();
	m_Scene->Init();

}

//========================================
//�I������
//========================================
void CManager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	CInput::Uninit();
	CRenderer::Uninit();
}

//========================================
//�X�V����
//========================================
void CManager::Update()
{
	CInput::Update();
	m_Scene->Update();
}

//========================================
//�`�揈��
//========================================
void CManager::Draw()
{
	//���Renderer������
	//�J����,3D,2D�̏��Ԃŕ`�悵�Ă�������
	CRenderer::Begin();
	
	//���C�g���g�������Ƃ�
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetLight(light);

	m_Scene->Draw();

	//�Ō��end
	CRenderer::End();
}
