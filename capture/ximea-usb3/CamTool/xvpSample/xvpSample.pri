macx: XIMEA_DIR=/library/Frameworks/m3api.framework
linux: XIMEA_DIR=/opt/XIMEA
win32: XIMEA_DIR=C:/XIMEA/

macx:DEST_SUBDIR=macx
linux-g++-32:DEST_SUBDIR=linux-x86
linux-g++-64:DEST_SUBDIR=linux-x86_64
linux-g++-jetson-tx2:DEST_SUBDIR=jetson-tx2
win32-g++:DEST_SUBDIR=win32-mingw
win32: contains(QMAKE_TARGET.arch, x86_64){
    DEST_SUBDIR=win_x64
}
win32: !contains(QMAKE_TARGET.arch, x86_64){
    DEST_SUBDIR=win_x86
}

CONFIG(debug, debug|release) {
    DEST_SUBDIR=$$DEST_SUBDIR/debug/
}else {
    DEST_SUBDIR=$$DEST_SUBDIR/release/
}

SAMPLE_ROOT=$$PWD
DESTDIR=$$SAMPLE_ROOT/bin/$$DEST_SUBDIR

linux: LIBS += -L$$XIMEA_DIR/CamTool
win32:contains(QMAKE_TARGET.arch, x86_64){
        LIBS += -L$$SAMPLE_ROOT/lib/win/x64
}
win32:!contains(QMAKE_TARGET.arch, x86_64){
        LIBS += -L$$SAMPLE_ROOT/lib/win/x86
}
LIBS += -L$$DESTDIR -lxiCore -lxiCoreGui
INCLUDEPATH += $$SAMPLE_ROOT/include/xiCore
INCLUDEPATH += $$SAMPLE_ROOT/include/xiCoreGui

#----xiAPI----
macx: INCLUDEPATH += $$XIMEA_DIR/Headers/
linux: INCLUDEPATH += $$XIMEA_DIR/include/
win32: INCLUDEPATH += $$XIMEA_DIR/API/xiAPI