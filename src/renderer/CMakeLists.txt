file(GLOB ren_sources CONFIGURE_DEPENDS *.cpp)
target_sources(space-shooter PRIVATE ${ren_sources})