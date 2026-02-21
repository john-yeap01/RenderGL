#version 330 core
out vec4 FragColor;
// color is from the vertex shader, interpolated
in vec3 color;
// UV coords of the texture 
in vec2 texCoord;

uniform sampler2D tex0;
uniform float time;

void main()
{
   // UV distortion -- like heat ripples
   const float frequency = 20.0f;
   const float speed = 10.0f;
   const float amplitude = 0.1f;

   vec2 center = vec2(0.5, 0.5);
   vec2 d = texCoord - center;
   float r = length(d); // defines how far ripples appear from the center
   float phase = r * frequency - time * speed;
   float wave = sin(phase);
   vec2 uvDistorted = texCoord + d * wave * amplitude ;
   FragColor = texture(tex0, uvDistorted) * vec4(color, 1.0);


   // texture returns a vec4 rgba, tex0 and texcoord are vec2s
   // FragColor = texture(tex0, texCoord) * vec4(color, 1.0);

}