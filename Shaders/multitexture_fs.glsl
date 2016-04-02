#version 150

// fragment shader 
out vec4 fColor; 
// per-fragment interpolated values from the 
// vertex shader 
in float y; // for texture interpolation
in vec3 fN; 
in vec3 fL; 
in vec3 fE; 
in vec2 texCoord;
in vec3 fS;

// Spotlight
uniform vec4 spotPosition;
uniform vec4 spotDir;
uniform float spotAngle;

uniform sampler2D sand;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D snow;

in mat4 mv; // without transformations (world coords)
in mat4 proj;
in vec4 pos; // transformed vertices (world coords)

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct; 
uniform vec4 LightPosition;
uniform float Shininess;

void main() {

    //fColor = vec4(1);
    //return;

    // Normalize the input lighting vectors 
    //vec3 N = normalize(mv * vec4(fN, 1.0)).xyz;
    //vec3 E = normalize(mv * vec4(fE, 1.0)).xyz;
    //vec3 L = normalize(mv * vec4(fL, 1.0)).xyz;
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

    vec4 texInterp = vec4(clamp(1.0f - abs(y - 0)/150.0f, 0, 1), clamp(1.0f - abs(y - 250)/150.0f, 0, 1), clamp(1.0f - abs(y - 500)/150.0f, 0, 1), clamp(1.0f - abs(y - 700)/150.0f, 0, 1));

    texInterp = texInterp / (texInterp.x + texInterp.y + texInterp.z + texInterp.w);

    vec4 shadeTex = (texInterp.x * sandTex) + (texInterp.y * grassTex) + (texInterp.z * rockTex) + (texInterp.w * snowTex);

    // Spotlight
    float intensity = 0.0;
    vec4 spec = vec4(0.0);
 
    // spotPosition and pos are in mv coordinates
    vec4 spot = mv * spotPosition;
    vec4 dir = (mv * spotDir) - spot;
    //vec4 dir = spotDir;
    vec3 ld = normalize(pos-spot).xyz;
    vec3 sd = normalize(vec3(dir)).xyz;
    vec3 n = (mv * vec4(N, 1.0)).xyz;
 
    // inside the cone?
    if (dot(sd,ld) > spotAngle) {
 
        intensity = max(dot(n,ld), 0.0);
 
        vec3 h = normalize(ld + E);
        if (intensity > 0.0)
        {
            float intSpec = max(dot(h,n), 0.0);
            float Kd = max(dot(ld, n), 0.0);
            diffuse = (Kd*DiffuseProduct);
            spec = (vec4(0.7, 0.0, 0.0, 1.0) + DiffuseProduct + vec4((SpecularProduct * pow(intSpec, Shininess))));
            shadeLight = vec4(0);
        }
    }
    
    fColor = (shadeLight+spec)*shadeTex;
}