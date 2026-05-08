cmake_minimum_required(VERSION 3.6)

set(APP_NAME CardGame)

project(${APP_NAME})

if(NOT DEFINED BUILD_ENGINE_DONE)
    set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d-x-3.17)
    if(NOT EXISTS ${COCOS2DX_ROOT_PATH})
        set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cocos2d-x-3.17)
    endif()
    set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif()

set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR VS)
    cocos_mark_multi_resources(cc_common_res RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

list(APPEND GAME_HEADER
     Classes/AppDelegate.h
     Classes/configs/models/CardTypeDef.h
     Classes/configs/models/LevelConfig.h
     Classes/configs/models/CardResConfig.h
     Classes/configs/loaders/LevelConfigLoader.h
     Classes/models/CardModel.h
     Classes/models/GameModel.h
     Classes/models/UndoModel.h
     Classes/views/CardView.h
     Classes/views/GameView.h
     Classes/controllers/GameController.h
     Classes/controllers/CardController.h
     Classes/managers/UndoManager.h
     Classes/services/GameModelFromLevelGenerator.h
     Classes/services/CardMatchService.h
     Classes/utils/CardUtils.h
     )
list(APPEND GAME_SOURCE
     Classes/AppDelegate.cpp
     Classes/configs/loaders/LevelConfigLoader.cpp
     Classes/configs/models/CardResConfig.cpp
     Classes/models/CardModel.cpp
     Classes/models/GameModel.cpp
     Classes/models/UndoModel.cpp
     Classes/views/CardView.cpp
     Classes/views/GameView.cpp
     Classes/controllers/GameController.cpp
     Classes/controllers/CardController.cpp
     Classes/managers/UndoManager.cpp
     Classes/services/GameModelFromLevelGenerator.cpp
     Classes/services/CardMatchService.cpp
     Classes/utils/CardUtils.cpp
     )

if(ANDROID)
    set(APP_NAME card_game)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         ${cc_common_res}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios/AppController.h
             proj.ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios/LaunchScreen.storyboard
            proj.ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios/main.m
             proj.ios/AppController.mm
             proj.ios/RootViewController.mm
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.mac/Icon.icns
            proj.mac/Info.plist
            )
        list(APPEND GAME_SOURCE
             proj.mac/main.cpp
             ${APP_UI_RES}
             )
    endif()
endif()

if(WINDOWS)
    add_executable(${APP_NAME} WIN32 ${GAME_SOURCE} ${GAME_HEADER})
else()
    add_executable(${APP_NAME} ${GAME_SOURCE} ${GAME_HEADER})
endif()

target_link_libraries(${APP_NAME} cocos2d)

target_include_directories(${APP_NAME} PRIVATE
    Classes
    ${COCOS2DX_ROOT_PATH}/cocos
)

add_custom_command(TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    "$<TARGET_FILE_DIR:${APP_NAME}>/Resources"
)