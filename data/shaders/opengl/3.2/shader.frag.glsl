#version 150

uniform sampler2D       uTexture;
uniform usampler2DRect  uSymbol;
uniform sampler2DRect   uColor;

uniform vec2 uTileCount;

in vec2 fTexCoord;

out vec3 color;

void main()
{
    ivec2 texCoords = ivec2(uTileCount * fTexCoord);

    float sym = texelFetch(uSymbol, texCoords).r;
    vec4 fColor = texelFetch(uColor, ivec2(texCoords.x * 2 + 0, texCoords.y));
    vec4 bColor = texelFetch(uColor, ivec2(texCoords.x * 2 + 1, texCoords.y));

    vec2 symIntCoords = vec2(mod(sym, 32.0), floor(sym * 0.03125));
    vec2 symCoords = (fract(uTileCount * fTexCoord) + symIntCoords) * 0.03125;

    vec4 symColor = texture(uTexture, symCoords);
    vec4 tileColor = fColor * symColor;
    color = mix(bColor, tileColor, symColor.a).rgb;
}
