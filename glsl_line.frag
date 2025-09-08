
#version 330 core
in float fragColor;
in float fragTrans;
out vec4 FragColor;

void main()
{
    FragColor = vec4(fragColor * -1.0, fragColor, 0.0, abs(fragColor) * fragTrans);
}

