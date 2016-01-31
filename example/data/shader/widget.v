layout(location = 0) in vec3 position;

uniform mat4 world_matrix;


out gl_PerVertex {
  vec4 gl_Position;
};

void main() {
  gl_Position = world_matrix * vec4(position.xyz, 1.0);
}

