#version 150

in vec3 E;
in vec3 N;
uniform samplerCube cube_texture;
uniform vec4 Ambience;
uniform mat4 View;
out vec4 fColor;

void main () {

  vec3 E_incident = normalize (E);
  vec3 normal = normalize (N);

  vec3 reflected = reflect (E_incident, normal);
  
  // to world space
  reflected = vec3 (inverse (View) * vec4 (reflected, 0.0));

  fColor = texture (cube_texture, reflected) + Ambience;
}