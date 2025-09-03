#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Position;
} gs_in[];

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Position;
} gs_out;

uniform float time;

vec4 Explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

void main() {    
    vec3 normal = GetNormal();

    gl_Position = Explode(gl_in[0].gl_Position, normal);
    gs_out.FragPos = gs_in[0].FragPos;
    gs_out.Normal = gs_in[0].Normal;
    gs_out.TexCoords = gs_in[0].TexCoords;
    gs_out.Position = gs_in[0].Position;
    EmitVertex();

    gl_Position = Explode(gl_in[1].gl_Position, normal);
    gs_out.FragPos = gs_in[1].FragPos;
    gs_out.Normal = gs_in[1].Normal;
    gs_out.TexCoords = gs_in[1].TexCoords;
    gs_out.Position = gs_in[1].Position;
    EmitVertex();

    gl_Position = Explode(gl_in[2].gl_Position, normal);
    gs_out.FragPos = gs_in[2].FragPos;
    gs_out.Normal = gs_in[2].Normal;
    gs_out.TexCoords = gs_in[2].TexCoords;
    gs_out.Position = gs_in[2].Position;
    EmitVertex();

    EndPrimitive();
}