#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "game_object.h"
#include "fade.h"

D3DXCOLOR CFade::m_Color(0.0f, 0.0f, 0.0f, 1.0f);
float CFade::m_Alpha = 0.0f;
float CFade::m_AddAlpha = 0.0f;
bool  CFade::m_bOut = false;
bool  CFade::m_bIsFade = false;

ID3D11Buffer* CFade::m_VertexBuffer = NULL;
ID3D11ShaderResourceView* CFade::m_Texture = NULL;

void CFade::Init() 
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(0.0f, 0.0f, 0.0f, m_Alpha);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(0.0f, 0.0f, 0.0f, m_Alpha);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(0.0f, 0.0f, 0.0f, m_Alpha);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(0.0f, 0.0f, 0.0f, m_Alpha);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));				//ZeroMemoryとは、構造体の中身をゼロにする関数。
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/black.jpg",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);

	m_bIsFade = false;
	m_Alpha = 0.0f;
}

void CFade::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}

void CFade::Update() 
{

	if (!m_bIsFade) return;

	m_Alpha += m_AddAlpha;

	if (m_bOut) {
		if (m_Alpha >= 1.0f) {
			m_Alpha = 1.0f;
			m_bIsFade = false;
		}
	}
	else {
		if (m_Alpha <= 0.0f) {
			m_Alpha = 0.0f;
			m_bIsFade = false;
		}
	}
}

void CFade::Draw() 
{

	if (m_Alpha == 0.0f) return;

	LIGHT light;
	light.Enable = false;
	CRenderer::SetLight(light);

	m_Color.a = m_Alpha;

	D3D11_MAPPED_SUBRESOURCE msr;
	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(0.0f, 0.0f, 0.0f, m_Alpha);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(0.0f, 0.0f, 0.0f, m_Alpha);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(0.0f, 0.0f, 0.0f, m_Alpha);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(0.0f, 0.0f, 0.0f, m_Alpha);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	//マトリックス設定
	CRenderer::SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Alpha);
	CRenderer::SetMaterial(material);

	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定(頂点の結び方)
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	CRenderer::GetDeviceContext()->Draw(4, 0);
}

void CFade::In() 
{
	m_bOut = true;
	m_AddAlpha = 0.01f;
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_bIsFade = true;

	m_Alpha = 0.0f;
}

void CFade::Out()
{
	m_bOut = false;
	m_AddAlpha = -0.02f;
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_bIsFade = true;

	m_Alpha = 1.0f;
}