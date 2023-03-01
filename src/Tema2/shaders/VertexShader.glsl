#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 car_position;

// Output value to fragment shader
out vec3 color;


void main()
{
    color = v_color;
    vec3 pos = (Model * vec4(v_position, 1.0f)).xyz;
    vec3 diff = pos - car_position;
    float dist = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    float yy = pos.y - dist * 0.01f;
    gl_Position = Projection * View * vec4(pos.x, yy, pos.z, 1.0f);
}
