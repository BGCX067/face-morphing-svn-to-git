#include "stdafx.h"
#include "DiskObject.h"


std::string DiskObject::d_appFolder;

DiskObject::DiskObject(void)
{
	getLogicalDrives();
	if (d_appFolder == "")
	{
		d_appFolder = getCurrentFolder();
	}
	d_currentVFolder = d_appFolder;
}

DiskObject::~DiskObject(void)
{
}

std::vector<std::string> DiskObject::getLogicalDrives()
{
	DWORD word = GetLogicalDrives();
	std::vector<std::string> res;

	for (int i = 2; i < 26; i ++)   // C: to Z:
    {
        if (GetLogicalDrives () & (1 << i))
		{
			char buffer[256];
			sprintf(buffer, "%c:\\", 'A' + i);
			res.push_back(std::string(buffer));
		}
	}
	return res;
}

std::string DiskObject::getCurrentFolder()
{
   TCHAR buffer[MAX_PATH];
   DWORD dwRet;

   dwRet = GetCurrentDirectory(MAX_PATH, buffer);

   if( dwRet == 0 )
   {
      fprintf(stderr, "GetCurrentDirectory failed (%d)\n", GetLastError());
      return NULL;
   }
   if(dwRet > MAX_PATH)
   {
      fprintf(stderr, "Buffer too small; need %d characters\n", dwRet);
      return NULL;
   }

   return std::string(buffer);
}

std::string DiskObject::getCurrentVFolder()
{
   return d_currentVFolder;
}

bool DiskObject::setCurrentVFolder(std::string path)
{
	if (path == "")
	{
		d_currentVFolder = "";
		return true;
	}
	if (setCurrentFolder(path))
	{
		d_currentVFolder = path;
		setCurrentFolder(d_appFolder);
		return true;
	}
	setCurrentFolder(d_appFolder);
	return false;
}

bool DiskObject::setCurrentFolder(std::string path)
{
	if( !SetCurrentDirectory(path.c_str()))
	{
		fprintf(stderr, "setCurrentFolder failed (%d)\n", GetLastError());
		return false;
	}
	return true;
}

std::string DiskObject::getApplicationFolder()
{
	return d_appFolder;
}

std::vector<std::string> DiskObject::getFolders()
{
	if (getCurrentVFolder() == "")
		return getLogicalDrives();
	setCurrentFolder(getCurrentVFolder());

	WIN32_FIND_DATA ffd;
	char buffer[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;
	std::vector<std::string> files;

	// Find the first file in the directory.

	std::string path = getCurrentFolder();
	path.append("\\*");
	sprintf((char*)buffer, path.c_str());
	
	hFind = FindFirstFileEx(buffer, 
					   FindExInfoStandard, 
					   &ffd,
					   FindExSearchNameMatch, 
					   NULL, 0);

	if (INVALID_HANDLE_VALUE == hFind) 
	{
		fprintf(stderr, "Error opening file\n");
		return files;
	} 

	// List all the files in the directory with some info about them.

	//files.push_back(std::string(".."));
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			std::string file(ffd.cFileName);
			//file.append(" <DIR>");
			if (file != "." && file != "..")
				files.push_back(file);
		}/*
		else
		{
			std::string file (ffd.nFileSizeLow);
			file.append(ffd.nFileSizeHigh);
			std::string file (ffd.cFileName); //filesize.QuadPart contains file size in bytes
			files.push_back(file);
		}*/
	}	
	while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		fprintf(stderr, "Error opening file\n");
		return files;
	}

	FindClose(hFind);

	setCurrentFolder(getApplicationFolder());
	return files;
}

std::vector<std::string> DiskObject::getFiles(std::string filter)
{
	std::vector<std::string> files;
	if (getCurrentVFolder() == "")
		return files;
	setCurrentFolder(getCurrentVFolder());

	WIN32_FIND_DATA ffd;
	char buffer[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;

	// Find the first file in the directory.

	std::string path = getCurrentFolder();
	path.append("\\");
	path.append(filter);
	sprintf((char*)buffer, path.c_str());
	
	hFind = FindFirstFileEx(buffer, 
					   FindExInfoStandard, 
					   &ffd,
					   FindExSearchNameMatch, 
					   NULL, 0);

	if (INVALID_HANDLE_VALUE == hFind) 
	{
		//fprintf(stderr, "Error opening file\n"); // No files with current filter
		return files;
	} 

	// List all the files in the directory with some info about them.

	do
	{
		if ( !(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			//std::string file (ffd.nFileSizeLow);
			//file.append(ffd.nFileSizeHigh);
			std::string file(ffd.cFileName);
			files.push_back(file);
		}
	}	
	while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		fprintf(stderr, "Error opening file\n");
		return files;
	}

	FindClose(hFind);

	setCurrentFolder(getApplicationFolder());
	return files;
}

void DiskObject::upLevel()
{
	using namespace std;
	string c_dir;
	vector<string> dir_list;

	c_dir = getCurrentVFolder();

	if (c_dir.size() == 3) // Drive root. Back to Drive selection
	{
		setCurrentVFolder("");
		return;
	}
	for (int n = c_dir.size()-1; n >= 2; n--)
	{
		if (c_dir[n] == '\\')
		{
			std::string up(c_dir.substr(0, n));
			if (up.size() == 2) up.append("\\");
			setCurrentVFolder(up);
			return;
		}
	}
	setCurrentVFolder("");
}