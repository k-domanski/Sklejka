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

  auto ShouldIgnore_2(int index) -> bool {
    /* Ignore Indexes */
    /* 0-1, 5-6, 25-26, 32, 39, 47, 49 - 53, 56, 63, 68, 73 - 75 */
    return InRange(index, 0, 1) || InRange(index, 5, 6) || InRange(index, 25, 26)
           || InRange(index, 32, 32) || InRange(index, 39, 39) || InRange(index, 47, 47)
           || InRange(index, 49, 53) || InRange(index, 56, 56) || InRange(index, 63, 63)
           || InRange(index, 68, 68) || InRange(index, 73, 75);
  }

  auto IsTree_2(int index) -> bool {
    /* 54-55, */
    return InRange(index, 54, 55);
  }

  auto TreeCollider_2() -> std::vector< TreeData > {
    return std::vector< TreeData >{
        /* D A T A */

        {/* 54 */
         /* Crown */ {{0.0f, 2.1f, 1.3f}, {28.6f, 0.0f, 0.0f}, {0.9f, 6.0f, 1.5f}},
         /* Trunk*/ {{0.7f, 2.1f, -1.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 3.5f}}
         /* -=-=- */},
        {/* 55 */
         /* Crown */ {{0.7f, 1.1f, -0.4f}, {-13.8f, 0.0f, 0.0f}, {2.1f, 4.9f, 1.0f}},
         /* Trunk*/ {{1.1f, 1.1f, 5.6f}, {0.0f, 0.0f, 0.0f}, {0.8f, 0.8f, 7.6f}}
         /* -=-=- */}

    };
  }

  auto TreeColliderIndex_2(int index) -> int {
    std::map< int, int > mp;
    mp[54] = 0;
    mp[55] = 1;

    return mp[index];
  }

  auto GetColliderMapping_2() -> std::vector< TransformData > {
    return std::vector< TransformData >{
        /* D A T A */
        /* Ind */ /* Position */ /*   Rotation   */ /*   Scale   */
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /*0*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*1*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*2*/ {{0.7f, 0.0f, 2.5f}, {0.0f, 0.0f, 0.0f}, {3.8f, 10.6f, 4.8f}},
        /*3*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*4*/ {{0.7f, 0.0f, 2.5f}, {0.0f, -0.6f, 0.0f}, {3.8f, 9.9f, 4.8f}},
        /*5*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*6*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*7*/ {{78.0f, 350.0f, -17.0f}, {0.0f, 0.0f, 0.0f}, {120.0f, 650.0f, 370.0f}},
        /*8*/ {{78.0f, 350.0f, -17.0f}, {0.0f, 0.0f, 0.0f}, {120.0f, 650.0f, 370.0f}},
        /*9*/ {{78.0f, 350.0f, -17.0f}, {0.0f, 0.0f, 0.0f}, {120.0f, 650.0f, 370.0f}},
        /*10*/ {{62.0f, 550.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {190.0f, 1100.0f, 650.0f}},
        /*11*/ {{62.0f, 550.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {190.0f, 1100.0f, 650.0f}},
        /*12*/ {{62.0f, 550.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {190.0f, 1100.0f, 650.0f}},
        /*13*/ {{120.0f, 580.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {250.0f, 1200.0f, 485.0f}},
        /*14*/ {{120.0f, 580.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {250.0f, 1200.0f, 485.0f}},
        /*15*/ {{120.0f, 580.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {250.0f, 1200.0f, 485.0f}},
        /*16*/ {{0.0f, 210.0f, 7.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 380.0f, 230.0f}},
        /*17*/ {{0.0f, 210.0f, 7.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 380.0f, 230.0f}},
        /*18*/ {{0.0f, 210.0f, 7.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 380.0f, 230.0f}},
        /*19*/ {{0.0f, 210.0f, 7.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 380.0f, 230.0f}},
        /*20*/ {{0.0f, 210.0f, 7.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 380.0f, 230.0f}},
        /*21*/ {{0.0f, 210.0f, 7.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 380.0f, 230.0f}},
        /*22*/ {{52.0f, 210.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 435.0f, 260.0f}},
        /*23*/ {{52.0f, 210.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 435.0f, 260.0f}},
        /*24*/ {{52.0f, 210.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 435.0f, 260.0f}},
        /*25*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*26*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*27*/ {{78.0f, 350.0f, -17.0f}, {0.0f, 0.0f, 0.0f}, {120.0f, 650.0f, 370.0f}},
        /*28*/ {{62.0f, 550.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {190.0f, 1100.0f, 650.0f}},
        /*29*/ {{120.0f, 580.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {250.0f, 1200.0f, 485.0f}},
        /*30*/ {{0.0f, 210.0f, 7.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 380.0f, 230.0f}},
        /*31*/ {{52.0f, 210.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 435.0f, 260.0f}},
        /*32*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*33*/ {{0.0f, 0.7f, 6.0f}, {-6.7f, 0.0f, 0.0f}, {8.0f, 0.2f, 30.0f}},
        /*34*/ {{78.0f, 350.0f, -17.0f}, {0.0f, 0.0f, 0.0f}, {120.0f, 650.0f, 370.0f}},
        /*35*/ {{62.0f, 550.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {190.0f, 1100.0f, 650.0f}},
        /*36*/ {{120.0f, 580.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {250.0f, 1200.0f, 485.0f}},
        /*37*/ {{0.0f, 210.0f, 7.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 380.0f, 230.0f}},
        /*38*/ {{52.0f, 210.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 435.0f, 260.0f}},
        /*39*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*40*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*41*/ {{78.0f, 350.0f, -17.0f}, {0.0f, 0.0f, 0.0f}, {120.0f, 650.0f, 370.0f}},
        /*42*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*43*/ {{62.0f, 550.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {190.0f, 1100.0f, 650.0f}},
        /*44*/ {{120.0f, 580.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {250.0f, 1200.0f, 485.0f}},
        /*45*/ {{0.0f, 210.0f, 7.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 380.0f, 230.0f}},
        /*46*/ {{52.0f, 210.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {150.0f, 435.0f, 260.0f}},
        /*47*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*48*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*49*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*50*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*51*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*52*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*53*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*54*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*55*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*56*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*57*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*58*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*59*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*60*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*61*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*62*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}},
        /*63*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*64*/ {{-16.0f, 120.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {120.0f, 230.0f, 210.0f}},
        /*65*/ {{0.0f, 135.0f, -5.0f}, {0.0f, 0.0f, 0.0f}, {100.0f, 260.0f, 175.0f}},
        /*66*/ {{330.0f, 130.0f, 0.0f}, {0.0f, 0.0f, 15.0f}, {700.0f, 95.0f, 215.0f}},
        /*67*/ {{170.0f, 75.0f, 0.0f}, {0.0f, 0.0f, 7.0f}, {330.0f, 75.0f, 315.0f}},
        /*68*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*69*/ {{1.1f, 1.1f, -0.3f}, {-14.0f, 0.0f, 0.0f}, {2.2f, 5.4f, 0.6f}},
        /*70*/ {{1.0f, 1.2f, 4.8f}, {-4.5f, 0.0f, 0.0f}, {1.0f, 1.0f, 11.4f}},
        /*71*/ {{0.0f, 2.3f, 1.4f}, {22.8f, 0.0f, 0.0f}, {2.2f, 5.4f, 0.6f}},
        /*72*/ {{1.0f, 1.7f, -1.5f}, {12.6f, 0.0f, 0.0f}, {0.8f, 0.8f, 3.9f}},
        /*73*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*74*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        /*75*/ {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /**/};
  }

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
  auto GetMeshMaterial_2(int index) -> std::shared_ptr< Material > {
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

    const std::shared_ptr< Material > tree_material =
        AssetManager::GetMaterial("Assets/materials/tree_l2.mat");

/* Wall */
#define IR(_a, _b) (InRange(index, _a, _b))
    auto is_wall = IR(7, 24) || IR(27, 31) || IR(34, 38) || IR(41, 46) || IR(64, 67);
    /* Floor */
    auto is_floor =
        IR(3, 3) || IR(6, 6) || IR(26, 26) || IR(33, 33) || IR(40, 40) || IR(48, 52) || IR(57, 62);
    /* Tree */
    /* Crown: 69, 71 | Trunk: 70, 72*/
    auto is_tree = IR(69, 72);
    /* OneMeshTree */
    auto is_one_mesh_tree = IsTree_2(index) || IR(2, 2) || IR(4, 4);

    if (is_wall) {
      auto r = rand() % wall_materials.size();
      return wall_materials[r];
    } else if (is_floor) {
      return floor_material;
    } else if (is_tree) {
      if (index == 70 || index == 72) {
        /* Tree Trunk */
        return trunk_material;
      } else {
        /* Tree Crown */
        return crown_material;
      }
    } else if (is_one_mesh_tree) {
      return tree_material;
    }

    return nullptr;
  }
}  // namespace Editor