#include "ModelLoader.hpp"

#include <iostream>

std::shared_ptr<Model> ModelLoader::LoadFromFile(const std::string& path, std::shared_ptr<Shader> shader_ptr)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices |
        aiProcess_PreTransformVertices // bakes node transforms into vertices, flattens the scene
    );

    if (scene == nullptr || scene->mRootNode == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0)
    {
        std::cout << "ModelLoader::LoadFromFile Error: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    const std::string directory = path.substr(0, path.find_last_of("/\\") + 1);

    std::vector<std::shared_ptr<Model>> meshes;
    meshes.reserve(scene->mNumMeshes);
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        std::shared_ptr<Model> mesh_ptr = ProcessMesh(scene->mMeshes[i], scene, directory, shader_ptr);
        if (mesh_ptr != nullptr)
        {
            meshes.push_back(mesh_ptr);
        }
    }

    if (meshes.empty())
    {
        std::cout << "ModelLoader::LoadFromFile Error: no usable meshes in " << path << std::endl;
        return nullptr;
    }

    if (meshes.size() == 1)
    {
        return meshes.front();
    }

    std::shared_ptr<Model> root_ptr = MakeEmptyParent(shader_ptr);
    for (const std::shared_ptr<Model>& mesh_ptr : meshes)
    {
        root_ptr->AddChild(mesh_ptr);
    }
    return root_ptr;
}

std::shared_ptr<Model> ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory,
                                                 const std::shared_ptr<Shader>& shader_ptr)
{
    if (mesh->mNumVertices == 0)
    {
        return nullptr;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> tex_coords;
    std::vector<glm::vec3> colors;
    std::vector<glm::uvec3> indices;

    positions.reserve(mesh->mNumVertices);
    const bool has_tex_coords = mesh->mTextureCoords[0] != nullptr;
    const bool has_vertex_colors = mesh->mColors[0] != nullptr;
    if (has_tex_coords) tex_coords.reserve(mesh->mNumVertices);
    if (has_vertex_colors) colors.reserve(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        const aiVector3D& p = mesh->mVertices[i];
        positions.emplace_back(p.x, p.y, p.z);

        if (has_tex_coords)
        {
            tex_coords.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }

        if (has_vertex_colors)
        {
            const aiColor4D& c = mesh->mColors[0][i];
            colors.emplace_back(c.r, c.g, c.b);
        }
    }

    indices.reserve(mesh->mNumFaces);
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];
        if (face.mNumIndices != 3)
        {
            continue; // aiProcess_Triangulate should prevent this, skip defensively
        }
        indices.emplace_back(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
    }

    std::shared_ptr<Model> model_ptr{std::make_shared<Model>()};
    model_ptr->SetGeometry(positions, indices);

    const aiMaterial* material =
        mesh->mMaterialIndex < scene->mNumMaterials ? scene->mMaterials[mesh->mMaterialIndex] : nullptr;
    const std::string diffuse_path = ResolveTexturePath(material, directory);

    if (!diffuse_path.empty() && has_tex_coords)
    {
        model_ptr->SetMaterial(diffuse_path.c_str(), tex_coords);
    }
    else if (has_vertex_colors)
    {
        model_ptr->SetMaterial(colors);
    }
    else
    {
        // No image texture and no per-vertex colors -- fall back to the
        // material's flat diffuse color (Kd in the .mtl) if there is one,
        // otherwise mid-grey.
        aiColor3D diffuse_color(0.7f, 0.7f, 0.7f);
        if (material != nullptr)
        {
            material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color);
        }
        model_ptr->SetMaterial(std::vector<glm::vec3>(
            positions.size(), glm::vec3(diffuse_color.r, diffuse_color.g, diffuse_color.b)));
    }

    model_ptr->SetShader(shader_ptr);
    model_ptr->initializeForGL();

    return model_ptr;
}

std::string ModelLoader::ResolveTexturePath(const aiMaterial* material, const std::string& directory)
{
    if (material == nullptr || material->GetTextureCount(aiTextureType_DIFFUSE) == 0)
    {
        return "";
    }

    aiString rel_path;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &rel_path) != AI_SUCCESS)
    {
        return "";
    }

    return directory + rel_path.C_Str();
}

std::shared_ptr<Model> ModelLoader::MakeEmptyParent(const std::shared_ptr<Shader>& shader_ptr)
{
    std::shared_ptr<Model> parent_ptr{std::make_shared<Model>()};
    parent_ptr->SetGeometry({glm::vec3(0.0f)}, {});
    parent_ptr->SetMaterial(std::vector<glm::vec3>{glm::vec3(1.0f)});
    parent_ptr->SetShader(shader_ptr);
    parent_ptr->initializeForGL();
    return parent_ptr;
}
