cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

find_package(cetmodules 3.12.00 REQUIRED)
project(RecoEnergyStudiesStandalone LANGUAGES CXX)

include(CetCMakeEnv)
cet_cmake_env()

find_package(art REQUIRED EXPORT)
find_package(art_root_io REQUIRED)
find_package(canvas REQUIRED EXPORT)
find_package(canvas_root_io REQUIRED EXPORT)
find_package(ROOT REQUIRED)
find_package(larcore REQUIRED EXPORT)
find_package(larcoreobj REQUIRED EXPORT)
find_package(larcorealg REQUIRED EXPORT)
find_package(larreco REQUIRED EXPORT)
find_package(larevt REQUIRED EXPORT)
find_package(lardata REQUIRED EXPORT)
find_package(lardataalg REQUIRED EXPORT)
find_package(lardataobj REQUIRED EXPORT)
find_package(larsim REQUIRED EXPORT)
find_package(larpandora REQUIRED EXPORT)
find_package(nusimdata REQUIRED EXPORT)
find_package(nutools REQUIRED EXPORT)
find_package(nug4 REQUIRED EXPORT)
find_package(messagefacility REQUIRED EXPORT)
find_package(cetlib REQUIRED EXPORT)
find_package(cetlib_except REQUIRED EXPORT)
find_package(Boost REQUIRED)
find_package(Range-v3 REQUIRED)

link_directories(
    ${art_LIBRARY_DIRS}
    ${art_root_io_LIBRARY_DIRS}
    ${canvas_LIBRARY_DIRS}
    ${canvas_root_io_LIBRARY_DIRS}
    ${larcore_LIBRARY_DIRS}
    ${larcoreobj_LIBRARY_DIRS}
    ${larcorealg_LIBRARY_DIRS}
    ${larreco_LIBRARY_DIRS}
    ${larevt_LIBRARY_DIRS}
    ${lardata_LIBRARY_DIRS}
    ${lardataalg_LIBRARY_DIRS}
    ${lardataobj_LIBRARY_DIRS}
    ${larsim_LIBRARY_DIRS}
    ${larpandora_LIBRARY_DIRS}
    ${nusimdata_LIBRARY_DIRS}
    ${nutools_LIBRARY_DIRS}
    ${nug4_LIBRARY_DIRS}
    ${messagefacility_LIBRARY_DIRS}
    ${cetlib_LIBRARY_DIRS}
    ${cetlib_except_LIBRARY_DIRS}
    ${ROOT_LIBRARY_DIR}
)

include_directories(SYSTEM
    "${CMAKE_BINARY_DIR}"
    ${art_INCLUDE_DIRS}
    ${art_root_io_INCLUDE_DIRS}
    ${canvas_INCLUDE_DIRS}
    ${canvas_root_io_INCLUDE_DIRS}
    ${larcore_INCLUDE_DIRS}
    ${larcoreobj_INCLUDE_DIRS}
    ${larcorealg_INCLUDE_DIRS}
    ${larreco_INCLUDE_DIRS}
    ${larevt_INCLUDE_DIRS}
    ${lardata_INCLUDE_DIRS}
    ${lardataalg_INCLUDE_DIRS}
    ${lardataobj_INCLUDE_DIRS}
    ${larsim_INCLUDE_DIRS}
    ${larpandora_INCLUDE_DIRS}
    ${nusimdata_INCLUDE_DIRS}
    ${nutools_INCLUDE_DIRS}
    ${nug4_INCLUDE_DIRS}
    ${messagefacility_INCLUDE_DIRS}
    ${cetlib_INCLUDE_DIRS}
    ${cetlib_except_INCLUDE_DIRS}
    ${ROOT_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${Range-v3_INCLUDE_DIRS}
    "${LARSOFT_DUNE_SOURCE_DIR}"
    "${LARSOFT_DUNE_BUILD_DIR}"
)

include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

add_subdirectory("${RECOENERGY_STUDIES_SOURCE}" RecoEnergyStudies)
