#pragma once
class CFade
{
private:
	static D3DXCOLOR m_Color;
	static float m_Alpha;
	static float m_AddAlpha;
	static bool m_bOut;
	static bool m_bIsFade;

	static ID3D11Buffer* m_VertexBuffer;
	static ID3D11ShaderResourceView* m_Texture;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static void In();
	static void Out();
	static bool IsFade() { return m_bIsFade; }
};
