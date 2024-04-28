#version 330
out vec2 vUv;

void main(){
	vec4 vertices[3] = vec4[3](vec4(-1,-1,0,0),vec4(3,-1,2,0),vec4(-1,3,0,2));
	vUv = vertices[gl_VertexID].zw;
	gl_Position = vec4(vertices[gl_VertexID].xy,0,1);
}