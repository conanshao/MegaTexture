#include "DXUT.h"
#include "terrainMesh.h"


TerrainMesh::TerrainMesh()
{

}

TerrainMesh::~TerrainMesh()
{

}


void TerrainMesh::InitBuffer(int terrainsize,float deltasize,int &facecount,int &vertexcount,IDirect3DVertexBuffer9** ppvb, IDirect3DIndexBuffer9** ppib)
{

	vertexcount = terrainsize * terrainsize;
	facecount = (terrainsize - 1) * (terrainsize - 1) * 2;

	device->CreateVertexBuffer(vertexcount * sizeof(TerrainVertex), 0, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_MANAGED, ppvb, NULL);
	device->CreateIndexBuffer(facecount * 3 * sizeof(DWORD), 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, ppib, NULL);


	TerrainVertex* vertexdata;
	IDirect3DVertexBuffer9* vb = *ppvb;
	IDirect3DIndexBuffer9* ib = *ppib;

	vb->Lock(0, 0, (void**)&vertexdata, D3DLOCK_DISCARD);
	for (int i = 0; i < terrainsize; i++)
		for (int j = 0; j < terrainsize; j++)
		{
			vertexdata[i*terrainsize + j].pos = D3DXVECTOR3((i - terrainsize / 2)*deltasize, 0.0f, (j - terrainsize / 2)*deltasize);
			vertexdata[i*terrainsize + j].tex = D3DXVECTOR2(float(i) / terrainsize, float(j) / terrainsize);

		}
	vb->Unlock();

	DWORD* indexdata;
	ib->Lock(0, 0, (void**)&indexdata, D3DLOCK_DISCARD);

	int index = 0;
	for (int i = 0; i < terrainsize - 1; i++)
		for (int j = 0; j < terrainsize - 1; j++)
		{
			indexdata[index++] = i * terrainsize + j;

			indexdata[index++] = (i + 1) * terrainsize + (j + 1);
			indexdata[index++] = (i + 1) * terrainsize + j;

			indexdata[index++] = i * terrainsize + j;
			indexdata[index++] = i * terrainsize + (j + 1);
			indexdata[index++] = (i + 1) * terrainsize + j + 1;
		}
	ib->Unlock();

	
}

void TerrainMesh::Init(IDirect3DDevice9* pDevice)
{
	device = pDevice;

	float deltasize = 0.5f;

	InitBuffer(2048, deltasize, facecount, vertexcount, &vertexbuffer, &indexbuffer);
	
	InitBuffer(256, deltasize*8.0f,  lowfacecount, lowvertexcount, &lowvb, &lowib);

}


void TerrainMesh::Render()
{

	device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);

	device->SetStreamSource(0, vertexbuffer, 0, sizeof(TerrainVertex));

	device->SetIndices(indexbuffer);
	/*
	for(int i=0;i<2;i++)
		for (int j = 0; j<2; j++)
	{
		D3DXVECTOR3 posbiasscale = D3DXVECTOR3( i*512.0f-256.0f, j*512.0f - 256.0f,  0.5f);
		D3DXVECTOR3 UVbiasscale = D3DXVECTOR3( i*0.5f, j*0.5f,  0.5f);


		device->SetVertexShaderConstantF(0, (const float*)&posbiasscale, 1);
		device->SetVertexShaderConstantF(1, (const float*)&UVbiasscale, 1);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexcount, 0, facecount);
	}
	*/
	/**/
	//D3DXVECTOR3 posbiasscale = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//D3DXVECTOR3 UVbiasscale = D3DXVECTOR3(0.0f, 0.0f, 1.0f);


	//device->SetVertexShaderConstantF(0, (const float*)&posbiasscale, 1);
	//device->SetVertexShaderConstantF(1, (const float*)&UVbiasscale, 1);

	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexcount, 0, facecount); 
	
}

void TerrainMesh::Renderlow()
{

	device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);

	device->SetStreamSource(0, lowvb, 0, sizeof(TerrainVertex));

	device->SetIndices(lowib);

	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, lowvertexcount, 0, lowfacecount);

}


void TerrainMesh::Shut()
{

	//
	SAFE_RELEASE(vertexbuffer);
	SAFE_RELEASE(indexbuffer);

	SAFE_RELEASE(lowvb);
	SAFE_RELEASE(lowib);

}
