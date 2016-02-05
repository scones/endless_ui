out vec4 color;


in block {
  vec4 vertex_color;
} In;


void main(){
  float alpha          = In.vertex_color.a;
  float negative_alpha = 1.0 - alpha;

  color                = In.vertex_color * alpha + gl_Color * negative_alpha;
}

