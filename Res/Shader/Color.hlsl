cbuffer MatrixBuffer
{
	matrix matWorld;
	matrix matView;
	matrix matProj;
};

// Vertex Shader

struct VertexInput
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PixelInput ColorVertexShader(VertexInput input)
{
	PixelInput output;

	input.position.w = 1.f;

	output.position = mul(input.position, matWorld);
	output.position = mul(output.position, matView);
	output.position = mul(output.position, matProj);

	output.color = input.color;

	return output;
}


// Pixel Shader
float4 ColorPixelShader(PixelInput input) : SV_TARGET
{
	return input.color;
}