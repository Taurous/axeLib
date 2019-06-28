texture al_tex;

sampler2D s = sampler_state {
	texture = <al_tex>;
};

uniform float2 mouse_axes;
uniform float2 screen_size;
uniform float light_size;
uniform float fade_size;

float4 ps_main(VS_OUTPUT Input) : COLOR0
{
	float4 pixel = tex2D(s, Input.TexCoord);

	float4 color = { 0.145f, 0.075f, 0.106f, 1.0f };

	float2 screenpos = Input.ScreenPos.xy / Input.ScreenPos.w;
	screenpos.y *= -1.0f;
	
	screenpos.x = (screenpos.x + 1.0f) / 2.0f;
	screenpos.y = (screenpos.y + 1.0f) / 2.0f;

	screenpos *= screen_size;

	float dis_to_pixel = distance(mouse_axes, screenpos) - light_size;

	float mult = clamp(dis_to_pixel / fade_size, 0.0f, 0.8f);

	pixel.r = (pixel.r * (1.0f - mult)) + (color.r * mult);
	pixel.g = (pixel.g * (1.0f - mult)) + (color.g * mult);
	pixel.b = (pixel.b * (1.0f - mult)) + (color.b * mult);
	pixel *= pixel.a;

	return pixel;
}