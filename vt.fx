//--------------------------------------------------------------------------------------
// File: virtualTex.fx
//
// The effect file for the virtualTex sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------


texture g_IndirectTexture;

float    g_fTime;                   // App's time in seconds
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix




texture g_HeightTexture;              // Color texture for mesh
sampler HeightTextureSampler =
sampler_state
{
	Texture = <g_HeightTexture>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

texture mask1;
sampler mask1Sampler =
sampler_state
{
	Texture = <mask1>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

texture mask2;
sampler mask2Sampler =
sampler_state
{
	Texture = <mask2>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};



texture brock;
sampler brockSampler =
sampler_state
{
	Texture = <brock>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MIPMAPLODBIAS = -1;
};

texture bforest;
sampler bforestSampler =
sampler_state
{
	Texture = <bforest>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MIPMAPLODBIAS = -1;
};

texture grass;
sampler grassSampler =
sampler_state
{
	Texture = <grass>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MIPMAPLODBIAS = -1;
};

texture ground;
sampler groundSampler =
sampler_state
{
	Texture = <ground>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MIPMAPLODBIAS = -1;
};

texture pine;
sampler pineSampler =
sampler_state
{
	Texture = <pine>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MIPMAPLODBIAS = -1;
};

texture srock;
sampler srockSampler =
sampler_state
{
	Texture = <srock>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MIPMAPLODBIAS = -1;
};

texture indirectTex;
sampler IndirectSampler =
sampler_state
{
	Texture = <indirectTex>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = POINT;
	MIPMAPLODBIAS = 3;
};


texture indirectMap;
sampler IndirectBiasMapSampler =
sampler_state
{
	Texture = <indirectMap>;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = POINT;
	MIPMAPLODBIAS = 6;
};

texture CacheTexture;
sampler CacheTextureSampler =
sampler_state
{
	Texture = <CacheTexture>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	
};


texture TestTexture;
sampler TestSampler =
sampler_state
{
	Texture = <TestTexture>;
	MipFilter = POINT;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MIPMAPLODBIAS = 4;

};

struct VS_OUTPUTNormal
{
	float4 Position   : POSITION;   // vertex position 
	float2 TextureUV  : TEXCOORD0;  // vertex texture coords 

};

struct VS_COLOR
{
	float4 Position   : POSITION;   // vertex position 
	float4 Color  : COLOR0;  // vertex texture coords 
	float psize : PSIZE;
};

//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
	
	float4 RGBColor : COLOR0;  // Pixel color    
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUTNormal RenderSceneTerrain(float4 vPos : POSITION,
	float2 vTexCoord0 : TEXCOORD0)
{

	VS_OUTPUTNormal Output;

	float3 Wpos = vPos.xyz;

	float2 Tex = vTexCoord0;

	float height = 122.0f*tex2Dlod(HeightTextureSampler, float4(Tex, 0, 0));

	Wpos.y = height;

	// Transform the position from object space to homogeneous projection space
	Output.Position = mul(float4(Wpos, 1.0f), g_mWorldViewProjection);

	Output.TextureUV = vTexCoord0;

	return Output;

}

VS_OUTPUTNormal RenderSceneTerrain2(float4 vPos : POSITION,
	float2 vTexCoord0 : TEXCOORD0)
{

	VS_OUTPUTNormal Output;

	float3 Wpos = vPos.xyz;

	// Transform the position from object space to homogeneous projection space
	Output.Position = mul(float4(Wpos, 1.0f), g_mWorldViewProjection);

	Output.TextureUV = vTexCoord0;

	return Output;

}


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
// color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS(VS_OUTPUTNormal In)
{
	PS_OUTPUT Output;

	float4 maskdata1 = tex2D(mask1Sampler, In.TextureUV);
	float4 maskdata2 = tex2D(mask2Sampler, In.TextureUV);

	float2 uv = 32.0f * In.TextureUV;

	float4 color = tex2D(grassSampler, uv);

	color = tex2D(pineSampler, 5.0f*uv)*maskdata2.r + color * (1.0f - maskdata2.r);

	color = tex2D(bforestSampler, 6.0f*uv)*maskdata1.g + color * (1.0f - maskdata1.g);

	color = tex2D(groundSampler, 7.0f*uv)*maskdata1.a + color * (1.0f - maskdata1.a);

	color = tex2D(brockSampler, 8.0f*uv)*maskdata1.r + color * (1.0f - maskdata1.r);

	color = tex2D(srockSampler, 9.0f*uv)*maskdata2.g + color * (1.0f - maskdata2.g);

	Output.RGBColor = color;

	return Output;
}




PS_OUTPUT RenderScenePS1(VS_OUTPUTNormal In)
{
	PS_OUTPUT Output;

	float2 uv =  In.TextureUV;

	float4 color = tex2D(HeightTextureSampler, uv);

	Output.RGBColor = color;

	return Output;
}


float mip_map_level(float2 texture_coordinate) // in texel units
{
	float2  dx_vtc = ddx(texture_coordinate);
	float2  dy_vtc = ddy(texture_coordinate);
	float delta_max_sqr = max(dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc));
	float mml = 0.5 * log2(delta_max_sqr)+0.5f;
	return max(0, mml); 
}

float4 colorlevel[11] = 
{
	float4(1.0f,1.0f,1.0f,1.0f),
	float4(0.0f, 0.0f, 1.0f, 1.0f),
	float4(0.0f, 1.0f, 0.0f, 1.0f),
	float4(1.0f, 0.0f, 0.0f, 1.0f),

	float4(0.5f, 0.5f,0.5f,0.5f),
	float4(0.0f, 0.0f, 0.5f, 0.5f),
	float4(0.0f, 0.5f, 0.0f, 0.5f),
	float4(0.5f, 0.0f, 0.0f, 0.5f),

	float4(1.0f,1.0f,1.0f,1.0f),
	float4(0.0f, 0.0f, 1.0f, 1.0f),
	float4(0.0f, 1.0f, 0.0f, 1.0f)
};



struct PS_OUTPUT2
{
	float4 RGBColor : COLOR0;  // Pixel color    
   
};

PS_OUTPUT2 RenderScenePS2(VS_OUTPUTNormal In)
{
	PS_OUTPUT2 Output;
	
	float level =  mip_map_level(In.TextureUV *1024.0f) ;

	Output.RGBColor =  tex2D(IndirectSampler, In.TextureUV);


	return Output;
}


PS_OUTPUT RenderScenePS3(VS_OUTPUTNormal In)
{
	PS_OUTPUT Output;

	float4 color = tex2D(IndirectBiasMapSampler, In.TextureUV);

	float level = floor(color.r*255.0f);

	float size = 1024.0f / pow(2.0f, level);

	float2 uvbias = floor( float2(color.g*255.0f, color.b*255.0f) );

	uvbias *= 1.0f / 32.0f;

	float2 uv = frac(In.TextureUV * size)/32.0f;

	float2 finaluv = uvbias + uv;
	finaluv.y = 1.0f - finaluv.y;
	Output.RGBColor = tex2D(CacheTextureSampler, finaluv);

	//Output.RGBColor = color;

	return Output;
}


PS_OUTPUT RenderScenePSTest(VS_OUTPUTNormal In)
{
	PS_OUTPUT Output;

	float4 color = tex2D(TestSampler, In.TextureUV);

	Output.RGBColor = color;

	return Output;
}



VS_COLOR RenderPointVS( float4 vPos : POSITION,
						float4 vColor : COLOR0)
{

	VS_COLOR Output;

	// Transform the position from object space to homogeneous projection space
	float texsize =  1024.0f / pow(2.0f, vPos.z);
	float posx = (vPos.x +0.5f)/ texsize;
	float posy = (vPos.y + 0.5f) / texsize;
	Output.Position = float4(2.0f*posx-1.0f,1.0f-2.0f*posy, 0.5f,1.0f);
	Output.Color = vColor;
	Output.psize = 1.0f;

	return Output;

}


PS_OUTPUT RenderPointPS(VS_COLOR In)
{
	PS_OUTPUT Output;

	Output.RGBColor = In.Color;

	return Output;
}


//--------------------------------------------------------------------------------------
// Renders scene 
//--------------------------------------------------------------------------------------
technique RenderScene
{

	pass P0
	{
		//fillmode = wireframe;
		cullmode = none;
		VertexShader = compile vs_3_0 RenderSceneTerrain2();
		PixelShader = compile ps_3_0 RenderScenePS();
	}

	pass P1
	{
		
		VertexShader = compile vs_3_0 RenderSceneTerrain();
		PixelShader = compile ps_3_0 RenderScenePS1();
	}

	pass P2
	{
		
		VertexShader = compile vs_3_0 RenderSceneTerrain();
		PixelShader = compile ps_3_0 RenderScenePS2();
	}

	pass P3
	{
		zenable = true;
		VertexShader = compile vs_3_0 RenderSceneTerrain();
		PixelShader = compile ps_3_0 RenderScenePS3();
	}

	pass P4
	{

		VertexShader = compile vs_3_0 RenderSceneTerrain();
		PixelShader = compile ps_3_0 RenderScenePS();
	}

	pass P5
	{
		zenable = false;
		PointScaleEnable = true;
		VertexShader = compile vs_3_0 RenderPointVS();
		PixelShader = compile ps_3_0 RenderPointPS();
	}


}
