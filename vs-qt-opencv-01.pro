# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = vs-qt-opencv-01
CONFIG(debug, debug|release):{
DESTDIR = ./x64/Debug
LIBS += -L"." \
    -lC:/Download/OfficeSoftware/OpenCV/opencv-4.9.0-windows/opencv/build/x64/vc16/lib/opencv_world490d
DEPENDPATH += .
MOC_DIR += GeneratedFiles/$(ConfigurationName)
OBJECTS_DIR += debug
UI_DIR += GeneratedFiles
RCC_DIR += GeneratedFiles
include(vs-qt-opencv-01.pri)
}
else:CONFIG(release, debug|release):{
DESTDIR = ./x64/Release
LIBS += -L"." \
    -lC:/Download/OfficeSoftware/OpenCV/opencv-4.9.0-windows/opencv/build/x64/vc16/lib/opencv_world490
DEPENDPATH += .
MOC_DIR += GeneratedFiles/$(ConfigurationName)
OBJECTS_DIR += release
UI_DIR += GeneratedFiles
RCC_DIR += GeneratedFiles
include(vs-qt-opencv-01.pri)
}
