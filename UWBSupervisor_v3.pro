QT +=  qml
QT += quick
QT += widgets
QT += serialport


#deploiment windows
#windeployqt --release --verbose 2 --qmldir %{CurrentProject:Path}  %{buildDir}\release

CONFIG += c++11
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    qml_object.cpp \
    serialdevice.cpp \
    device.cpp \
    atcommand.cpp \
    test.cpp \
    file_manager.cpp \
    anchorscollections.cpp \
    mat_file.cpp \
    libdl/dl_dxf.cpp \
    libdl/dl_writer_ascii.cpp \
    uwb_config.cpp \
    anchorslist.cpp \
    img_provider.cpp \
    message_system.cpp \
    setting.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    serialdevice.h \
    device.h \
    atcommand.h \
    test.h \
    file_manager.h \
    qml_object.h \
    anchorscollections.h \
    mat_file.h \
    libdl/dl_attributes.h \
    libdl/dl_codes.h \
    libdl/dl_creationadapter.h \
    libdl/dl_creationinterface.h \
    libdl/dl_dxf.h \
    libdl/dl_entities.h \
    libdl/dl_exception.h \
    libdl/dl_extrusion.h \
    libdl/dl_global.h \
    libdl/dl_writer.h \
    libdl/dl_writer_ascii.h \
    uwb_config.h \
    anchorslist.h \
    img_provider.h \
    message_system.h \
    setting.h

