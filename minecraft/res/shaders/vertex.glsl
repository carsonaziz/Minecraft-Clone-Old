#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 frag_pos;
out vec3 f_normal;
out vec2 f_texcoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
   f_normal = aNormal;
   f_texcoord = aUV;

   frag_pos = vec3(model * vec4(aPos.x, aPos.y, aPos.z, 1.0));
   gl_Position = proj * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}