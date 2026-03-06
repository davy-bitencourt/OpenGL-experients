#version 330 core

out vec4 FragColor;

/* uniform é chamado por call draw */
uniform vec4 u_Color;

void main(){
   FragColor = u_Color;
};