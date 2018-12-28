#include "DXUT.h"
#include "DXUTmisc.h"
#include "TexGenerator.h"
#include "TextureA1.h"
#include "SDKmisc.h"

VTGenerator::VTGenerator(IDirect3DDevice9* pD3DDevice)
{
	
	pDevice = pD3DDevice;

	Init();

	InitTex();

}

void VTGenerator::InitTex()
{
	D3DXCreateTextureFromFileA(pDevice, "tex/mask1.tga", &masktex1);
	D3DXCreateTextureFromFileA(pDevice, "tex/mask2.tga", &masktex2);

	D3DXCreateTextureFromFileA(pDevice, "tex/T_cliff_a_d.tga", &brock);
	D3DXCreateTextureFromFileA(pDevice, "tex/T_birch_forest_a_d.tga", &bforest);
	D3DXCreateTextureFromFileA(pDevice, "tex/T_grass_a_d.tga", &grass);
	D3DXCreateTextureFromFileA(pDevice, "tex/T_ground_a_d.tga", &ground);
	D3DXCreateTextureFromFileA(pDevice, "tex/T_forest_a_d.tga", &pine);
	D3DXCreateTextureFromFileA(pDevice, "tex/T_ground_rocks_a_d.tga", &srock);

	WCHAR str[MAX_PATH];
	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
	DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"vt.fx");
	D3DXCreateEffectFromFile(pDevice, str, NULL, NULL, dwShaderFlags,
		NULL, &pTexEffect, NULL);

	D3DXHANDLE hmask1 = pTexEffect->GetParameterByName(NULL, "mask1");
	pTexEffect->SetTexture(hmask1, masktex1);

	D3DXHANDLE hmask2 = pTexEffect->GetParameterByName(NULL, "mask2");
	pTexEffect->SetTexture(hmask2, masktex2);

	D3DXHANDLE hbrock = pTexEffect->GetParameterByName(NULL, "brock");
	pTexEffect->SetTexture(hbrock, brock);

	D3DXHANDLE hbforest = pTexEffect->GetParameterByName(NULL, "bforest");
	pTexEffect->SetTexture(hbforest, bforest);

	D3DXHANDLE hgrass = pTexEffect->GetParameterByName(NULL, "grass");
	pTexEffect->SetTexture(hgrass, grass);

	D3DXHANDLE hground = pTexEffect->GetParameterByName(NULL, "ground");
	pTexEffect->SetTexture(hground, ground);

	D3DXHANDLE hpine = pTexEffect->GetParameterByName(NULL, "pine");
	pTexEffect->SetTexture(hpine, pine);

	D3DXHANDLE hsrock = pTexEffect->GetParameterByName(NULL, "srock");
	pTexEffect->SetTexture(hsrock, srock);

	g_hmWorldViewProjection = pTexEffect->GetParameterByName(NULL, "g_mWorldViewProjection");


}

void VTGenerator::DrawFullScreenQuad()
{
	struct QuadVertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 uv;
	};

	QuadVertex v[4];

	v[0].pos = D3DXVECTOR3(-512.0f,0.0f, -512.0f);
	v[0].uv = D3DXVECTOR2(0.0f, 1.0f);

	v[1].pos = D3DXVECTOR3(-512.0f, 0.0f, 512.0f);
	v[1].uv = D3DXVECTOR2(0.0f, 0.0f);

	v[2].pos = D3DXVECTOR3(512.0f, 0.0f, -512.0f);
	v[2].uv = D3DXVECTOR2(1.0f, 1.0f);

	v[3].pos = D3DXVECTOR3(512.0f, 0.0f, 512.0f);
	v[3].uv = D3DXVECTOR2(1.0f, 0.0f);


	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void*)v, sizeof(QuadVertex));

}

void VTGenerator::Init()
{
	pDevice->CreateTexture(4096, 4096, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &TextureCache, NULL);
	pDevice->CreateTexture(128, 128, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pRT, NULL);

	pRT->GetSurfaceLevel(0, &pNewRT);
	pDevice->CreateDepthStencilSurface(128, 128, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, true, &pNewDS, NULL);

	//TextureCache->GetSurfaceLevel(0, &pNewRT);
	//pDevice->CreateDepthStencilSurface(4096, 4096, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, true, &pNewDS, NULL);
	
	pOrinRT = nullptr;
	pOrinDS = nullptr;
}


void VTGenerator::Begin()
{
	if (pOrinRT == nullptr)
	{
		pDevice->GetRenderTarget(0, &pOrinRT);
		pDevice->GetDepthStencilSurface(&pOrinDS);
	}

	pDevice->SetRenderTarget(0, pNewRT);
	pDevice->SetDepthStencilSurface(pNewDS);

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
}

