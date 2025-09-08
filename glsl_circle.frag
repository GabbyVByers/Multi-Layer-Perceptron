
#version 330 core
in float fragColor;
out vec4 FragColor;

void main()
{
    vec2 coord = gl_PointCoord * 2.0 - 1.0;
    float dist = length(coord);
    if (dist > 1.0) discard;
    FragColor = vec4(fragColor, fragColor, fragColor, 1.0);
}

