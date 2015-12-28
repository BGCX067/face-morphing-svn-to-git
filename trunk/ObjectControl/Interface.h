#pragma once
#include <Hikari.h>
#include "BaseApplication.h"
#include "DiskObject.h"
#include "ClassDefine.h"


class Interface
{
	
public:

	enum Type
	{
		// FILE
		SAVE,
		OPEN,
		//EXIT,

		// COLOR
		GRAY,
		QUANT_UNIFORM, 
		QUANT_POPULOSITY, 
		QUANT_MEDIAN, 

		// DITHERING
		DITHER_THRESHOLD, 
		DITHER_RANDOM, 
		DITHER_FS, 
		DITHER_BRIGHT, 
		DITHER_CLUSTER, 
		DITHER_COLOR, 

		// FILTER
		FILTER_BOX, 
		FILTER_BARTLETT, 
		FILTER_GAUSSIAN, 
		FILTER_GAUSSIAN_N,
		FILTER_EDGE, 
		FILTER_ENHANCE, 

		// SIZE
		HALF_SIZE, 
		DOUBLE_SIZE, 
		RESIZE,
		ROTATE,

		// Composing
		COMP_OVER, 
		COMP_IN, 
		COMP_OUT, 
		COMP_ATOP, 
		COMP_XOR, 

		// SPECIAL
		NPR_PAINT, 
		MORPHING_BY_NUM_OF_PICS,
		MORPHING_BY_NUM_OF_PERCENTAGE, 
	};

	Main* main;
	Interface(Main* _main)
		:hikariMgr(0)
	{
		main=_main;
	}

	virtual ~Interface(void);
	void setupHikari();
	void flip();
	Hikari::FlashControl* controls;
	Hikari::FlashControl* fileBrowser;
	Hikari::FlashControl* Prompt;
	DiskObject* disk;
	Hikari::HikariManager* hikariMgr;
	int currntOperation;

	std::string fileName;

protected:
	
	Hikari::FlashValue onButtonClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onCancel(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onOk(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onItemSelect(Hikari::FlashControl* caller, const Hikari::Arguments& args);

	Hikari::FlashValue onProp(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onParam(Hikari::FlashControl* caller, const Hikari::Arguments& args);

	Hikari::FlashValue onClose(Hikari::FlashControl* caller, const Hikari::Arguments& args);

	
	void updateFolderList();
	std::vector<std::string> parse(std::string l, char delim) ;
	
	std::string d_result;
	std::string d_fileFilter;
	std::vector<std::string> d_itemList;
	std::string filePath;
	
};