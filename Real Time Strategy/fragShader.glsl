#version 330 core


//layout (location = 2) in float texIndex;

out vec4 FragColor;




//struct Material {
 //   sampler2D texture_diffuse1;
 //   };

in vec2 texCoords;
in vec3 coords;
//uniform Material material;

uniform sampler2D ourTexture;
uniform bool black;
uniform bool hitbox;
//uniform sampler2D texture_diffuse3;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_specular2;

void main()
{
    FragColor = texture(ourTexture, texCoords);
    if(black)
    {
        FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    if(hitbox)
    {
        FragColor = vec4(1.0f, 1.5, 1.0f, 1.0f);
    }
}

