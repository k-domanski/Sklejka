#include <pch.h>
#include "MeshMapping.h"
#include <Utility/Utility.h>

using namespace Engine::Utility;

namespace Editor {
  auto ShouldIgnore_1(int index) -> bool {
    /* Ignore Indexes */
    /* 0-1, 9, 11 - 19, 27 - 28, 34 */
    return InRange(index, 0, 1) || InRange(index, 9, 9) || InRange(index, 11, 15)
           || InRange(index, 22, 22) || InRange(index, 28, 28) || InRange(index, 34, 39);
  }
  auto GetColliderMapping_1() -> std::vector< TransformData > {
    return std::vector< TransformData >{
        /* D A T A */
        /* Ind */ /* Position */ /*   Rotation   */ /*   Scale   */
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /*0*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*1*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*2*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /*3*/ {{100.0f, 350.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {77.0f, 700.0f, 415.0f}},
        /*4*/ {{90.0f, 330.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {135.0f, 700.0f, 415.0f}},
        /*5*/ {{100.0f, 535.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {230.0f, 1175.0f, 670.0f}},
        /*6*/ {{110.0f, 585.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {160.0f, 1175.0f, 670.0f}},
        /*7*/ {{-7.6f, 200.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {130.0f, 400.0f, 250.0f}},
        /*8*/ {{45.0f, 220.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {130.0f, 425.0f, 280.0f}},
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /*9*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*10*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*11*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*12*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        ///*12*/ {{2.4f, 0.3f, 5.8f}, {-3.1f, -0.4f, 30.0f}, {3.5f, 0.2f, 13.4f}},
        /*13*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*14*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        ///*14*/ {{-0.6f, 0.2f, 6.0f}, {-4.8f, 0.0f, -30.0f}, {3.4f, 0.3f, 13.0f}},
        /*15*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*16*/ {{0.0f, 0.7f, 6.0f}, {-6.7f, 0.0f, 0.0f}, {8.0f, 0.2f, 30.0f}},
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /*17*/ {{90.0f, 330.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {135.0f, 700.0f, 415.0f}},
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /*18*/ {{100.0f, 535.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {230.0f, 1175.0f, 670.0f}},
        /*19*/ {{110.0f, 585.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {160.0f, 1175.0f, 670.0f}},
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /*20*/ {{-7.6f, 200.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {130.0f, 400.0f, 250.0f}},
        /*21*/ {{45.0f, 220.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {130.0f, 425.0f, 280.0f}},
        /*22*/ {{80.0f, 540.0f, 36.0f}, {0.0f, 0.0f, 0.0f}, {200.0f, 1000.0f, 650.0f}},
        /*23*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*24*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*25*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /*26*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*27*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*28*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /*29*/ {{-16.0f, 120.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {120.0f, 230.0f, 210.0f}},
        /*30*/ {{0.0f, 135.0f, -5.0f}, {0.0f, 0.0f, 0.0f}, {100.0f, 260.0f, 175.0f}},
        /*31*/ {{330.0f, 130.0f, 0.0f}, {0.0f, 0.0f, 15.0f}, {700.0f, 95.0f, 215.0f}},
        /*32*/ {{170.0f, 75.0f, 0.0f}, {0.0f, 0.0f, 7.0f}, {330.0f, 75.0f, 315.0f}},
        /*33*/ {{100.0f, 350.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {77.0f, 700.0f, 415.0f}},
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /*34*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*35*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*36*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*37*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*38*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*39*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /**/};
  } /* -=-=-=-*/

  auto GetMeshMaterial_1(int index) -> std::shared_ptr< Material > {
    const std::vector< std::shared_ptr< Material > > wall_materials{
        /* Wall Material */
        AssetManager::GetMaterial("Assets/materials/l1w1.mat"),
        AssetManager::GetMaterial("Assets/materials/l1w2.mat"),
        AssetManager::GetMaterial("Assets/materials/l1w3.mat"),
        AssetManager::GetMaterial("Assets/materials/l1w4.mat"),
        AssetManager::GetMaterial("Assets/materials/l1w5.mat"),
        AssetManager::GetMaterial("Assets/materials/l1w6.mat"),
        /**/};
    const std::shared_ptr< Material > floor_material =
        AssetManager::GetMaterial("Assets/materials/l1f1.mat");

    const std::shared_ptr< Material > crown_material =
        AssetManager::GetMaterial("Assets/materials/l1t1.mat");
    const std::shared_ptr< Material > trunk_material =
        AssetManager::GetMaterial("Assets/materials/l1t2.mat");

    /* If wall */
    auto is_wall  = InRange(index, 3, 8) || InRange(index, 17, 21) || InRange(index, 29, 33);
    auto is_floor = InRange(index, 2, 2) || InRange(index, 10, 16) || InRange(index, 23, 27);
    auto is_tree  = InRange(index, 36, 39);

    if (is_wall) {
      auto r = rand() % wall_materials.size();
      return wall_materials[r];
    } else if (is_floor) {
      return floor_material;
    } else if (is_tree) {
      if (index == 37 || index == 39) {
        /* Tree Trunk */
        return trunk_material;
      } else {
        /* Tree Crown */
        return crown_material;
      }
    }

    return nullptr;
  }
}  // namespace Editor