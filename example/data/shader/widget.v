layout(location = 0) in vec3 position;
layout(location = 1) in vec4 vertex_color;

uniform mat4 world_matrix;


out gl_PerVertex {
  vec4 gl_Position;
};


out block {
  vec4 vertex_color;
} Out;


void main() {
  gl_Position = world_matrix * vec4(position.xyz, 1.0);
  Out.vertex_color = vertex_color;
}

