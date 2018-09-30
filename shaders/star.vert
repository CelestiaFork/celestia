#version 110

/*
 Render stars as point sprites; this shader does nothing more than
 perform a basic model/view/projection transform and pass along the
 point size attribute.
*/

//ATTRIB iPos          = vertex.position;
//ATTRIB iColor        = vertex.color;
//ATTRIB iSize         = vertex.attrib[6];
//PARAM  mvp[4]        = { state.matrix.mvp };
//OUTPUT oPos          = result.position;
//OUTPUT oColor        = result.color;
//OUTPUT oSize         = result.pointsize;

attribute float size;
varying vec4 color;

void main(void) {
  // # Transform the vertex by the modelview matrix
  // DP4   oPos.x, mvp[0], iPos;
  // DP4   oPos.y, mvp[1], iPos;
  // DP4   oPos.z, mvp[2], iPos;
  // DP4   oPos.w, mvp[3], iPos;
  gl_Position = ftransform();

  //MOV   oColor, iColor;
  color = gl_Color;
  //MOV   oSize.x, iSize;
  gl_PointSize = size * 0.5; // w/o 0.5 points are to big (diameter in old and radius in new shaders?)
}
