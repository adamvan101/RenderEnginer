#version 150

// fragment shader 
out vec4 fColor; 
// per-fragment interpolated values from the 
// vertex shader 
in vec3 fN; 
in vec3 fL; 
in vec3 fE; 
in vec2 texCoord;
uniform sampler2D tex;
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

    vec4 shadeTex = vec4(texture(tex,texCoord).rgb, 1.0);

    fColor = shadeLight*shadeTex;
    //fColor = shadeTex;
    //fColor = vec4(texCoord.x, texCoord.y, 1.0, 1.0);
}