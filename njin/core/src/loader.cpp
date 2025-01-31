#include "core/loader.h"

#include <core/MeshBuilder.h>

#include "core/njCamera.h"
#include "util/GLTFAsset.h"
#include "util/json.h"

#include <set>

namespace rj = rapidjson;

namespace njin::core {
    void load_meshes(const std::string& path,
                     njRegistry<njMesh>& mesh_registry) {
        // Check that the schema for the config is a valid json
        rj::Document document{
            njin::util::make_validated_document("schema/meshes.schema.json",
                                                path)
        };

        rj::GenericArray meshes{ document["meshes"].GetArray() };
        for (const auto& gltf_mesh : meshes) {
            std::string name{ gltf_mesh["name"].GetString() };
            std::string mesh_path{ gltf_mesh["path"].GetString() };
            njin::gltf::GLTFAsset asset{ mesh_path };

            // get the attribute arrays
            auto position_attributes{ asset.get_position_attributes() };
            auto normal_attributes{ asset.get_normal_attributes() };
            auto tangent_attributes{ asset.get_tangent_attributes() };
            auto indices{ asset.get_indices() };

            core::MeshBuilder mesh_builder{ indices };
            mesh_builder.add_position_attributes(position_attributes);
            mesh_builder.add_normal_attributes(normal_attributes);
            mesh_builder.add_tangent_attributes(tangent_attributes);

            core::njMesh mesh{ mesh_builder.build() };
            mesh_registry.add(name, mesh);
        }
    }

    void load_cameras(const std::string& path,
                      njRegistry<njCamera>& camera_registry) {
        rj::Document document{
            njin::util::make_validated_document("schema/cameras.schema.json",
                                                path)
        };

        rj::GenericArray cameras{ document["cameras"].GetArray() };
        for (const auto& rj_camera : cameras) {
            std::string name{ rj_camera["name"].GetString() };
            float far{ rj_camera["far"].GetFloat() };
            float near{ rj_camera["near"].GetFloat() };
            float y_fov{ rj_camera["yFov"].GetFloat() };

            njCamera::njCameraSettings settings{ .far = far,
                                                 .near = near,
                                                 .y_fov = y_fov };
            njCamera camera{ name, settings };
            camera_registry.add(name, camera);
        }
    }
}  // namespace njin::core