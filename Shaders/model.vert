#version 330 core

layout(location = 0) in vec3 aPos;				//vertices
layout(location = 1) in vec3 vertexNormal;		//normals
layout(location = 2) in vec2 aTex;				//Uvs
layout(location = 3) in vec3 m_tan;				//tangent
layout(location = 4) in vec3 m_btan;			//bitangent

//pass the tex coord to the fragment shader
out vec3 normCoord;
out vec3 fragPos;
out vec2 texCoord;
out mat3 TBN;


uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

void main ()
{
	gl_Position = projection *  //multiply the Projection Matrix with the View
				  view *		//multiply the View with the Position
				  transform *	//Multiply the matrix with the Position
				  vec4(aPos, 1.0); //Turns our 3x1 matrix into 1 4x1

	texCoord = aTex;

	mat3 modelMat = mat3(transpose(inverse(transform)));
	normCoord = modelMat * vertexNormal;

	vec3 T = normalize(modelMat * m_tan);		//tangent
	vec3 B = normalize(modelMat * m_btan);		//bitangent
	vec3 N = normalize(normCoord);
	TBN = mat3(T, B, N);

	fragPos = vec3(transform * vec4(aPos, 1.0));
	
}


//commented out ones are related to texture and normals, uncomment them if texture and normals are implemented in main