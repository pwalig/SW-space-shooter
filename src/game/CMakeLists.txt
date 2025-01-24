file(GLOB game_sources CONFIGURE_DEPENDS *.cpp)
target_sources(space-shooter PRIVATE ${game_sources})