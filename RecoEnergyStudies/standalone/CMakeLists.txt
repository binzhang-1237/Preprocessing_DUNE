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

# Give field-response work histograms distinct names in the local service.
# Generate a patched copy in the build directory; CVMFS sources stay read-only.
find_path(RECO_DUNECORE_INCLUDE dunecore/Utilities/SignalShapingServiceDUNE.h
    HINTS "$ENV{DUNECORE_INC}" "$ENV{DUNECORE_DIR}/include" REQUIRED)
get_filename_component(RECO_DUNECORE_PREFIX "${RECO_DUNECORE_INCLUDE}" DIRECTORY)
set(RECO_SIGNAL_SOURCE "${RECO_DUNECORE_PREFIX}/source/dunecore/Utilities/SignalShapingServiceDUNE_service.cc")
set(RECO_SIGNAL_PATCH "${RECOENERGY_STUDIES_SOURCE}/patches/SignalShapingServiceDUNE_histogram_names.patch")
set(RECO_SIGNAL_GENERATED "${CMAKE_CURRENT_BINARY_DIR}/SignalShapingServiceDUNE_service.cc")
find_program(RECO_PATCH_EXECUTABLE patch REQUIRED)
add_custom_command(OUTPUT "${RECO_SIGNAL_GENERATED}"
    COMMAND "${RECO_PATCH_EXECUTABLE}" --batch --output "${RECO_SIGNAL_GENERATED}"
            "${RECO_SIGNAL_SOURCE}" "${RECO_SIGNAL_PATCH}"
    DEPENDS "${RECO_SIGNAL_SOURCE}" "${RECO_SIGNAL_PATCH}"
    VERBATIM)
add_library(SignalShapingServiceDUNE_service SHARED "${RECO_SIGNAL_GENERATED}")
target_include_directories(SignalShapingServiceDUNE_service PRIVATE "${RECO_DUNECORE_INCLUDE}")
get_target_property(RECO_SIGNAL_LIBRARIES RecoEnergyS_module LINK_LIBRARIES)
target_link_libraries(SignalShapingServiceDUNE_service PRIVATE
    ${RECO_SIGNAL_LIBRARIES} ROOT::Hist ROOT::RIO lardata_Utilities_LArFFT)
