#include "stdafx.h"
#include "Interface.h"
#include "Main.h"

//-------------------------------------------------------------------------------------
Interface::~Interface(void)
{
}

void Interface::setupHikari()
{
	using namespace Hikari;
	hikariMgr = new HikariManager("");
	//	controls = hikariMgr->createFlashOverlay("Menu",main->mViewPort, main->mViewPort->getActualWidth(), main->mViewPort->getActualWidth()*0.4, Position(TopLeft));
	controls = hikariMgr->createFlashOverlay("Menu",main->mViewPort, main->mViewPort->getActualWidth(), main->mViewPort->getActualWidth()*0.4, Position(0.0f, -8.0f));
	controls->load("..\\..\\SimpleMenu\\SimpleMenu.swf");
	controls->setDraggable(false);
	controls->setTransparent(true);
	controls->bind("ButtonClicked", FlashDelegate(this, &Interface::onButtonClick));

	fileBrowser = hikariMgr->createFlashOverlay("FileBrowser",main->mViewPort, 300, 300, Position(Center));
	fileBrowser->load("..\\..\\SimpleMenu\\FileSelect.swf");
	fileBrowser->setDraggable(true);
	fileBrowser->setTransparent(true,true);
	fileBrowser->hide();
	fileBrowser->bind("Cancel", FlashDelegate(this, &Interface::onCancel));
	fileBrowser->bind("Ok", FlashDelegate(this, &Interface::onOk));
	fileBrowser->bind("ItemSelect", FlashDelegate(this, &Interface::onItemSelect));
	disk = new DiskObject();
	d_fileFilter=std::string("*.png");

	Prompt = hikariMgr->createFlashOverlay("Prompt",main->mViewPort, 300, 200, Position(Center));
	Prompt->load("..\\..\\SimpleMenu\\Prompt.swf");
	Prompt->setDraggable(true);
	Prompt->setTransparent(true,true);
	Prompt->hide();
	Prompt->bind("Param", FlashDelegate(this, &Interface::onParam));
	Prompt->bind("Cancel", FlashDelegate(this, &Interface::onClose));
}

std::vector<std::string>  Interface::parse(std::string l, char delim) 
{
	std::replace(l.begin(), l.end(), delim, ' ');
	std::istringstream stm(l);
	std::vector<std::string> tokens;
	for (;;) {
		std::string word;
		if (!(stm >> word)) break;
		tokens.push_back(word);
	}
	return tokens;
}

Hikari::FlashValue Interface::onClose(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	Prompt->hide();
	return FLASH_VOID;
}

Hikari::FlashValue Interface::onParam(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	Prompt->hide();
	float fParam = args[0].getNumber();
	int iParam = args[0].getNumber();

	if (currntOperation == SAVE)
	{
		Ogre::String str = "..\\..\\media\\materials\\textures\\";
		str.append(args[0].getString());
		str.append(".png");
//		main->saveImage(str.c_str());
	}
	else if (currntOperation == MORPHING_BY_NUM_OF_PICS)
	{
		main->imageMorphing(iParam);
	}
	else if (currntOperation == MORPHING_BY_NUM_OF_PERCENTAGE)
	{
		main->imageMorphing(fParam);
	}
/*	else if (currntOperation == FILTER_GAUSSIAN_N)
	{
		printf("\nN: %d\n", iParam);
		main->Filter_Gaussian_N(iParam);
	}
	else if (currntOperation == ROTATE)
	{
		printf("\nThe degree of rotate: %.2lf\n", fParam);
		main->Rotate(fParam);
	}
	else if (currntOperation == RESIZE)
	{
		printf("\nSize: %.2lf\n", fParam);
		main->Resize(fParam);
	}*/

	return FLASH_VOID;
}