void VTGenerator::Begin(IDirect3DTexture9* ptex)
{
	if (pOrinRT == nullptr)
	{
		pDevice->GetRenderTarget(0, &pOrinRT);
		pDevice->GetDepthStencilSurface(&pOrinDS);
	}

	ptex->GetSurfaceLevel(0, &pNewRT);
	pDevice->SetRenderTarget(0, pNewRT);
	pDevice->SetDepthStencilSurface(pNewDS);

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	SAFE_RELEASE(pNewRT);

}

void VTGenerator::TestupdateTexture(int textadr, IDirect3DTexture9* ptex)
{

	int levelmask = 0xff000000;
	int level = (textadr&levelmask) >> 24;

	int bias = textadr & 0x00ffffff;

	int xbias = bias % 4096;
	int ybias = bias / 4096;

	float level0size = 1.0f;
	float levelsize = 1.0f * (1<<level) ;
	float halfesize = levelsize / 2.0f;

	Begin(ptex);

	D3DXMATRIX mview;
	D3DXMATRIX mproj;
	D3DXMATRIX mvp;

	float centerx = -512.0f + halfesize + xbias * levelsize;
	float centery = -512.0f + halfesize + ybias * levelsize;

	D3DXVECTOR3 veye = D3DXVECTOR3(centerx, 10.0f, centery);
	D3DXVECTOR3 vat = D3DXVECTOR3(centerx, 0.0f, centery);
	D3DXVECTOR3 vup = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXMatrixLookAtLH(&mview, &veye, &vat, &vup);

	D3DXMatrixOrthoLH(&mproj, levelsize, levelsize, 1.0f, 50.0f);

	mvp = mview * mproj;

	pTexEffect->SetMatrix(g_hmWorldViewProjection, &mvp);

	pTexEffect->Begin(nullptr, 0);
	pTexEffect->BeginPass(0);

	DrawFullScreenQuad();

	pTexEffect->EndPass();
	pTexEffect->End();
	//
	End();

}

void VTGenerator::saveTexture()
{
	

}

void VTGenerator::updateTexture(int texpage, int textadr)
{

	

	int levelmask = 0xff000000;
	int level = (textadr&levelmask) >> 24;

	int bias = textadr & 0x00ffffff;

	int xbias = bias % 4096;
	int ybias = bias / 4096;

	float level0size = 1.0f;
	float levelsize = 1.0f * (1 << level);
	float halfesize = levelsize / 2.0f;

	Begin();

	D3DXMATRIX mview;
	D3DXMATRIX mproj;
	D3DXMATRIX mvp;

	float centerx = -512.0f + halfesize + xbias * levelsize;
	float centery = -512.0f + halfesize + ybias * levelsize;

	D3DXVECTOR3 veye = D3DXVECTOR3(centerx, 10.0f, centery);
	D3DXVECTOR3 vat = D3DXVECTOR3(centerx, 0.0f, centery);
	D3DXVECTOR3 vup = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXMatrixLookAtLH(&mview, &veye, &vat, &vup);

	D3DXMatrixOrthoLH(&mproj, levelsize, levelsize, 1.0f, 50.0f);

	mvp = mview * mproj;

	pTexEffect->SetMatrix(g_hmWorldViewProjection, &mvp);

	pTexEffect->Begin(nullptr, 0);
	pTexEffect->BeginPass(0);

	DrawFullScreenQuad();

	pTexEffect->EndPass();
	pTexEffect->End();
	//
	End();


	//stretch texture to the cacheTexture
	
	IDirect3DSurface9* psurf;
	TextureCache->GetSurfaceLevel(0,&psurf);

	int biasx = texpage %  32;
	int biasy = texpage / 32;

	RECT rect;
	rect.left	= 0 + biasx*128;
	rect.bottom	= 4096 -  biasy * 128;
	//rect.bottom = 0 + biasy * 128;

	rect.right	= 128 + biasx * 128;
	rect.top = 4096 - 128 - biasy * 128;
	//rect.top = 128 + biasy * 128;
		
	pDevice->StretchRect(pNewRT,NULL, psurf, &rect, D3DTEXF_LINEAR);
		
}


IDirect3DTexture9* VTGenerator::getTex()
{
	return TextureCache;
}

void VTGenerator::End()
{

	pDevice->SetRenderTarget(0, pOrinRT);
	pDevice->SetDepthStencilSurface(pOrinDS);

}


