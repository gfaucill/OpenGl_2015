#version 330 compatibility

/// PI
float PI = 3.1415926535897932384626433832795;

///////////////////////////////////////////////////////////////////////////////
///                  Info from vertex shader
///////////////////////////////////////////////////////////////////////////////
in vec3 color;
in vec3 normal;
in vec3 pos;
in vec2 UV;
in vec3 textCoord;

///////////////////////////////////////////////////////////////////////////////
///                  Data structure for Lights and Materials
///////////////////////////////////////////////////////////////////////////////
#define MAX_LIGHT 16

uniform bool lightON;

struct LightParameters {
   vec4 ambient;
   vec4 diffuse;
   vec4 specular;
   vec4 position;
   vec3 spotDirection;
   float spotCutoff;
   int shadingModel; /* { Phong = 0 } */
   int lightType; /* { Ponctual = 0 , Directional = 1 , Spot = 2 } */
};

// Array who contains all lights
uniform LightParameters Lights[MAX_LIGHT];
// Maximum number of light
uniform int maxLights;

struct MaterialParametrers {
    vec4 emission;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

// Contains materials parameter of the curent mesh
uniform MaterialParametrers Materials;

struct LigthProduct {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

uniform LigthProduct u_lightProduct[MAX_LIGHT];
LigthProduct lightProduct[MAX_LIGHT];

///////////////////////////////////////////////////////////////////////////////
///                  Texture support
///////////////////////////////////////////////////////////////////////////////
uniform bool textureActive;
uniform sampler2D texture0;

///////////////////////////////////////////////////////////////////////////////
///                  Skybox support
///////////////////////////////////////////////////////////////////////////////
uniform bool skyboxActive;
uniform samplerCube cubeMapTexture;

///////////////////////////////////////////////////////////////////////////////
///                     phong Shading Models
///////////////////////////////////////////////////////////////////////////////

vec4 phongShading(vec3 L, vec3 E, vec3 R, int lightIndex) {

    //calculate Ambient Term:
    vec4 Iamb = vec4(lightProduct[lightIndex].ambient);

    //calculate Diffuse Term:
    vec4 Idiff = vec4(lightProduct[lightIndex].diffuse) * max(dot(normal,L), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);

    // calculate Specular Term:
    vec4 Ispec = vec4(lightProduct[lightIndex].specular )
            * pow(max(dot(R,E),0.0), 0.3 * Materials.shininess);
    Ispec = clamp(Ispec, 0.0, 1.0);

    // write Total Color:
    return Idiff + Ispec;
}

vec4 phongShadingSpot(int lightIndex) {
    vec3 L = normalize(vec3(Lights[lightIndex].position) - pos);
    vec3 E = normalize(Lights[lightIndex].spotDirection);
    vec4 Iamb = vec4(0.0),  Idiff = vec4(0.0), Ispec = vec4(0.0);


    vec3 n = normalize(normal.xyz);
    vec3 v = Lights[lightIndex].position.xyz;
    vec3 l = normalize(v - pos);
    vec3 h = normalize(l-normalize(v));
    vec3 d = normalize(Lights[lightIndex].spotDirection);

    //calculate Ambient Term:
    Iamb = vec4(lightProduct[lightIndex].ambient);

    // In the Cone ?
    if ( dot(-l, d) > cos(Lights[lightIndex].spotCutoff/2)) {
        vec3 R = normalize(reflect(L, normal));

        //calculate Diffuse Term:
        Idiff = vec4(lightProduct[lightIndex].diffuse) * max(dot(normal,L), 0.0);
        Idiff = clamp(Idiff, 0.0, 1.0);

        // calculate Specular Term:
        Ispec = vec4(lightProduct[lightIndex].specular)
                * pow(max(dot(R,E),0.0),0.3*Materials.shininess);
        Ispec = clamp(Ispec, 0.0, 1.0);
    }
    // write Total Color:
    return  Idiff + Ispec;
}

vec4 phongShadingDirectional(int lightIndex) {
    vec3 L = normalize(Lights[lightIndex].spotDirection);
    vec3 E = normalize(-pos);
    vec3 R = normalize(-reflect(L,normal));
    return phongShading(L, E, R, lightIndex);
}

vec4 phongShadingPonctual(int lightIndex) {
    vec3 L = normalize(vec3(Lights[lightIndex].position) - pos);
    vec3 E = normalize(-pos);
    vec3 R = normalize(-reflect(L,normal));
    return phongShading(L, E, R, lightIndex);
}

///////////////////////////////////////////////////////////////////////////////
///                              main
///////////////////////////////////////////////////////////////////////////////
void main()
{
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);

    if( lightON == true ) {
        if( textureActive == true ) {
            //finalColor += texture2D(texture0, UV);
            vec4 color = texture2D(texture0, UV);
            for (int i=0; i<maxLights; i++) {
                lightProduct[i].ambient = color * Lights[i].ambient; //* Materials.ambient;
                lightProduct[i].diffuse = color * Lights[i].diffuse; //* Materials.diffuse;
                lightProduct[i].specular = color * Lights[i].specular; //* Materials.specular;
            }
        }
        else if ( skyboxActive == true )  {
            //gl_FragColor = textureCube(cubeMapTexture, textCoord) * 0.4;
            vec4 color = textureCube(cubeMapTexture, textCoord);
            for (int i=0; i<maxLights; i++) {
                lightProduct[i].ambient = color * Lights[i].ambient; //* Materials.ambient;
                lightProduct[i].diffuse = color * Lights[i].diffuse; //* Materials.diffuse;
                lightProduct[i].specular = color * Lights[i].specular; //* Materials.specular;
            }
        } else {
            //Copy lightProduct
            for (int i=0; i<maxLights; i++) {
                lightProduct[i].ambient = u_lightProduct[i].ambient;
                lightProduct[i].diffuse = u_lightProduct[i].diffuse;
                lightProduct[i].specular = u_lightProduct[i].specular;
            }
        }

        for (int i=0; i<maxLights; i++) {
            if(Lights[i].shadingModel == 0) {
                if (Lights[i].lightType == 0) {
                    finalColor += phongShadingPonctual(i);
                }
                else if(Lights[i].lightType == 1 ) {
                    finalColor += phongShadingDirectional(i);
                }
                else if(Lights[i].lightType == 2) {
                    finalColor += phongShadingSpot(i);
                }
            }
        }
        gl_FragColor = finalColor;
    }
    else {
        gl_FragColor = vec4(color, 1);
    }

}
