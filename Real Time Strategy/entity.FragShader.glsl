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
uniform bool outline;
uniform vec4 outlineColor;





void main()
{


    vec4 color = texture(ourTexture, texCoords);

    vec3 localTint = tint;


    if(outline && color.a == 0)
    {
        
        vec2 pixelSize = 1.0/ vec2(textureSize(ourTexture, 0));

        //TODO: this is bad as it's to omuch per pixel, maybe reduce the highlight width, or have it adjustable? :O

        for(int i = -10; i < 11; i++)
        {
            for(int j = -10; j < 11; j++)
            {
                vec4 possibleSolid = texture(ourTexture, texCoords + (vec2(i, j)*pixelSize), 0);
                if (possibleSolid.a != 0)
                {
                    color = outlineColor;
                    localTint = vec3(0);
                    i = 16;
                    j = 16;
                }
            }
        }
    
    }
    if (color.a == 0)
    {
        
        discard;
    }
    if (localTint != vec3(0))
    {
        color = vec4(mix(vec3(color), localTint, tintRatio), color.a);
    }
    if(ignoreAlpha)
    {
        color.a = 1.0f;
    }

    FragColor = color;
}

