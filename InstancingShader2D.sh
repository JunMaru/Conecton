
float4x4 mtx_proj;
sampler samp;

struct VS_OUT
{
	float4 pos : POSITION;
	float4 col : COLOR0;
        float2 tex_pos : TEXCOORD0;
};

VS_OUT vsMain( float2 Pos : POSITION,float4 Col : COLOR0,float2 UV : TEXCOORD0
              ,float4 Mtx1 : TEXCOORD1,float4 Mtx2 : TEXCOORD2,float4 Mtx3 : TEXCOORD3,float4 Mtx4 : TEXCOORD4,float2 TexPos : TEXCOORD5 )
{
	VS_OUT Out = (VS_OUT)0;

	float4x4 MtxWorld;

	MtxWorld[0][0] = Mtx1[0];
	MtxWorld[0][1] = Mtx1[1];
	MtxWorld[0][2] = Mtx1[2];
	MtxWorld[0][3] = Mtx1[3];

	MtxWorld[1][0] = Mtx2[0];
	MtxWorld[1][1] = Mtx2[1];
	MtxWorld[1][2] = Mtx2[2];
	MtxWorld[1][3] = Mtx2[3];

	MtxWorld[2][0] = Mtx3[0];
	MtxWorld[2][1] = Mtx3[1];
	MtxWorld[2][2] = Mtx3[2];
	MtxWorld[2][3] = Mtx3[3];

	MtxWorld[3][0] = Mtx4[0];
	MtxWorld[3][1] = Mtx4[1];
	MtxWorld[3][2] = Mtx4[2];
	MtxWorld[3][3] = Mtx4[3];

   	float4x4 mtx;
   	mtx  = mul( MtxWorld,mtx_proj );
	float4 pos4 = float4( Pos,0,1 );
   	Out.pos = mul( pos4, mtx );
	Out.col = Col;
        Out.tex_pos = UV + TexPos;
	return Out;
		
}

float4 psMain( VS_OUT In ) : COLOR0
{   
   return In.col * tex2D( samp,In.tex_pos);
}