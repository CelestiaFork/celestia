#version 120

varying vec4 color;

void main(void) {
  vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
  float distance = dot(circCoord, circCoord);
  if (distance > 1.0) {
    gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  } else {
//    if (distance > 0.95) {
//      gl_FragColor = vec4(color.xyz, sin(distance * 3.14 * 0.5));
//    } else {
      gl_FragColor = color;
//    }
  }
}
