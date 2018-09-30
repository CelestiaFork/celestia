#version 120

/*
 Render stars as point sprites; this shader does nothing more than
 perform a basic model/view/projection transform and pass along the
 point size attribute.
*/

attribute float size;
varying vec4 color;

void main(void) {
  gl_Position = ftransform();

  color = gl_Color;
  gl_PointSize = size;
}
