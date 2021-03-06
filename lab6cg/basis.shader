#shader vertex
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;


uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec3 norm;
out vec3 fragCord;

void main() {

	gl_Position = u_proj * u_view *u_model * vec4(pos, 1);

}



#shader fragment
#version 330 core
out vec4 color;

uniform vec3 renderColor;



void main() {



	color = vec4(renderColor, 1);
}