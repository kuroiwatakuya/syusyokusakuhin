#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "scene.h"
#include "game_object.h"
#include "Fade.h"
#include "game.h"
#include "title.h"
#include <math.h>

static bool g_bEnd = false;
#define KEYMAX 1

struct KEY
{
	float PositionX;
	float PositionY;
	float VelocityX;
	float VelocityY;
	float Rasius;
	float Mass;
};

struct  KEY_ANIME
{
	int flame;
	D3DXVECTOR2 pos;
};

float g_Gravity = 9.8f * 10.0f;

KEY g_key[KEYMAX];
KEY_ANIME g_Anim[] = {
	{60,                    // 次のキーまでのフレーム数
								{ 1000.0f, 1000.0f}   //  キーフレームデータ
							},
							{60,
								{ 1200.0f, 1200.0f}
							},
							{60,
								{ 800.0f, 1040.0f}
							},
							{60,
								{ 2000.0f, 720.0f}
							},
							{60,
								{ 2400.0f, 500.0f}
							},
							{60,
								{ 1980.0f, 720.0f}
							},
							{60,
								{ 1980, 720.0f}
							},
};
int g_KeyMax;
float g_MotionTime = 0.0f;


//===============================================
//タイトル初期化処理
//===============================================
void CTitle::Init()
{
	g_MotionTime = 0.0f;
	g_KeyMax = sizeof(g_Anim) / sizeof(KEY_ANIME);
	g_bEnd = false;

	VERTEX_3D vertex[4];
	VERTEX_3D vertex2[4];

	//タイトル
	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(
		CRenderer::GetDevice(),
		"asset/texture/Title.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	/*m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);*/

	g_MotionTime = 0;
	
	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "pixelLightingVS.cso");

	CRenderer::CreatePixelShader(&m_PixelShader, "pixelLightingPS.cso");
}
//===============================================
//タイトル終了処理
//===============================================
void CTitle::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
	
	m_VertexLayout->Release();
	m_PixelShader->Release();
	m_VertexShader->Release();
}
//===============================================
//タイトル更新処理
//===============================================
void CTitle::Update()
{
	PAINTSTRUCT ps;
	
	float dt = 0.01f;
	int i = (int)g_MotionTime;//iに全体アニメ時間の整数部分を代入

	
	/*if (i > g_KeyMax - 2)
	{
		i = g_KeyMax - 2;
	}*/
	if (i >= 6)
	{
		i = 0;
	}
	else
	{
		dt = 1.0f / g_Anim[i].flame;

		g_key[0].PositionX = g_Anim[i].pos.x
			+ (g_Anim[i + 1].pos.x - g_Anim[i].pos.x)
			* (g_MotionTime - i);

		g_key[0].PositionY = g_Anim[i].pos.y
			+ (g_Anim[i + 1].pos.y - g_Anim[i].pos.y)
			* (g_MotionTime - i);

		g_MotionTime += dt;

	}
	
	if (g_MotionTime > g_KeyMax - 1.0f)
	{
		g_MotionTime = g_KeyMax - 1.0f;
	}
	
	m_Position.x = g_Anim[i].pos.x;
	m_Position.y = g_Anim[i].pos.y;

	CScene::Update();
	if (!g_bEnd)
	{
		if (CInput::GetKeyTrigger(VK_SPACE))
		{
			CFade::In();
			g_bEnd = true;
		}
	}
	else
	{
		if (!CFade::IsFade())
		{
			CFade::Out();
			CManager::SetScene<CGame>();
		}
	}
}
//===============================================
//タイトル描画処理
//===============================================
void CTitle::Draw()
{
	D3D11_MAPPED_SUBRESOURCE msr;

	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(0.0, 0.0, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(m_Position.x, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, m_Position.y, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(m_Position.x, m_Position.y, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);

	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);	//拡大縮小
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z); //回転のマトリックス(設定はy,x,zの順番)
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);	//座標
	world = scale * rot * trans;	//3つの合成(普通の掛け算ではない)
	CRenderer::SetWorldMatrix(&world);

	////ライト無効
	//LIGHT light;
	//light.Enable = false;
	//CRenderer::SetLight(light);

	//マトリクス設定
	//11では2Dポリゴン描画はいちいち設定しなければならない
	CRenderer::SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	
	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	CRenderer::GetDeviceContext()->Draw(4, 0);	//今回は頂点は4つなので4
}