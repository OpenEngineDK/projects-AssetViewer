
# Project source code list
SET( ASSETVIEWER_SOURCES
  # Add all the cpp source files here
  main.cpp
  GameFactory.cpp
)

# todo get rid of this!@#!
# Include if using SDL under Mac OS X
IF(APPLE)
  SET(ASSETVIEWER_SOURCES ${ASSETVIEWER_SOURCES} ${SDL_MAIN_FOR_MAC})
ENDIF(APPLE)

# Project executable
ADD_EXECUTABLE(AssetViewer ${ASSETVIEWER_SOURCES})

# Project dependencies
TARGET_LINK_LIBRARIES(AssetViewer
  OpenEngine_Core
  OpenEngine_Logging
  OpenEngine_Display
  OpenEngine_Devices
  OpenEngine_Renderers
  OpenEngine_Resources
  OpenEngine_Scene
  OpenEngine_Utils
  # Extensions
  Extensions_SDL
  Extensions_ColladaResource
  Extensions_GenericHandlers
  Extensions_TGAResource
  Extensions_OBJResource
  Extensions_FixedTimeStepPhysics
  ${BOOST_SERIALIZATION_LIB}
)
