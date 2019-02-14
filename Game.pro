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
        "C:/mingw64/x86_64-w64-mingw32.shared/lib/libpsapi.a"

SOURCES += \
        main.cpp \
        application/Application.cpp \
    models/Model.cpp \
    states/GameState.cpp
        

HEADERS += \
    application/Application.h \
    models/Model.h \
    states/State.h \
    states/GameState.h

DISTFILES += \
    basevertex.txt
