#include "stdafx.h"
#include "DynamicImage.h"

using namespace Ogre;

DynamicImage::DynamicImage(SceneManager* _mSceneMgr, 
	Camera* _mCamera, 
	String _nodeName, 
	Vector3 _nodePosition,
	Vector3 _drawColor,
	//Vector3 _nodeScale,
	String _materialName,
	String _manualObjectName,
	String _tempImageStr,
	Vector2 _imageSize,
	bool _drawable,
	bool _controllable,
	int _sn)
{
	mSceneMgr = _mSceneMgr;
	mCamera = _mCamera;
	drawColor = _drawColor;
	nodeName = _nodeName;
	nodePosition = _nodePosition;
	//nodeScale = _nodeScale;
	materialName = _materialName;
	manualObjectName = _manualObjectName;
	tempImageStr = _tempImageStr;
	imageSize = _imageSize;
	bAlt = false;
	bMidMouseDown = false;
	bLMouseDown = false;
	bRMouseDown = false;
	drawable = _drawable;
	controllable = _controllable;
	sn = _sn;

	mCamView = mCamera->getViewport();
}
void DynamicImage::init()
{

}
void DynamicImage::openImage( const char* filePath )
{

	std::cout << "haha" << std::endl;

	if (filePath == "")
	{
		mImageSrc.load(mPath, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		mImageDst.load(mPath, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		//const char* tempString = mPath.c_str();
		//cvImageSrc = new OpenCVImage("..\\..\\media\\materials\\textures\\tmp.png", "XD");
		//cvImageSrc = new OpenCVImage(tempString, "XD");
		renew();
	}
	else
	{
		mImageSrc.load(filePath, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		mImageDst.load(filePath, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		//cvImageSrc = new OpenCVImage(filePath, "XD");

		//cvImageSrc = new OpenCVImage("..\\..\\media\\materials\\textures\\tmp.png", "XD");

		mPath = filePath;
		renew();
	}

	std::cout << "\n" << mPath << "\n" << std::endl;		

	img_data = mImageSrc.getData();
	img_width = mImageSrc.getWidth();
	img_height = mImageSrc.getHeight();
	original_data = new unsigned char[img_height*img_width*4];
	for(int i=0; i<img_width*img_height*4; i++)
	{
		original_data[i] = img_data[i];
	}
}

void DynamicImage::saveImage(const char *filename )
{
	printf("\nSave Image: %s\n", filename);
	mImageDst.save(filename);
}
void DynamicImage::renew()
{
	int debugFlag = 0;
	Ogre::Vector3 originalScale;
	Ogre::Vector3 originalPosition;
	if (mSceneMgr->hasSceneNode(nodeName))
	{
		originalScale = mSceneMgr->getSceneNode(nodeName)->getScale();
		originalPosition = mSceneMgr->getSceneNode(nodeName)->getPosition();
		mSceneMgr->destroySceneNode(nodeName);
	}
	else
	{
		printf("init size\n");
		originalPosition = nodePosition;
		originalScale.x = 1;
		originalScale.y = 1;
	}
	if (mSceneMgr->hasManualObject(manualObjectName))
		mSceneMgr->destroyManualObject(manualObjectName);

	Ogre::String str = tempImageStr;
	Ogre::MaterialManager::getSingleton().remove(materialName);
	Ogre::TextureManager::getSingleton().remove(str); 
	/*
	mImageDst.save(str);


	Ogre::TexturePtr mOgreTexture = Ogre::TextureManager::getSingleton().loadImage(str, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mImageDst);

	Ogre::MaterialPtr mMaterial = Ogre::MaterialManager::getSingleton().create(materialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mMaterial->getTechnique(0)->getPass(0)->setCullingMode(Ogre::CullingMode::CULL_NONE);
	mMaterial->setReceiveShadows(false);
	mMaterial->setLightingEnabled(false);
	mMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(mOgreTexture->getName());
	*/

	Ogre::TexturePtr mOgreTexture = TextureManager::getSingleton().create(str, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); 
	mOgreTexture->loadImage(mImageSrc);

	Ogre::MaterialPtr mMaterial = Ogre::MaterialManager::getSingleton().create(materialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mMaterial->getTechnique(0)->getPass(0)->setCullingMode(Ogre::CullingMode::CULL_NONE);
	mMaterial->setReceiveShadows(false);
	mMaterial->setLightingEnabled(false);
	mMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(str);


	Ogre::ManualObject *manual = mSceneMgr->createManualObject(manualObjectName);
	manual->setUseIdentityProjection(true);
	manual->setUseIdentityView(true);
	manual->clear();
	manual->begin(materialName, Ogre::RenderOperation::OT_TRIANGLE_LIST);

	Vector2 ratio(mOgreTexture->getWidth() / ((float)mCamView->getActualWidth()), mOgreTexture->getHeight() / ((float)mCamView->getActualHeight()));	

	if (ratio.y >= imageSize.y || ratio.x >= imageSize.x)
	{
		if (mOgreTexture->getWidth()  / ((float)mCamView->getActualWidth()) / imageSize.x < mOgreTexture->getHeight() / ((float)mCamView->getActualHeight()) / imageSize.y)
		{
			float r = mCamView->getActualHeight() / (float) mOgreTexture->getHeight() * imageSize.y;
			ratio.x = r * mOgreTexture->getWidth() / (float) mCamView->getActualWidth();
			ratio.y = imageSize.y;
		}
		else//if (mOgreTexture->getWidth() > mOgreTexture->getHeight())
		{
			float r = mCamView->getActualWidth() / (float) mOgreTexture->getWidth() * imageSize.x;
			ratio.y = r * mOgreTexture->getHeight() / (float) mCamView->getActualHeight();
			ratio.x = imageSize.x;
		}
	}
	imageRatio = ratio;

	manual->position(-ratio.x, -ratio.y, 0.0);
	manual->textureCoord(Ogre::Vector2(0, 1));
	manual->position( ratio.x, -ratio.y, 0.0);
	manual->textureCoord(Ogre::Vector2(1, 1));
	manual->position( ratio.x, ratio.y, 0.0);
	manual->textureCoord(Ogre::Vector2(1, 0));
	manual->position(-ratio.x, ratio.y, 0.0);
	manual->textureCoord(Ogre::Vector2(0, 0));
	manual->quad(0,1,2,3);

	manual->end();

	Ogre::SceneNode *drawNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(nodeName);
	drawNode->attachObject(manual);
	drawNode->setPosition(originalPosition);
	drawNode->setScale(originalScale);

	std::cout << "Renew" << std::endl;
	
}
bool DynamicImage::isTouched(const OIS::MouseEvent &arg, bool needDraw)
{
	// get the position and scale of the picture
	Ogre::SceneNode *drawNode = mSceneMgr->getSceneNode(nodeName);
	Vector3 pos(drawNode->getPosition());
	Vector3 scale(drawNode->getScale());

	Vector2 imageOnScreen;
	Vector2 clickOnImage;
	Vector2 clickOnScreen;

	// get the image size on the screen
	imageOnScreen.x = imageRatio.x * scale.x;
	imageOnScreen.y = imageRatio.y * scale.y;

	// get the position which the mouse click on the screen
	clickOnScreen.x = arg.state.X.abs/float(arg.state.width);
	clickOnScreen.y = arg.state.Y.abs/float(arg.state.height);

	float leftBound, rightBound, topBound, bottomBound;
	leftBound	=	0.5-imageOnScreen.x/2 + pos.x/2;
	rightBound	=	0.5+imageOnScreen.x/2 + pos.x/2;
	topBound	=	0.5+imageOnScreen.y/2 - pos.y/2;
	bottomBound	=	0.5-imageOnScreen.y/2 - pos.y/2;

	// check whether user click on the image
	if(clickOnScreen.x > leftBound && clickOnScreen.x < rightBound && clickOnScreen.y < topBound && clickOnScreen.y > bottomBound)
	{
		if(needDraw && drawable)
		{
			// get the coordinate on the image
			imageOnScreen.x = (clickOnScreen.x - leftBound) / imageOnScreen.x;
			imageOnScreen.y = (clickOnScreen.y - bottomBound) / imageOnScreen.y;

			drawPoint.x = imageOnScreen.x;
			drawPoint.y = imageOnScreen.y;
			drawRedDot();
		}
		return true;
	}
	else
	{
		return false;
	}
}
int DynamicImage::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
	{
		if(isTouched(arg,true))
		{
			return sn;
		}
		//bLMouseDown = true;
	}
	else if(id == OIS::MB_Right)
	{
		if(isTouched(arg,false))
		{
			bRMouseDown = true;
		}
	}
	else if (id == OIS::MB_Middle)
	{
		if(isTouched(arg,false))
		{
			bMidMouseDown = true;
		}
	}
	return -1;
}
void DynamicImage::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
	{
		bLMouseDown = false;
	}
	else if(id == OIS::MB_Right)
	{
		bRMouseDown = false;
	}
	else if (id == OIS::MB_Middle)
	{
		bMidMouseDown = false;
	}
}
void DynamicImage::mouseMoved(const OIS::MouseEvent &arg)
{
	if(controllable)
	{
		if(bMidMouseDown || bAlt)
		{
			Ogre::SceneNode *drawNode = mSceneMgr->getSceneNode(nodeName);
			Vector3 pos(drawNode->getPosition());
			pos.x += arg.state.X.rel / 500.0f;
			pos.y -= arg.state.Y.rel / 500.0f;
			drawNode->setPosition(pos);
		}
		else if(bRMouseDown)
		{
			Ogre::SceneNode *drawNode = mSceneMgr->getSceneNode(nodeName);
			Vector3 scale(drawNode->getScale());
			scale.x -= arg.state.Y.rel / 500.0f;
			scale.y -= arg.state.Y.rel / 500.0f;
			if (scale.x < 0.001f) scale.x = 0.001f;
			if (scale.y < 0.001f) scale.y = 0.001f;
			drawNode->setScale(scale);
		}
	}
}
void DynamicImage::nodeMoved(Ogre::Vector3 newPosition)
{
	Ogre::SceneNode *drawNode = mSceneMgr->getSceneNode(nodeName);
	nodePosition = newPosition;
	drawNode->setPosition(newPosition);
}
void DynamicImage::nodeScale(Ogre::Vector2 newSize)
{
	Ogre::SceneNode *drawNode = mSceneMgr->getSceneNode(nodeName);
	Vector3 scale(drawNode->getScale());
	scale.x = newSize.x;
	scale.y = newSize.y;
	drawNode->setScale(scale);	
}
void DynamicImage::drawRedDot()
{
	int deviedby;
	if(img_height>img_width)
	{
		deviedby = img_height/200;
	}
	else
	{
		deviedby = img_width/200;
	}
	// draw a 5x5 red square on the image
	for(int i=0;i<deviedby;i++)
	{
		for(int j=0;j<deviedby;j++)
		{
			int yy;
			int xx;

			yy = img_height * drawPoint.y - deviedby/2 +i;
			xx = img_width * drawPoint.x - deviedby/2 +j;

			if(yy < img_height && yy > 0 && xx < img_width && img_width > 0)
			{
				int offset_rgba = yy*img_width*4+xx*4;
				img_data[offset_rgba+rr] = drawColor.x;
				img_data[offset_rgba+gg] = drawColor.y;
				img_data[offset_rgba+bb] = drawColor.z;
			}

		}

	}	
	Vector2 center(img_width * drawPoint.x, img_height * drawPoint.y);
	points.push_back(center);
	//delete[] rgb;
	mImageDst.loadDynamicImage(img_data, img_width, img_height, mImageSrc.getDepth(), PF_A8R8G8B8);
	
	renew();
	//changeMaterial();
}
void DynamicImage::undo()
{
	if(!points.empty())
	{
		drawPoint = points.back();
		drawPoint.x /= img_width;
		drawPoint.y /= img_height;
		printf("undo: (%f,%f)\n", drawPoint.x, drawPoint.y);

		int deviedby;
		if(img_height>img_width)
		{
			deviedby = img_height/200;
		}
		else
		{
			deviedby = img_width/200;
		}
		// draw a 5x5 red square on the image
		for(int i=0;i<deviedby;i++)
		{
			for(int j=0;j<deviedby;j++)
			{
				int yy;
				int xx;

				yy = img_height * drawPoint.y - deviedby/2 +i;
				xx = img_width * drawPoint.x - deviedby/2 +j;

				if(yy < img_height && yy > 0 && xx < img_width && img_width > 0)
				{
					int offset_rgba = yy*img_width*4+xx*4;
					img_data[offset_rgba+rr] = original_data[offset_rgba+rr];
					img_data[offset_rgba+gg] = original_data[offset_rgba+gg];
					img_data[offset_rgba+bb] = original_data[offset_rgba+bb];
				}

			}

		}	
		points.pop_back();
		//delete[] rgb;
		mImageDst.loadDynamicImage(img_data, img_width, img_height, mImageSrc.getDepth(), PF_A8R8G8B8);
		renew();

	}
}
void DynamicImage::reload()
{
	points.clear();
	for(int i=0;i<img_width;i++)
	{
		for(int j=0;j<img_height;j++)
		{
			int offset_rgba = j*img_width*4+i*4;
			img_data[offset_rgba+rr] = original_data[offset_rgba+rr];
			img_data[offset_rgba+gg] = original_data[offset_rgba+gg];
			img_data[offset_rgba+bb] = original_data[offset_rgba+bb];
		}
	}	
	//delete[] rgb;
	mImageDst.loadDynamicImage(img_data, img_width, img_height, mImageSrc.getDepth(), PF_A8R8G8B8);
	renew();

}