#pragma once
#include <windows.h>
#include <string>
#include <vector>

/**
 * @short Disk management and operations for win32 API.
 * DiskObject creates a virtual disk and the user can move in it. The
 * application directory changes temporaly while retrieving files or
 * folders but returns to the app root ASAP.
*/
class DiskObject
{
public:
	DiskObject(void);
	~DiskObject(void);

	/**
		@short returns the path of the current folder
	*/
	std::string getCurrentVFolder();

	/**
		@short sets the current folder to the path.
	*/
	bool setCurrentVFolder(std::string path);

	/**
		@short resturns the path of the application folder
	*/
	std::string getApplicationFolder();

	/**
		@short returns a vector with the folders in current directory
	*/
	std::vector<std::string> getFolders();

	/**
		@short returns a vector with files in current directory
	*/
	std::vector<std::string> getFiles(std::string filter = std::string("*"));

	/**
		@short returns a vector with names of logical drives from A to Z in the system
	*/
	std::vector<std::string> getLogicalDrives();

	/**
		@short goes one directory level up
	*/
	void upLevel();

private:
	std::string getCurrentFolder();
	bool setCurrentFolder(std::string path);
	static std::string d_appFolder;
	std::string d_currentVFolder;
};
