#version 330 core

in vec3 frag_pos;
in vec3 f_normal;
in vec2 f_texcoord;

out vec4 FragColor;

uniform sampler2D tex;

void main()
{
    float ambient = 0.3f;

    vec3 norm = normalize(f_normal);
    vec3 light_dir = normalize(vec3(-3, 4, -1));

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * vec3(1, 1, 1);

    FragColor = (diff + ambient) * texture(tex, f_texcoord);
}