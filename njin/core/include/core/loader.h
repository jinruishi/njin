#pragma once
#include "core/njCamera.h"
#include "core/njMesh.h"
#include "core/njRegistry.h"

namespace njin::core {

    /**
 * Loads meshes specified in a .meshes file into a given mesh registry
 * @param path Path to .meshes file
 * @param mesh_registry Mesh registry to populate
 */
    void load_meshes(const std::string& path,
                     njRegistry<njMesh>& mesh_registry);

    /**
 * Loads cameras specified in a .meshes file into a given mesh registry
 * @param path Path to .cameras file
 * @param camera_registry Camera registry to populate
 */
    void load_cameras(const std::string& path,
                      njRegistry<njCamera>& camera_registry);

}  // namespace njin::core