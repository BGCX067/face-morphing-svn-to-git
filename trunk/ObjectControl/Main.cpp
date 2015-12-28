/******************************************************

  Project 1: Image Editor in OGRE

  National Taiwan University of Science and Technology
  Computer Game, Animation, and Graphics Laboratory

  Professor: Yu-Chi Lai

  Student:  Dobromir Todorov, Chun-Wei Wang, Damody		

******************************************************/

#include "stdafx.h"
#include "Main.h"
#include <Windows.h>
#include <vector>
// #include <cassert>

using namespace Ogre;
//-------------------------------------------------------------------------------------
Main::Main(void):
bLMouseDown(false),
bRMouseDown(false),
bMidMouseDown(false),
bAlt(false),
numObjects(0),
mCurrentObject(0),
mRaySceneQuery(0),
yKeyPressed(false),
xKeyPressed(false),
uKey(false),
zKey(false),
rKey(false),
needDraw(false)
{
}

//-------------------------------------------------------------------------------------
Main::~Main(void)
{
}

//-------------------------------------------------------------------------------------
void Main::createScene(void)
{
	mCamView = mCamera->getViewport();

	leftFace = new DynamicImage(mSceneMgr,
		mCamera,
		"leftFaceNode",
		Ogre::Vector3(-1.0f/2,0,0),
		Ogre::Vector3(255.0f,0,0),
		"leftFaceMaterial",
		"leftFaceManualObj",
		"..\\..\\media\\materials\\textures\\leftTmp.png",
		Ogre::Vector2(.5,.5),
		true,
		true,
		0);
	leftFace->openImage("a.png");

	rightFace = new DynamicImage(mSceneMgr,
		mCamera,
		"rightFaceNode",
		Ogre::Vector3(1.0f/2,0,0),
		Ogre::Vector3(255.0f,255.0f,0),
		"rightFaceMaterial",
		"rightFaceManualObj",
		"..\\..\\media\\materials\\textures\\rightTmp.png",
		Ogre::Vector2(.5,.5),
		true,
		true,
		1);
	rightFace->openImage("b.png");


	mViewPort = mWindow->getViewport(0);
	mInterface = new Interface(this);
	mInterface->setupHikari();

}

void Main::createFrameListener(void)
{
	BaseApplication::createFrameListener();
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	windowResized(mWindow);

	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
	mRoot->addFrameListener(this);
	mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());


}

bool Main::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(!BaseApplication::frameRenderingQueued(evt))
	{
		return false;
	}

	if (bRenew)
	{
		//renew();
		//drawSampleImage();
	}
	if (needDraw)
	{
		//drawRedDot();
	}

	//mCamera->yaw(Ogre::Degree(10.0f*evt.timeSinceLastFrame));
	mInterface->hikariMgr->update();
	return true;
}

bool Main::mouseMoved(const OIS::MouseEvent &arg)
{

	if(!BaseApplication::mouseMoved(arg))
	{
		return false;
	}

	leftFace->mouseMoved(arg);
	rightFace->mouseMoved(arg);

	if(bMidMouseDown || bAlt)
	{
	/*	Ogre::SceneNode *drawNode = mSceneMgr->getSceneNode("drawNode");
		Vector3 pos(drawNode->getPosition());
		pos.x += arg.state.X.rel / 500.0f;
		pos.y -= arg.state.Y.rel / 500.0f;
		drawNode->setPosition(pos);*/
	}
	else if(bRMouseDown)
	{
	/*	Ogre::SceneNode *drawNode = mSceneMgr->getSceneNode("drawNode");
		Vector3 scale(drawNode->getScale());
		scale.x -= arg.state.Y.rel / 500.0f;
		scale.y -= arg.state.Y.rel / 500.0f;
		if (scale.x < 0.001f) scale.x = 0.001f;
		if (scale.y < 0.001f) scale.y = 0.001f;
		drawNode->setScale(scale);*/
	}

	using namespace Hikari;
	return mInterface->hikariMgr->injectMouseMove(arg.state.X.abs, arg.state.Y.abs) ||  mInterface->hikariMgr->injectMouseWheel(arg.state.Z.rel);
}
bool Main::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if(mInterface->hikariMgr->injectMouseDown(id))
	{

	}
	else
	{
		int r = -1;
		DynamicImage* hit;
		if(r==-1)
		{
			hit = rightFace;
			r = hit->mousePressed(arg, id);
		}
		if(r==-1)
		{
			hit = leftFace;
			r = hit->mousePressed(arg, id);
		}
		if(r!=-1)
		{
			if(hit->drawable) undoVector.push_back(r);
		}
	}
	/*else if(id == OIS::MB_Left)
	{
		bLMouseDown = true;
		//getTouchPosition(arg);
	}
	else if(id == OIS::MB_Right)
	{
		bRMouseDown = true;
	}
	else if (id == OIS::MB_Middle)
	{
		bMidMouseDown = true;
	}*/

	using namespace Hikari;
	return mInterface->hikariMgr->injectMouseDown(id);
}

