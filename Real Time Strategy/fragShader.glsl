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

void main()
{
    vec4 color = texture(ourTexture, texCoords);
    if (color.a == 0)
    {
        discard;
    }
    FragColor = color;
}

