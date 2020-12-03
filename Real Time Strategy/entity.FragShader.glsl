#version 330 core


//layout (location = 2) in float texIndex;

out vec4 FragColor;




//struct Material {
 //   sampler2D texture_diffuse1;
 //   };

in vec2 texCoords;
in vec2 coords;
//uniform Material material;

uniform sampler2D ourTexture;
//uniform sampler2D texture_diffuse3;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_specular2;

uniform vec3 tint;
uniform vec3 tintRatio;
uniform bool ignoreAlpha;

void main()
{
    vec4 color = texture(ourTexture, texCoords);
    //if (color.a == 0)
    //{
    //    discard;
    //}
    if (tint != vec3(0))
    {
        color = vec4(mix(vec3(color), tint, tintRatio), color.a);
    }
    if(ignoreAlpha)
    {
        color.a = 1.0f;
    }
    FragColor = color;
}

