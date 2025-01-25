file(GLOB input_sources CONFIGURE_DEPENDS *.cpp)
target_sources(space-shooter PRIVATE ${input_sources})