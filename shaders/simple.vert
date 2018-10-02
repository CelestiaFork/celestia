#version 120

uniform vec3 color;
uniform float opacity;

void main(void) {
  gl_Position = ftransform();
}
