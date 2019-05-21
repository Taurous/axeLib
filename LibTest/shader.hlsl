texture al_tex;

sampler2D s = sampler_state {
	texture = <al_tex>;
};

float mouse_x;

float4 ps_main(VS_OUTPUT Input) : COLOR0
{
	float4 pixel = tex2D(s, Input.TexCoord);

	float mult = 0.0f;
	if (mouse_x > Input.TexCoord.x * 640) mult = 1.0f;

	pixel.r *= mult;
	pixel.g *= mult;
	pixel.b *= mult;

	return pixel;
}