/********************************************************************
	created:	2012/04/05
	created:	5:4:2012   10:02
	filename: 	XmlParser.h
	file path:	FrozenEngine\Frozen Engine
	file base:	XmlParser
	file ext:	h
	author:		Kristian Skarseth, Peder Thorup
	
	purpose:	Imports filepath and names of all resources to be used 
				in the game. When a full filepath to a resource, either
				images, sounds of video, is specified in the resources
				xml file, the class can import one of either type
*********************************************************************/
#ifndef XmlResourcesImport_h__
#define XmlResourcesImport_h__

#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "Logger.h"
#include "GCPtr.h"

using boost::property_tree::ptree;
using std::endl;
using std::string;

enum eFilePathTypes
{ 
	SURFACE_FILES, 
	SOUND_FILES, 
	VIDEO_FILES	
};

struct SpriteInfo
{
	float runAnimDuration;
	int runFrames;
	int runFrameWidth;
	int runFrameHeight;

	int jumpFrameWidth;
	int jumpFrameHeight;
};

struct VideoInfo
{
	int width;
	int height;
};

typedef std::vector<string> StringVector;
typedef std::vector<SpriteInfo*> SpriteInfoVector;
typedef std::vector<VideoInfo*> VideoInfoVector;
class CXmlResourcesImport
{
public:
	CXmlResourcesImport();
	// Constructor for predefined xml and child paths
	CXmlResourcesImport(enum eFilePathTypes xmlFile, string logLineName);
	
	// Constructor for custom xml and child paths
	CXmlResourcesImport(string XMLpath, string ChildPath);
	
	~CXmlResourcesImport();

	// Returns a reference to the vector with imported filePaths
	StringVector* GetFilePathVector();
	
	// Returns a reference to the vector with matching clean filenames 
	// (just the name, no filepath or file extension)
	StringVector* GetFileNameVector();

	// Additional getters for sprite vectors that are dynamically allocated 
	// when we're importing surfaces.
	SpriteInfoVector* GetSpriteInfoVector();
	StringVector* GetSpriteFilePathVector();
	StringVector* GetSpriteFileNameVector();
	VideoInfoVector* GetVideoInfoVector();

private:
	Logger log;

	// Imports all child nodes from the node path specified by _childPath
	// from the xml file spcified by _xmlPath. The imported (strings) will 
	// be placed in _pt
	void _ImportXml();
	
	bool _AddTofilePathVector();
	bool _AddToFileNameVector();
	
	bool _AddSpriteInfo();
	bool _AddVideoInfo();
	// Returns a clean filename string without the filePath or file extension. The format of the string you 
	// send with the param must be like following example: "..//dir//myFile.extension". The function will then return a string
	// with the name of the file only, like follows: "myFile". It can format any filepath, as long as there is no '/' letters
	// used for the file name itself.
	string _FormatKeyName(string& filePath);

	eFilePathTypes importType;

	ptree _ptFullImport;
	ptree _importChildren;

	string _xmlPath;
	string _childPath;

	StringVector _filePathVector;
	StringVector _fileNameVector;

	// Additional vectors used by the class if we're importing surfaces, 
	// to differentiate static surfaces from sprites.
	StringVector _spriteFilePathVector;
	StringVector _spriteNameVector;
	SpriteInfoVector _spriteInfo;

	//GCPtr<VideoInfo> _videoInfo;
	VideoInfoVector _videoInfoVector;
};

#endif // XmlResourcesImport_h__