cmake_minimum_required(VERSION 3.20)
project(SoulKnightClone)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio)

add_executable(SoulKnightClone
        main.cpp
        Game.cpp Game.h
        State.h
        MenuState.cpp MenuState.h
        PlayingState.cpp PlayingState.h
        PauseState.cpp PauseState.h
        EndState.cpp EndState.h
        Player.cpp Player.h
        Weapon.cpp Weapon.h
        Projectile.cpp Projectile.h
        Enemy.cpp Enemy.h
        Boss.cpp Boss.h
        Map.cpp Map.h
        UI.cpp UI.h
        Quiz.cpp Quiz.h
)

target_include_directories(SoulKnightClone PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(SoulKnightClone PRIVATE SFML::Graphics SFML::Audio)

if(WIN32)
    add_custom_command(TARGET SoulKnightClone POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_DIR}/../../../bin/sfml-graphics-3.dll"
            "${SFML_DIR}/../../../bin/sfml-window-3.dll"
            "${SFML_DIR}/../../../bin/sfml-system-3.dll"
            "${SFML_DIR}/../../../bin/sfml-audio-3.dll"
            $<TARGET_FILE_DIR:SoulKnightClone>)
endif()