bool Main::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	leftFace->mouseReleased(arg, id);
	rightFace->mouseReleased(arg, id);
	if(id  == OIS::MB_Left)
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

	using namespace Hikari;
	return  mInterface->hikariMgr->injectMouseUp(id);
}

bool Main::keyPressed( const OIS::KeyEvent &arg )
{
	switch (arg.key)
	{
		/*case OIS::KC_RETURN:  // Enter for Reload image
			openImage("");
			break;
		case OIS::KC_R:  // Enter for Reload image
			openImage("tmp.png");
			break;
		case OIS::KC_SPACE:  // Space for hide/show UI
			mInterface->flip();
			break;*/
		case OIS::KC_LMENU:
		case OIS::KC_RMENU:
			bAlt = true;
			break;
		default:
			break;
	}
	return true;
}


bool Main::keyReleased( const OIS::KeyEvent &arg )
{

	switch (arg.key)
	{
	case OIS::KC_LMENU:
	case OIS::KC_RMENU:
		bAlt = false;
		break;

	default:
		break;
	}
	return true;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Main app;

        try {
            app.go();
    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Converts an image to RGB form, and returns the rgb pixel data - 24 
//  bits per pixel. The returned space should be deleted when no longer 
//  required.
//
///////////////////////////////////////////////////////////////////////////////
/*
unsigned char* Main::To_RGB(void)
{
	unsigned char *rgb = new unsigned char[img_width * img_height * 3];
	int		    i, j;

	if (! img_data)
		return NULL;

	// Divide out the alpha
	for (i = 0; i < img_height; i++)
	{
		int in_offset = i * img_width * 4;
		int out_offset = i * img_width * 3;

		for (j = 0 ; j < img_width ; j++)
		{
			RGBA_To_RGB(img_data + (in_offset + j*4), rgb + (out_offset + j*3));
		}
	}

	return rgb;
}
*/

//////////////////////////////////////////////////////////////////////////////
//
//  Given a single RGBA pixel return, via the second argument, the RGB
//  equivalent composited with a black background.
//
///////////////////////////////////////////////////////////////////////////////
/*
void Main::RGBA_To_RGB(unsigned char *rgba, unsigned char *rgb)
{
	const unsigned char	BACKGROUND[3] = { 0, 0, 0 };

	unsigned char  alpha = rgba[3];

	if (alpha == 0)
	{
		rgb[0] = BACKGROUND[0];
		rgb[1] = BACKGROUND[1];
		rgb[2] = BACKGROUND[2];
	}
	else
	{
		float	alpha_scale = (float)255 / (float)alpha;
		int	val;
		int	i;

		for (i = 0 ; i < 3 ; i++)
		{
			val = (int)floor(rgba[i] * alpha_scale);
			if (val < 0)
				rgb[i] = 0;
			else if (val > 255)
				rgb[i] = 255;
			else
				rgb[i] = val;
		}
	}
}
*/




///////////////////////////////////////////////////////////////////////////////
//
//  Convert image to grayscale.  Red, green, and blue channels should all 
//  contain grayscale value.  Alpha channel shoould be left unchanged.  Return
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
/*
void Main::Gray()
{
	unsigned char *rgb = this->To_RGB();

	for (int i=0; i<img_height; i++)
	{
		for (int j=0; j<img_width; j++)
		{
			int offset_rgb = i*img_width*3+j*3;
			int offset_rgba = i*img_width*4+j*4;
			unsigned char gray = 0.299 * rgb[offset_rgb + rr] + 0.587 * rgb[offset_rgb + gg] + 0.114 * rgb[offset_rgb + bb];

			for (int k=0; k<3; k++)
				img_data[offset_rgba+k] = gray;
			img_data[offset_rgba + aa] = WHITE;
		}
	}

	delete[] rgb;
	mImageDst.loadDynamicImage(img_data, img_width, img_height, mImageSrc.getDepth(), PF_A8R8G8B8);
	bRenew = true;
}
*/

void Main::imageMorphing(float percentageOfLeft)
{
	imageMorphing(1, percentageOfLeft);
}
void Main::imageMorphing(int numPics)
{
	imageMorphing(numPics, 1.);
}
void Main::imageMorphing(int numPics, float percentageOfLeft)
{
	leftFace->saveImage(leftFace->tempImageStr.c_str());
	rightFace->saveImage(rightFace->tempImageStr.c_str());
	const char *leftFaceStr = leftFace->tempImageStr.c_str();
	const char *rightFaceStr = rightFace->tempImageStr.c_str();
	IplImage* leftImage = cvLoadImage("..\\..\\media\\materials\\textures\\a.png");
	IplImage* rightImage = cvLoadImage("..\\..\\media\\materials\\textures\\b.png");

	CvSize imageSize = cvSize(rightImage->width, rightImage->height);
	IplImage* leftWarping;
	IplImage* rightWarping;
	IplImage* resultImage;

	if(numPics != 1)
	{
		for(int i=0; i < numPics+1; i++)
		{
			leftWarping = cvCreateImage(imageSize, rightImage->depth, rightImage->nChannels);
			rightWarping = cvCreateImage(imageSize, rightImage->depth, rightImage->nChannels);
			resultImage = cvCreateImage(imageSize, rightImage->depth, rightImage->nChannels);

			transformImage(rightImage, rightWarping, transformMat(leftFace->points, rightFace->points, (float)i/numPics));
			transformImage(leftImage, leftWarping, transformMat(rightFace->points, leftFace->points, (float)(numPics-i)/numPics));

			cvCvtColor(rightWarping, rightWarping, CV_BGR2Luv);
			cvCvtColor(leftWarping, leftWarping, CV_BGR2Luv);


			cvAddWeighted(leftWarping, (float)i/numPics, rightWarping, (float)(numPics-i)/numPics, 0, resultImage);

			cvCvtColor(resultImage, resultImage, CV_Luv2BGR);

			char str[100];

			sprintf(str, "..\\..\\media\\materials\\textures\\result\\result%d.png", i);
			cvSaveImage(str, resultImage);

			cvReleaseImage(&resultImage);
			cvReleaseImage(&leftWarping);
			cvReleaseImage(&rightWarping);
		}
	}
	else
	{
		leftWarping = cvCreateImage(imageSize, rightImage->depth, rightImage->nChannels);
		rightWarping = cvCreateImage(imageSize, rightImage->depth, rightImage->nChannels);
		resultImage = cvCreateImage(imageSize, rightImage->depth, rightImage->nChannels);

		transformImage(rightImage, rightWarping, transformMat(leftFace->points, rightFace->points, percentageOfLeft));
		transformImage(leftImage, leftWarping, transformMat(rightFace->points, leftFace->points, 1.0f-percentageOfLeft));

		showImage(leftWarping, "warp result left");
		showImage(rightWarping, "warp result right");

		cvCvtColor(rightWarping, rightWarping, CV_BGR2Luv);
		cvCvtColor(leftWarping, leftWarping, CV_BGR2Luv);

		cvAddWeighted(leftWarping, percentageOfLeft, rightWarping, 1.-percentageOfLeft, 0.0, resultImage);

		cvCvtColor(resultImage, resultImage, CV_Luv2BGR);

		showImage(resultImage, "result YA");
		cvSaveImage("..\\..\\media\\materials\\textures\\result\\result.png", resultImage);

		cvReleaseImage(&resultImage);
		cvReleaseImage(&leftWarping);
		cvReleaseImage(&rightWarping);
	}
	cvReleaseImage(&leftImage);
	cvReleaseImage(&rightImage);
	
}
CvMat* Main::transformMat(std::vector<Ogre::Vector2> p1, std::vector<Ogre::Vector2> p2, float percent)
{
	int numPoints = leftFace->points.size();
	double	a[200*2];
	double	b[200*2];

	for(int i=0;i<numPoints && i<200;i++)
	{
		a[i*2+0] = p1[i].x;
		a[i*2+1] = p1[i].y;
		b[i*2+0] = p2[i].x * percent + p1[i].x * (1.-percent);
		b[i*2+1] = p2[i].y * percent + p1[i].y * (1.-percent);
		printf("(%.0f, %.0f) (%.0f, %.0f)\n", a[i*2+0], a[i*2+1], b[i*2+0], b[i*2+1]);
	}

	CvMat	src_point;
	src_point = cvMat( numPoints, 2, CV_64FC1, b );

	CvMat	dst_point;
	dst_point = cvMat( numPoints, 2, CV_64FC1, a );

	CvMat	*h = cvCreateMat( 3, 3, CV_64FC1 );

	cvFindHomography( &src_point, &dst_point, h , CV_LMEDS);
	return h;
}

void Main::transformImage(IplImage* src, IplImage* dst, CvMat* transMat)
{
	CvSize imageSize = cvSize(src->width, src->height);
	IplImage* temp = cvCreateImage(imageSize, src->depth, src->nChannels);

	cvCopy(src, temp);

	cvWarpPerspective(
		temp,	//src image
		dst,	//dst image
		transMat	 //transformation matrix
		);

	cvReleaseImage(&temp);
}
void Main::showImage(IplImage* img, char* windowName)
{


	int width = img->width;
	int height = img->height;

	if(width > height)
	{
		height = height * (640. / width);
		width = width * (640. / width);
	}
	else
	{
		width = width * (640. / height);
		height = height * (640. / height);
	}

	cvNamedWindow(windowName,0);
	cvResizeWindow(windowName,width,height);
	cvShowImage(windowName,img);
}
//-------------------------------------------------------

void Main::Undo()
{
	if(!undoVector.empty())
	{
		int n;
		n = undoVector.back();
		undoVector.pop_back();
		
		if(n == leftFace->sn)
		{
			leftFace->undo();
		}if(n == rightFace->sn)
		{
			rightFace->undo();
		}
		/*else if(n == leftFace->sn)
		{
			leftFace->undo();
		}*/
	}
}
void Main::Reload()
{
	undoVector.clear();
	leftFace->reload();
	rightFace->reload();
}

