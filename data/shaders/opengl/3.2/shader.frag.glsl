#version 150

uniform sampler2D   uTexture;
uniform usampler2D  uSymbol;
uniform sampler2D   uColor;
uniform sampler2D   uColorBg;

uniform vec2 uTileCount;

in vec2 fTexCoord;

out vec3 color;

void main()
{
    ivec2 texCoords = ivec2(uTileCount * fTexCoord);

    float sym = texelFetch(uSymbol, texCoords, 0).r;
    vec4 fColor = texelFetch(uColor, texCoords, 0);
    vec4 bColor = texelFetch(uColorBg, texCoords, 0);

    vec2 symIntCoords = vec2(mod(sym, 32.0), floor(sym * 0.03125));
    vec2 symCoords = (fract(uTileCount * fTexCoord) + symIntCoords) * 0.03125;

    vec4 symColor = texture(uTexture, symCoords);
    vec4 tileColor = fColor * symColor;
    color = mix(bColor, tileColor, symColor.a).rgb;
}
