#pragma once


class TextureAl;
class VTGenerator
{
private:
	IDirect3DTexture9* TextureCache;

	IDirect3DTexture9* pRT;

	IDirect3DDevice9* pDevice;

	IDirect3DSurface9* pOrinRT;
	IDirect3DSurface9* pOrinDS;

	IDirect3DSurface9* pNewRT;
	IDirect3DSurface9* pNewDS;

private:

	void Init();
	TextureAl *ptexa1;

	IDirect3DTexture9* masktex1 = NULL;
	IDirect3DTexture9* masktex2 = NULL;

	IDirect3DTexture9* brock = NULL;
	IDirect3DTexture9* bforest = NULL;
	IDirect3DTexture9* grass = NULL;
	IDirect3DTexture9* ground = NULL;

	IDirect3DTexture9* pine = NULL;
	IDirect3DTexture9* srock = NULL;

	void InitTex();

	void DrawFullScreenQuad();

	IDirect3DVertexBuffer9* pvb;
	IDirect3DIndexBuffer9* pib;

	ID3DXEffect * pTexEffect;

	D3DXHANDLE                  g_hmWorldViewProjection;
	D3DXHANDLE                  g_hmWorld;
	D3DXHANDLE                  g_hfTime;

public:


	inline IDirect3DTexture9 * getDebugTex();
	IDirect3DTexture9 * getTex();

	VTGenerator(IDirect3DDevice9* pD3DDevice);

	void Begin();

	void Begin(IDirect3DTexture9* ptex);

	void updateTexture(int texpage, int texadr);

	void TestupdateTexture(int textadr,IDirect3DTexture9* ptex);

	void End();

	void saveTexture();


};


inline IDirect3DTexture9 * VTGenerator::getDebugTex()
{
	return pRT;
}
