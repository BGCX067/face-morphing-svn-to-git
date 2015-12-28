// stdafx.h

// Ogre and CEGUI headers you need and most likely you wont modify often 
// or at all. 
#include <Ogre.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <cv.h>
#include <cvaux.h>
#include <cxcore.h>
#include <highgui.h>
/*
#include <SdkTrays.h>
#include <SdkCameraMan.h>

#include <cegui/include/CEGUI.h>
#include <CEGUI/include/CEGUISystem.h>
#include <CEGUI/include/CEGUISchemeManager.h>
#include <CEGUI/include/RendererModules/Ogre/CEGUIOgreRenderer.h>
*/
// any other header can be included, as usual
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif