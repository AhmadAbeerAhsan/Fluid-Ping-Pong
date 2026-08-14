#pragma once

#include <memory>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Model.hpp"
#include "Shader.hpp"

// Loads .obj (or anything else Assimp supports) into Model instances --
// geometry and texture only. Node transforms are baked into the vertex data
// by Assimp itself (see aiProcess_PreTransformVertices in the .cpp), and
// per-material shaders are not extracted: every produced Model just uses
// the Shader you pass in.
//
// Returns a single Model. If the file has more than one mesh (e.g. one
// aiMesh per material, which is normal for .obj), that Model is an empty
// parent with each mesh Model added as a direct child via AddChild --
// draw() on the parent will draw all of them.
class ModelLoader
{
public:
    // Returns nullptr on failure (Assimp's error is printed to stdout).
    static std::shared_ptr<Model> LoadFromFile(const std::string& path, std::shared_ptr<Shader> shader_ptr);

private:
    static std::shared_ptr<Model> ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory,
                                               const std::shared_ptr<Shader>& shader_ptr);

    static std::string ResolveTexturePath(const aiMaterial* material, const std::string& directory);

    // Only used as a parent when a file has more than one mesh. Model::draw()
    // calls drawVertices()/useTexture() unconditionally with no null check,
    // so even a "just hold children" node needs valid (if invisible)
    // geometry and material or it will crash on draw.
    static std::shared_ptr<Model> MakeEmptyParent(const std::shared_ptr<Shader>& shader_ptr);
};
