//=============================================================================
#ifdef LN_HLSL_DX9

float4x4	g_worldMatrix;
float4x4	g_viewProjMatrix;

float2 g_viewportSize;
static float2 g_pixelStep = (float2(2.0, 2.0) / g_viewportSize);

texture		g_texture;
texture		g_glyphMaskTexture;

sampler2D	g_texSampler = sampler_state
{
	texture = <g_texture>;
	MINFILTER = NONE;
	MAGFILTER = NONE;
};

sampler2D	g_glyphMaskSampler = sampler_state
{
	texture = <g_glyphMaskTexture>;
	MINFILTER = NONE;
	MAGFILTER = NONE;
};

struct VS_OUTPUT
{
	float4 Pos			: POSITION;
	float4 Color		: COLOR0;
	float4 UVOffset		: TEXCOORD0;
	float2 UVTileUnit	: TEXCOORD1;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
	float3 inPos		: POSITION,
	float4 inColor		: COLOR0,
	float4 inUVOffset	: TEXCOORD0,
	float2 inUVTileUnit	: TEXCOORD1)
{
	inPos.xy -= 0.5;
	VS_OUTPUT o;
	o.Pos			= mul(float4(inPos, 1.0f), g_worldMatrix);
	o.Pos			= mul(o.Pos, g_viewProjMatrix);
	//o.Pos.xy -= g_pixelStep;
	o.Color			= inColor;
	o.UVOffset		= inUVOffset;
	o.UVTileUnit	= inUVTileUnit;
	return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
	float4 inColor			: COLOR0,
	float4 inUVOffset		: TEXCOORD0,
	float2 inUVTileUnit		: TEXCOORD1) : COLOR0
{
	// 転送元左上 UV
	float2 uvUpperLeft = inUVOffset.xy;//float2(inUVOffset.x, inUVOffset.y);
	
	// 転送元矩形の幅 UV
	float2 uvWidth = inUVOffset.zw ;//uvUpperLeft - float2(inUVOffset.zw / inUVTileUnit);
	
	// 1つの四角形の中のどこにいるのか (0.0〜1.0)
	float2 uvRatio = fmod(inUVTileUnit, 1.0);
	
	float2 uv = lerp(uvUpperLeft, uvUpperLeft + uvWidth, uvRatio);
	//uv -= g_pixelStep;
	
    return tex2D(g_texSampler, uv) * tex2D(g_glyphMaskSampler, uv) * inColor;
}

//-------------------------------------------------------------------------
// technique
//-------------------------------------------------------------------------
technique MainDraw
{
	pass P0
	{
		VertexShader = compile vs_2_0 vsBasic();
		PixelShader	 = compile ps_2_0 psBasic();
	}
}
#endif
//=============================================================================
#ifdef LN_GLSL_VERTEX
uniform mat4	g_worldMatrix;
uniform mat4	g_viewProjMatrix;
attribute vec3	ln_Vertex;			// Pos
attribute vec4	ln_Color0;			// Color
attribute vec4	ln_MultiTexCoord0;	// UVOffset
attribute vec2	ln_MultiTexCoord1;	// UVTileUnit
varying vec4	v_Color;
varying vec4	v_UVOffset;
varying vec2	v_UVTileUnit;
void main()
{
	ln_Vertex.xy -= 0.5;
	gl_Position		= float4(ln_Vertex, 1.0f) * g_worldMatrix;
	gl_Position		= gl_Position * g_viewProjMatrix;
	v_Color			= ln_Color0;
	v_UVOffset		= ln_MultiTexCoord0;
	v_UVTileUnit	= ln_MultiTexCoord1;
}

#endif
//=============================================================================
#ifdef LN_GLSL_FRAGMENT
uniform sampler2D	g_texture;
varying vec3		v_Pos;
varying vec4		v_Color;
varying vec4		v_UVOffset;
varying vec2		v_UVTileUnit;
void main()
{
	vec2 uvUpperLeft = v_UVOffset.xy;	// 転送元左上 UV
	vec2 uvWidth = v_UVOffset.zw;		// 転送元矩形の幅 UV
	vec2 uvRatio = fmod(v_UVTileUnit, 1.0);	// 1つの四角形の中のどこにいるのか (0.0〜1.0)
	vec2 uv = lerp(uvUpperLeft, uvUpperLeft + uvWidth, uvRatio);
    gl_FragColor = texture2D(g_texture, uv);
}
#endif
