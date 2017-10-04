#version 150

uniform sampler2D   uTexture;
uniform usampler2D  uSymbol;
uniform sampler2D   uColor;
uniform sampler2D   uColorBg;

uniform vec2        uTileCount;

in vec2 fTexCoord;

out vec3 color;

void main()
{
    float sym = texture(uSymbol, fTexCoord).r;
    vec2 sym_coord = vec2(mod(sym, 32.0), floor(sym * 0.03125));
    vec2 tcoord = (fract(uTileCount * fTexCoord) + sym_coord) * 0.03125;

    vec3 ccolor = texture(uColor, fTexCoord).rgb;
    vec4 tex = texture(uTexture, tcoord);
    vec3 fcolor = vec3(ccolor * tex.rgb);
    vec3 bcolor = texture(uColorBg, fTexCoord).rgb;
    color = mix(bcolor, fcolor, tex.a);
}
