//
// Created by Maximilian Denninger on 2019-05-05.
//

#include "VisObject.h"

void VisObject::indexVBO(std::vector<glm::vec3>& in_vertices, std::vector<glm::vec3>& in_normals){
	struct PackedVertex {
		glm::vec3 position;
		glm::vec3 normal;

		bool operator<(const PackedVertex that) const{
			return memcmp((void*) this, (void*) &that, sizeof(PackedVertex)) > 0;
		};
	};

	auto getSimilarVertexIndex_fast = [](PackedVertex& packed,
										 std::map<PackedVertex, unsigned short>& VertexToOutIndex,
										 unsigned short& result) -> bool{
		const auto it = VertexToOutIndex.find(packed);
		if(it == VertexToOutIndex.end()){
			return false;
		}else{
			result = it->second;
			return true;
		}
	};

	std::map<PackedVertex, unsigned short> vertexToOutIndex;

	// For each input vertex
	for(unsigned int i = 0; i < in_vertices.size(); i++){
		PackedVertex packed = {in_vertices[i], in_normals[i]};

		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex_fast(packed, vertexToOutIndex, index);

		if(found){ // A similar vertex is already in the VBO, use it instead !
			m_indices.push_back(index);
		}else{ // If not, it needs to be added in the output data.
			m_indexedVertices.push_back(in_vertices[i]);
			m_indexedNormals.push_back(in_normals[i]);
			unsigned short newindex = (unsigned short) m_indexedVertices.size() - 1;
			m_indices.push_back(newindex);
			vertexToOutIndex[packed] = newindex;
		}
	}
}

bool VisObject::loadData(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals){
	std::vector<unsigned int> vertexIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;


	FILE* file = fopen(m_filePath.c_str(), "r");
	if(file == nullptr){
		printError("This path can not be found: " << m_filePath);
		getchar();
		return false;
	}

	while(true){
		char lineHeader[256];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if(res == EOF){
			break;
		} // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if(strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}else if(strcmp(lineHeader, "vn") == 0){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}else if(strcmp(lineHeader, "f") == 0){
			unsigned int vertexIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1],
								 &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
			if(matches != 6){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for(unsigned int i = 0; i < vertexIndices.size(); i++){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		vertices.push_back(vertex);
		normals.push_back(normal);
	}
	fclose(file);
	return true;

}

void VisObject::init(){
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	loadData(vertices, normals);

	indexVBO(vertices, normals);

	glGenBuffers(1, &m_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_indexedVertices.size() * sizeof(glm::vec3), &m_indexedVertices[0],
				 GL_STATIC_DRAW);

	glGenBuffers(1, &m_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_indexedNormals.size() * sizeof(glm::vec3), &m_indexedNormals[0],
				 GL_STATIC_DRAW);

	glGenBuffers(1, &m_elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned short), &m_indices[0], GL_STATIC_DRAW);
}

void VisObject::draw(){
	// Draw the triangles !
	glDrawElements(
			GL_TRIANGLES,      // mode
			m_indices.size(),    // count
			GL_UNSIGNED_SHORT,   // type
			(void*) 0           // element array buffer offset
	);

}
