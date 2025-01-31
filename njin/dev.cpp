#include <core/MeshBuilder.h>
#include <rapidjson/document.h>
#include <util/GLTFAsset.h>

#include "core/njMesh.h"
#include "core/njRegistry.h"
#include "util/json.h"
namespace core = njin::core;
namespace rj = rapidjson;

void load_meshes(core::njRegistry<core::njMesh>& registry) {
    // Check that the schema for the config is a valid json
    rj::Document document{
        njin::util::make_validated_document("schema/meshes.schema.json",
                                            "meshes.nj")
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
        registry.add(name, mesh);
    }
}

int main() {
    core::njRegistry<core::njMesh> mesh_registry{};
    load_meshes(mesh_registry);

    // core::njRegistry<core::njNode> node_registry();
    // load_level(node_registry);
}
