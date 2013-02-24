// ******************************************************************
// * TGP4162 Project 2: Visualization using OpenGL shader language *
// * Stein Dale/Thorvald Natvig, IPT, NTNU							*
// ******************************************************************

// Required uniform variables to solve Project.
// Remember texture values are within the domain [0.0, 1.0]
uniform float splitter;		// splitter value, domain [0.0, 1.0]
uniform sampler2D tex;		// sampler for helping to access texture value

// The framework vertex shader conveys the following varying attributes which can be used if you want. 
varying vec3 normal;		// interpolated surface normal
varying vec3 lightDir;		// interpolated vector pointing towards light source

//smooth in vec3 position;

uniform vec3 lightPos;

void main()
{
	vec4 color;
	
	vec4 tx = texture2D(tex, gl_TexCoord[0].st);
	float value = tx[0] - splitter;

	vec4 test;

    if (value > 0.0){
        test[0] = 1.0;

        float temp = 1.0 - (value / (1.0-splitter));
        test[1] = temp;
        test[2] = temp;
    }
    else {
        test[2] = 1.0;

        float temp = 1.0 - (abs(value) / splitter);
        test[1] = temp;
        test[0] = temp;
    
    }

	test[3]= 1.0;

	gl_FragColor = test;
    
    float diffuseLightIntensity = max(0.0,dot(normalize(normal),lightDir));

    vec4 fragcolor = diffuseLightIntensity * test;
    
    gl_FragColor = fragcolor;
}
