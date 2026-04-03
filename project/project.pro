QT       += core gui widgets sql multimedia
QT       += core gui multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += d:/four_project/Facial_recognition/opencv-SeetaFace-qt5.14.0minGW/opencv452/include \
               d:/four_project/Facial_recognition/opencv-SeetaFace-qt5.14.0minGW/SeetaFace/include

LIBS += -Ld:/four_project/Facial_recognition/opencv-SeetaFace-qt5.14.0minGW/opencv452/x64/mingw/lib \
        -lopencv_core452 -lopencv_imgproc452 -lopencv_imgcodecs452 -lopencv_objdetect452 -lopencv_highgui452 -lopencv_videoio452 \
        -Ld:/four_project/Facial_recognition/opencv-SeetaFace-qt5.14.0minGW/SeetaFace/bin \
        -lSeetaFaceDetector -lSeetaFaceLandmarker -lSeetaFaceRecognizer -lSeetaFaceTracker -lSeetaNet -lSeetaQualityAssessor

SOURCES += \
    face_in.cpp \
    main.cpp \
    login.cpp \
    main_win.cpp \
    register_win.cpp

HEADERS += \
    face_in.h \
    login.h \
    main_win.h \
    register_win.h

FORMS += \
    face_in.ui \
    login.ui \
    main_win.ui \
    register_win.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
# Windows 可执行文件 + 任务栏图标
RC_ICONS = $$PWD/image/logo.ico
