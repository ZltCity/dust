#include <stdexcept>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

int main()
{
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile( "./assets/maps/test/test.glb",
//											 aiProcess_CalcTangentSpace       |
												 aiProcess_Triangulate            |
												 aiProcess_JoinIdenticalVertices  |
												 aiProcess_SortByPType);

	// If the import failed, report it
	if (nullptr == scene) {
		throw std::runtime_error("Could not import glb file.");
	}

	return 0;
}