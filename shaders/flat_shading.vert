#version 330 compatibility

///////////////////////////////////////////////////////////////////////////////
///                                 VAO
///////////////////////////////////////////////////////////////////////////////
layout(location=0) in vec3 vx_pos; // Vertex
layout(location=1) in vec3 vx_nrm; // Normal
layout(location=2) in vec2 vx_txt; // Texture coordonate
layout(location=3) in vec3 vx_col; // Color

///////////////////////////////////////////////////////////////////////////////
///                         Info to fragment shader
///////////////////////////////////////////////////////////////////////////////
out vec3 color;
out vec3 normal;
out vec3 pos;
out vec2 UV;
out vec3 textCoord;
///////////////////////////////////////////////////////////////////////////////
///                            Scene's  Matrix
///////////////////////////////////////////////////////////////////////////////
uniform mat4 modelMatrix, normalMatrix, viewMatrix, projectionMatrix;

void main()
{ 
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vx_pos, 1.0);
  vec4 vertpos = viewMatrix * modelMatrix * vec4(vx_pos,1.0);
  pos = vec3(vx_pos);

  normal =  normalize(vec3(viewMatrix * /*modelMatrix*/ normalMatrix * vec4(vx_nrm, .0)));

  color = vx_col;
  UV = vec2(vx_txt);
  textCoord = vx_pos;
}
