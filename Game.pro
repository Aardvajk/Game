TEMPLATE = app
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES -= UNICODE

INCLUDEPATH += "C:/Projects/Gx/Gx" \
               "C:/Projects/pcx" \
               "C:/lib/Bullet/src"

LIBS += "C:/Projects/Gx/build-Gx/release/libgx.a" \
        "C:/Projects/pcx/build-pcx/release/libpcx.a"

QMAKE_CXXFLAGS += -std=gnu++11 \
                  -include "GxCore/GxDebug.h"

QMAKE_LFLAGS += -Wl,--exclude-all-symbols

QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas -Wno-comment -Wno-maybe-uninitialized -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-attributes

PRE_TARGETDEPS += "C:/Projects/Gx/build-Gx/release/libgx.a" \
                  "C:/Projects/pcx/build-pcx/release/libpcx.a"

LIBS += "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3d9.lib" \
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3dx9.lib" \
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/dxerr.lib" \
        "C:/mingw64/x86_64-w64-mingw32.shared/lib/libuser32.a" \
        "C:/mingw64/x86_64-w64-mingw32.shared/lib/libgdi32.a" \
        "C:/mingw64/x86_64-w64-mingw32.shared/lib/libpsapi.a" \
        "C:/lib/Bullet/build-Bullet/release/libBullet.a"

SOURCES += \
        main.cpp \
        application/Application.cpp \
    states/GameState.cpp \
    graphics/Graphics.cpp \
    graphics/vertices/MeshVertex.cpp \ 
    debug/DebugMesh.cpp \
    models/Model.cpp \
    scene/Scene.cpp \
    scene/nodes/StaticMeshNode.cpp \
    graphics/VertexBuffer.cpp \
    graphics/VertexStream.cpp \
    entities/pc/Pc.cpp \
    maths/Kcc.cpp \
    application/Events.cpp \
    maths/Camera.cpp \
    debug/DebugRender.cpp \
    graphics/vertices/ColorVertex.cpp \
    application/ResourcePath.cpp \
    graphics/vertices/TextureVertex.cpp \
    states/State.cpp \
    entities/Entity.cpp \
    application/ShaderCompiler.cpp \
    application/FpsCounter.cpp \
    debug/DebugText.cpp \
    debug/MemUsage.cpp \
    scene/RenderKey.cpp \
    entities/TestShape.cpp \
    scene/SceneParams.cpp \
    graphics/vertices/PositionVertex.cpp \
    scene/nodes/SkyBoxNode.cpp \
    graphics/SkyBox.cpp \
    graphics/vertices/RigVertex.cpp \
    scene/nodes/RigMeshNode.cpp
        

HEADERS += \
    application/Application.h \
    states/State.h \
    states/GameState.h \
    graphics/Graphics.h \
    graphics/vertices/MeshVertex.h \ 
    debug/DebugMesh.h \
    models/Model.h \
    scene/Scene.h \
    scene/nodes/SceneNode.h \
    scene/nodes/StaticMeshNode.h \
    graphics/VertexBuffer.h \
    scene/SceneParams.h \
    graphics/VertexStream.h \
    entities/pc/Pc.h \
    entities/Entity.h \
    maths/Kcc.h \
    application/Events.h \
    maths/Camera.h \
    debug/DebugRender.h \
    graphics/vertices/ColorVertex.h \
    application/AppParams.h \
    application/ResourcePath.h \
    graphics/vertices/TextureVertex.h \
    application/FrameParams.h \
    scene/RenderTypes.h \
    application/ShaderCompiler.h \
    application/FpsCounter.h \
    debug/DebugText.h \
    debug/MemUsage.h \
    scene/RenderKey.h \
    entities/TestShape.h \
    graphics/vertices/PositionVertex.h \
    scene/nodes/SkyBoxNode.h \
    graphics/SkyBox.h \
    graphics/vertices/RigVertex.h \
    scene/nodes/RigMeshNode.h \
    maths/FadeValue.h

DISTFILES += \
    assets/shaders/colorvertex.txt \
    assets/shaders/depthvertex.txt \
    assets/shaders/screenvertex.txt \
    assets/shaders/depthpixel.txt \
    assets/shaders/unpackpixel.txt \
    assets/shaders/meshvertex.txt \
    assets/shaders/skyboxpixel.txt \
    assets/shaders/skyboxvertex.txt \
    assets/shaders/rigdepthvertex.txt \
    assets/shaders/rigvertex.txt \
    assets/shaders/litpixel.txt

