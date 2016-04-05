#version 150

// fragment shader 
out vec4 fColor; 
// per-fragment interpolated values from the 
// vertex shader 
in vec3 fN; 
in vec3 fL; 
in vec3 fE; 
in vec2 texCoord;

in float y; // for texture interpolation

uniform sampler2D sand;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D snow;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;

void main() {
    // Normalize the input lighting vectors 
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);
    vec3 H = normalize( L + E ); 
    vec4 ambient = AmbientProduct; 

    float Kd = max(dot(L, N), 0.0); 
    vec4 diffuse = Kd*DiffuseProduct; 

    float Ks = pow(max(dot(N, H), 0.0), Shininess); 
    vec4 specular = Ks*SpecularProduct;
    //discard the specular highlight if the 
    //vertex is not facing light 
    if( dot(L, N) < 0.0 )
        specular = vec4(0.0, 0.0, 0.0, 1.0); 

    vec4 shadeLight = ambient + diffuse + specular; 
    shadeLight.a = 1.0;

    vec4 sandTex = vec4(texture(sand,texCoord).rgb, 1.0);
    vec4 grassTex = vec4(texture(grass,texCoord).rgb, 1.0);
    vec4 rockTex = vec4(texture(rock,texCoord).rgb, 1.0);
    vec4 snowTex = vec4(texture(snow,texCoord).rgb, 1.0);

    float newY = y * 1000;

    vec4 texInterp = vec4(clamp(1.0f - abs(newY - 0)/100.0f, 0, 1), clamp(1.0f - abs(newY - 100)/150.0f, 0, 1), clamp(1.0f - abs(newY - 200)/150.0f, 0, 1), clamp(1.0f - abs(newY - 250)/150.0f, 0, 1));

    texInterp = texInterp / (texInterp.x + texInterp.y + texInterp.z + texInterp.w);

    vec4 shadeTex = (texInterp.x * sandTex) + (texInterp.y * grassTex) + (texInterp.z * rockTex) + (texInterp.w * snowTex);

    fColor = shadeTex;
//    return;

    fColor = shadeLight*shadeTex;
    //fColor = mix(shadeLight, shadeTex, 0.5);//(shadeLight)*shadeTex;
}