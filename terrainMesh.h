#pragma once


class TerrainMesh
{
public:

	struct TerrainVertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 tex;
	};

	IDirect3DDevice9 * device;
	IDirect3DVertexBuffer9 * vertexbuffer;
	IDirect3DIndexBuffer9* indexbuffer;

	IDirect3DVertexBuffer9 * lowvb;
	IDirect3DIndexBuffer9* lowib;

	
	int lowvertexcount;
	int lowfacecount;



	
	int vertexcount;
	int facecount;

	TerrainMesh();

	~TerrainMesh();

	void Init(IDirect3DDevice9* pDevice);


	void Render();

	void Renderlow();


	void Shut();

	void InitBuffer(int terrainsize, float deltasize, int &facecount, int &vertexcount, IDirect3DVertexBuffer9** ppvb, IDirect3DIndexBuffer9** ppib);


};