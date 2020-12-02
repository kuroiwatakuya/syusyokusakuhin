#include "main.h"
#include "renderer.h"
#include "Field.h"

//========================================
//初期化
//========================================
void CField::Init()
{
	VERTEX_3D vertex[4];

	//3D空間上の座標を定義する
	vertex[0].Position = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//法線方向は上に向かせるのでyに1を入れる
	vertex[0].Binormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertex[0].Tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Binormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertex[1].Tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(70.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Binormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertex[2].Tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 70.0f);

	vertex[3].Position = D3DXVECTOR3(100.0f, 0.0f, -100.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Binormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertex[3].Tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(70.0f, 70.0f);

	//頂点バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(
		CRenderer::GetDevice(),
		"asset/texture/ConcreteGeometric3_512_albedo.tif",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	//テクスチャが読み込まれなかった場合エラーを出す処理
	assert(m_Texture);

	//法線マップ読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/ConcreteGeometric3_512_normal.tif",
		NULL,
		NULL,
		&m_TextureNormal,
		NULL);
	assert(m_TextureNormal);

	//ハイトマップ読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/ConcreteGeometric3_512_height.tif",
		NULL,
		NULL,
		&m_TextureHeight,
		NULL);
	assert(m_TextureHeight);

	m_Position		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "parallaxocclusionVS.cso");
	CRenderer::CreatePixelShader(&m_PixelShader, "parallaxocclusionPS.cso");
}

//========================================
//終了処理
//========================================
void CField::Uninit()
{
	//テクスチャは晴れてると思います
	//多分そうです
	//Create~で作った変数はリリースする必要がある
	//スマートポインタ―とかいれればいちいちリリースする必要はなくなる
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_TextureNormal->Release();
	m_TextureHeight->Release();

	m_VertexLayout->Release();
	m_PixelShader->Release();
	m_VertexShader->Release();
}

//========================================
//更新処理
//========================================
void CField::Update()
{

}

//========================================
//描画処理
//========================================
void CField::Draw()
{

	//入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);

	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);	//拡大縮小
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z); //回転のマトリックス(設定はy,x,zの順番)
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);	//座標
	world = scale * rot * trans;	//3つの合成(普通の掛け算ではない)
	CRenderer::SetWorldMatrix(&world);

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
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_TextureNormal);		//第一引数:2番目のテクスチャーとして登録する 第二引数:配列番号
	CRenderer::GetDeviceContext()->PSSetShaderResources(2, 1, &m_TextureHeight);

	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	CRenderer::GetDeviceContext()->Draw(4, 0);	//今回は頂点は4つなので4
}