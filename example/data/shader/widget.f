out vec4 color;


in block {
  vec4 vertex_color;
} In;


void main(){
    color = In.vertex_color;
}

