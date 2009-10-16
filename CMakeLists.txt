INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

QT4_WRAP_UI(UI_SOURCE
  MainGUI.ui
)
QT4_WRAP_CPP(MOC_SOURCE
  MainGUI.h
)

ADD_EXECUTABLE(AssetViewer
  main.cpp
  MainGUI.cpp
  RenderingView.h
  RenderingView.cpp
  GridNode.h
  GridNode.cpp
  ${MOC_SOURCE}
  ${UI_SOURCE}
)

TARGET_LINK_LIBRARIES(AssetViewer
  OpenEngine_Core
  OpenEngine_Utils
  # Extensions
  Extensions_SetupHelpers
  Extensions_Qt
  Extensions_OpenGLSelection
  Extensions_SDLFont
  ${BOOST_SERIALIZATION_LIB}
)