Hikari::FlashValue Interface::onButtonClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	using namespace Hikari;
	Ogre::String funcArg = args[0].getString();

	
	if (funcArg=="Open")
	{
		fileBrowser->show();
		fileBrowser->focus();
		disk->setCurrentVFolder(disk->getApplicationFolder());
		disk->setCurrentVFolder("..\\..\\media\\materials\\textures");
		updateFolderList();
		currntOperation=OPEN;
	}
	else if(funcArg=="Save")
	{
		Prompt->show();
		fileBrowser->focus();
		Ogre::String s = "Input the name of the file";
		Prompt->callFunction("reset",Args(s));
		currntOperation= SAVE;
	}
	else if(funcArg=="Exit")
	{
//		main->mShutDown=true;
	}
	else if(funcArg=="Gray")
	{
//		main->Gray();
	}
	else if(funcArg=="Undo")
	{
		main->Undo();
	}
	else if(funcArg=="Reload")
	{
		main->Reload();
	}
	else if(funcArg=="MorphingHalf")
	{	
		main->imageMorphing(0.5f);
	}
	else if(funcArg=="MorphingPics")
	{	
		Prompt->show();
		fileBrowser->focus();
		Ogre::String s = "Enter the number of results";
		Prompt->callFunction("reset",Args(s));
		currntOperation= MORPHING_BY_NUM_OF_PICS;
	}
	else if(funcArg=="MorphingPercent")
	{	

		Prompt->show();
		fileBrowser->focus();
		Ogre::String s = "Enter the percentage of the left face";
		Prompt->callFunction("reset",Args(s));
		currntOperation= MORPHING_BY_NUM_OF_PERCENTAGE;
	}
	/*else if(funcArg=="Quant_Populosity")
	{
		main->Quant_Populosity();
	}
	else if(funcArg=="Dither_Threshold")
	{
		main->Dither_Threshold();
	}
	else if(funcArg=="Dither_Random")
	{
		main->Dither_Random();
	}
	else if(funcArg=="Dither_FS")
	{
		main->Dither_FS();
	}
	else if(funcArg=="Dither_Bright")
	{
		main->Dither_Bright();
	}
	else if(funcArg=="Dither_Cluster")
	{
		main->Dither_Cluster();
	}
	else if(funcArg=="Dither_Color")
	{
		main->Dither_Color();
	}
	else if(funcArg=="Filter_Box")
	{
		main->Filter_Box();
	}
	else if(funcArg=="Filter_Bartlett")
	{
		main->Filter_Bartlett();
	}
	else if(funcArg=="Filter_Gaussian")
	{
		main->Filter_Gaussian();
	}
	else if(funcArg=="Filter_Gaussian_N")
	{
		Prompt->show();
		fileBrowser->focus();
		Ogre::String s = "Enter the size of filter";
		Prompt->callFunction("reset",Args(s));
		currntOperation= FILTER_GAUSSIAN_N;
	}
	else if(funcArg=="Filter_Edge")
	{
		main->Filter_Edge();
	}
	else if(funcArg=="Filter_Enhance")
	{
		main->Filter_Enhance();
	}
	else if(funcArg=="Half_Size")
	{
		main->Half_Size();
	}
	else if(funcArg=="Double_Size")
	{
		main->Double_Size();
	}
	else if(funcArg=="Resize")
	{
		Prompt->show();
		fileBrowser->focus();
		Ogre::String s = "Input the percent of the scale (e.g 0.5 will resize the image by half)";
		Prompt->callFunction("reset",Args(s));
		currntOperation= RESIZE;
	}
	else if(funcArg=="Rotate")
	{	
		Prompt->show();
		fileBrowser->focus();
		Ogre::String s = "Enter the angle of rotate";
		Prompt->callFunction("reset",Args(s));
		currntOperation= ROTATE;
	}
	else if(funcArg.substr(0,4) == "Comp")
	{
		fileBrowser->show();
		fileBrowser->focus();
		disk->setCurrentVFolder(disk->getApplicationFolder());
		disk->setCurrentVFolder("..\\..\\media\\materials\\textures");
		updateFolderList();
		if (funcArg == "Comp_Over")
			currntOperation = COMP_OVER;
		else if (funcArg == "Comp_In")
			currntOperation = COMP_IN;
		else if (funcArg == "Comp_Out")
			currntOperation = COMP_OUT;
		else if (funcArg == "Comp_Atop")
			currntOperation = COMP_ATOP;	
		else if (funcArg == "Comp_Xor")
			currntOperation = COMP_XOR;
	}
	else if(funcArg=="NPR_Paint")
	{
		main->NPR_Paint();
	}*/
	else if(funcArg == "Reload")
	{
//		main->openImage("");
	}
	else if(funcArg == "Hide")
	{
		flip();	
	}

	return FLASH_VOID;
}

Hikari::FlashValue Interface::onCancel(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	using namespace Hikari;
	fileBrowser->hide();
	return FLASH_VOID;
}

Hikari::FlashValue Interface::onOk(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	using namespace Hikari;
	fileBrowser->hide();

	if(currntOperation == OPEN)
	{
		//printf("PATH: %s",filePath);
//		main->openImage(filePath.c_str());
	}
	else if(currntOperation == SAVE)
	{
		filePath = disk->getCurrentVFolder();
		std::string res= fileBrowser->callFunction("readText").getString();
		filePath+=res;
		printf("PATH: %s",filePath);

	}
	
	/*else if (currntOperation == COMP_OVER)
	{
		main->loadSecondaryImge(filePath.c_str());
		main->Comp_Over();
	}
	else if (currntOperation == COMP_IN)
	{
		main->loadSecondaryImge(filePath.c_str());
		main->Comp_In();
	}
	else if (currntOperation == COMP_OUT)
	{
		main->loadSecondaryImge(filePath.c_str());
		main->Comp_Out();
	}
	else if (currntOperation == COMP_ATOP)
	{
		main->loadSecondaryImge(filePath.c_str());
		main->Comp_Atop();
	}
	else if (currntOperation == COMP_XOR)
	{
		main->loadSecondaryImge(filePath.c_str());
		main->Comp_Xor();
	}*/

	return FLASH_VOID;
}
Hikari::FlashValue Interface::onItemSelect(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	using namespace Hikari;

	std::string text = args.at(0).getString(); 
	fileName=text;
	if (text == "..")
	{
		//disk->upLevel();
		//updateFolderList();
		//d_result = disk->getCurrentVFolder();
		//fileBrowser->callFunction("setName",Args(d_result));
	}
	else if(text.find(".",1)!=std::string::npos)
	{
		//filePath=disk->getCurrentVFolder()+ "\\" +text;
		filePath = text;
	}
	else
	{
		std::string path = disk->getCurrentVFolder();
		if (path.size() > 3) path.append("\\");
		path.append(text);
		disk->setCurrentVFolder(std::string(path.c_str()));
		updateFolderList();
		d_result = disk->getCurrentVFolder();
		fileBrowser->callFunction("setName",Args(text));
	}

	return FLASH_VOID;
}
void Interface::updateFolderList()
{
	using namespace Hikari;
	using namespace std;
	string path ="";
	d_itemList.clear();
	d_itemList = disk->getFolders();
	for(int i=0;i<disk->getFiles(d_fileFilter).size();i++)
	{
		d_itemList.push_back(disk->getFiles(d_fileFilter)[i]);
		
	}
	for (int n = 0; n < d_itemList.size(); n++)
	{
		path.append(d_itemList[n].c_str());
		path.append("|");
	}
	fileBrowser->callFunction("refreshList",Args(path));
}

void Interface::flip()
{
	if (controls->getVisibility())
		controls->hide();
	else
		controls->show();
}
