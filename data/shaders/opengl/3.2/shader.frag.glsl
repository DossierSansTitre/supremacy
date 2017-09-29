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
    vec2 sym_coord = vec2(mod(sym, 32.0), floor(sym / 32.0));
    vec2 tcoord = (fract(uTileCount * fTexCoord) + sym_coord) / 32.0;

    vec4 ccolor = vec4(texture(uColor, fTexCoord).rgb, 1.0);
    vec4 csymbol = texture(uTexture, tcoord);
    vec3 fcolor = vec3(ccolor * csymbol);
    vec3 bcolor = texture(uColorBg, fTexCoord).rgb;
    float a = csymbol.a;
    color = mix(bcolor, fcolor, a);
}
