/******************************************************

  Project 1: Image Editor in OGRE

  National Taiwan University of Science and Technology
  Computer Game, Animation, and Graphics Laboratory

  Professor: Yu-Chi Lai

  Student:  Dobromir Todorov, Chun-Wei Wang, Damody		

******************************************************/

#pragma once
#include "stdafx.h"
#include "BaseApplication.h"
#include "Interface.h"
#include "ClassDefine.h"
#include "DynamicImage.h"

class Stroke { // Data structure for holding painterly strokes.
public:
	Stroke(void);
	Stroke(unsigned int radius, unsigned int x, unsigned int y,
		unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	// data
	unsigned int radius, x, y;	// Location for the stroke
	unsigned char r, g, b, a;	// Color
};

class Main : public BaseApplication
{
public:
    Main(void);
    virtual ~Main(void);

	int numObjects;
	Ogre::Viewport *mCamView;
	Ogre::Viewport* mViewPort;
	Ogre::SceneNode *mCamNode;   // The SceneNode the camera is currently attached to
	Ogre::Plane mPlane;	//Invisible plane on which we do the mouse dragging
	Ogre::SceneNode *mCurrentObject;	//pointer to our currently selected object

	Interface *mInterface;

	unsigned char* Main::To_RGB(void);
	void Main::RGBA_To_RGB(unsigned char *rgba, unsigned char *rgb);

/*
	// File
	void loadSecondaryImge(const char* filePath);
	void openImage(const char* filePath);
	void saveImage(const char *filename);

	// Color
	void Gray();
	void Redo();
	//void resample_src(int u, int v, float ww, unsigned char* rgba);
	*/
	void imageMorphing(float percentageOfLeft);
	void imageMorphing(int numPics);
	void imageMorphing(int numPics, float percentageOfLeft);
	std::vector<int> undoVector;
	void Reload();
	void Undo();

protected:

    void createScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual void createFrameListener(void);
	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );
	Ogre::Vector3 mDirection;     // Value to move in the correct direction
	
	Ogre::Real mMove;            // The movement constant
	Ogre::Real mRotate;            // The movement constant
	bool bAlt, bMidMouseDown, bLMouseDown, bRMouseDown, uKey,zKey,rKey;	//true if mouse buttons are held down

	bool yKeyPressed;	//Flag for moving objects in Y direction
	bool xKeyPressed;	//Flag for moving objects in XZ direction
	Ogre::Vector3 realPoint;

// 	void renew();
//	bool getTouchPosition(float mouseScreenX, float mouseScreenY);


	// draw function
	void getTouchPosition(const OIS::MouseEvent &arg);
	Ogre::RaySceneQuery* mRaySceneQuery;
	Ogre::Vector2 imageRatio;
	Ogre::Vector2 drawPoint;
	void Main::drawRedDot();
	void drawSampleImage();
	CvMat* transformMat(std::vector<Ogre::Vector2> p1, std::vector<Ogre::Vector2> p2, float percent);
	void transformImage(IplImage* backgroundImage, IplImage* poseImage);
	void transformImage(IplImage* src, IplImage* dst, CvMat* transMat);
	void showImage(IplImage* img, char* windowName);
	bool needDraw;

	bool bRenew;

	DynamicImage* leftFace;
	DynamicImage* rightFace;
	DynamicImage* resultFace;
/*
	Ogre::Image mImageSrc, mImageSrcSecond;
	Ogre::Image mImageDst;
	Ogre::Image mSampleImage;
	//OpenCVImage* cvImageSrc;
	Ogre::String mPath, mPathSecond; // for recover image
*/

	// Image data
/*
	unsigned char* img_data,*img_data2;
	int img_width,img_width2;
	int img_height,img_height2;
*/
	
	// you should swap the rr and bb if you read img_data directly without using To_RGB(), cause OGRE is in BGR format
	enum {bb, gg, rr, aa}; 
	enum {BLACK = 0, WHITE = 255};
};