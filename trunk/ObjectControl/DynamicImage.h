#include "stdafx.h"

class DynamicImage
{
public:
	DynamicImage::DynamicImage(Ogre::SceneManager* _mSceneMgr, 
		Ogre::Camera* _mCamera, 
		Ogre::String _nodeName, 
		Ogre::Vector3 _nodePosition,
		Ogre::Vector3 _drawColor,
		//Ogre::Vector3 _nodeScale,
		Ogre::String _materialName,
		Ogre::String _manualObjectName,
		Ogre::String _tempImageStr,
		Ogre::Vector2 _imageSize,
		bool _drawable,
		bool _controllable,
		int _sn);

	~DynamicImage(void);

	void openImage( const char* filePath );
	void saveImage(const char *filename );
	void renew();
	void mouseMoved(const OIS::MouseEvent &arg);
	int mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void getTouchPosition(const OIS::MouseEvent &arg);
	void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void drawRedDot();
	bool isTouched(const OIS::MouseEvent &arg, bool needDraw);
	void undo();
	void init();
	void nodeMoved(Ogre::Vector3 newPosition);
	void nodeScale(Ogre::Vector2 newSize);
	void reload();
	bool drawable;
	bool controllable;

	Ogre::Vector2 imageRatio;
	Ogre::Vector2 drawPoint;

	Ogre::Image mImageSrc, mImageSrcSecond;
	Ogre::Image mImageDst;

	Ogre::String mPath, mPathSecond; // for recover image
	bool bAlt, bMidMouseDown, bLMouseDown, bRMouseDown;	//true if mouse buttons are held down

	unsigned char* img_data,*original_data;
	int img_width;
	int img_height;

	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::Viewport *mCamView;

	Ogre::Vector3 nodePosition;
	Ogre::Vector3 drawColor;
	//Ogre::Vector3 nodeScale;
	Ogre::Vector2 imageSize;

	Ogre::String nodeName;
	Ogre::String materialName;
	Ogre::String manualObjectName;
	Ogre::String tempImageStr;

	enum {bb, gg, rr, aa}; 
	int sn;

	std::vector<Ogre::Vector2> points;
};
