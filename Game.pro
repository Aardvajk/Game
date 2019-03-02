TEMPLATE = app
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES -= UNICODE

INCLUDEPATH += "C:/Projects/Gx/Gx" \
               "C:/Projects/pcx"

LIBS += "C:/Projects/Gx/build-Gx/release/libgx.a" \
        "C:/Projects/pcx/build-pcx/release/libpcx.a"

QMAKE_CXXFLAGS += -std=gnu++11
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
    debug/DebugRender.cpp \
    graphics/VertexStream.cpp \
    entities/pc/Pc.cpp
        

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
    debug/DebugRender.h \
    graphics/VertexStream.h \
    entities/pc/Pc.h \
    entities/Entity.h

DISTFILES += \
    meshvertex.txt
