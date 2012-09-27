#include "xmlResourcesImport.h"
#include "xmlSettingsHandling.h"
//
using namespace boost::property_tree;
//

/* P U B L I C    F U N C T I O N S */
/*----------------------------------*/

CXmlResourcesImport::CXmlResourcesImport() : log("xmlresourcesImport", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{
}

CXmlResourcesImport::CXmlResourcesImport(enum eFilePathTypes xmlFile, string logLineName) 
	: log("XmlParser", CXmlSettings::Inst()->GetLoggerSettings()->logLevel)
{	
	importType = xmlFile;
	switch(xmlFile)
	{
	case SURFACE_FILES: 
		_xmlPath = "..//XML//Resources.xml";
		_childPath = "resources.images";
		break;
	case VIDEO_FILES:
		_xmlPath = "..//XML//Resources.xml";
		_childPath = "resources.video";
		break;
	case SOUND_FILES:
		_xmlPath = "..//XML//Resources.xml";
		_childPath = "resources.sfx";
		break;
	}

	_ImportXml();
}

CXmlResourcesImport::CXmlResourcesImport(string XMLpath, string ChildPath) 
	: log("XmlParser", CXmlSettings::Inst()->GetLoggerSettings()->logLevel), _xmlPath(XMLpath), _childPath(ChildPath)
{
	_ImportXml();
}

CXmlResourcesImport::~CXmlResourcesImport()
{
	for(SpriteInfoVector::iterator i = _spriteInfo.begin(); 
		i != _spriteInfo.end(); i++)
	{
		delete (*i);
		(*i) = 0;
	}
}

StringVector* CXmlResourcesImport::GetFilePathVector()
{
	return &_filePathVector;
}

StringVector* CXmlResourcesImport::GetFileNameVector()
{
	return &_fileNameVector;
}

SpriteInfoVector* CXmlResourcesImport::GetSpriteInfoVector()
{
	return &_spriteInfo;
}

StringVector* CXmlResourcesImport::GetSpriteFilePathVector()
{
	return &_spriteFilePathVector;
}

StringVector* CXmlResourcesImport::GetSpriteFileNameVector()
{
	return &_spriteNameVector;
}

VideoInfoVector* CXmlResourcesImport::GetVideoInfoVector()
{
	return &_videoInfoVector;
}

/* P R I V A T E     F U N C T I O N S */
/*-------------------------------------*/

void CXmlResourcesImport::_ImportXml()
{
	log << INFO << "Loading XML file: " << _xmlPath << "..." << endl;
	read_xml(_xmlPath, _ptFullImport);
	_importChildren = _ptFullImport.get_child(_childPath);
	
	if(_AddTofilePathVector() && _AddToFileNameVector())
	{
		log << WARN << "Loading XML file: " << _xmlPath << ", "<< _childPath << " Successful." << endl;	
	}
	else
	{
		log << ERRORX << "Loading XML file: " << _xmlPath << ", " << _childPath << " failed." << endl;
	}

	if(importType == SURFACE_FILES)
	{
		if(!_AddSpriteInfo())
		{
			log << ERRORX << "Loading XML file: " << _xmlPath << ", " << _childPath << " sprite info failed." << endl;
		}
	}
	else if(importType == VIDEO_FILES)
	{
		if(!_AddVideoInfo())
		{
			log << ERRORX << "Loading video info: " << _xmlPath << ", " << _childPath << " failed." << endl;
		}
	}
}

bool CXmlResourcesImport::_AddTofilePathVector()
{
	for(ptree::iterator i = _importChildren.begin(); i != _importChildren.end(); i++)
	{
		_filePathVector.push_back(i->second.get<string>("filename"));
		log << INFO << "Loaded: " << _filePathVector.back() << endl;
	}

	if(_importChildren.size() != _filePathVector.size())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CXmlResourcesImport::_AddToFileNameVector()
{
	for(StringVector::iterator i = _filePathVector.begin(); i != _filePathVector.end(); i++)
	{
		_fileNameVector.push_back(_FormatKeyName(*i));
		log << INFO << "Created formated fileName: " << _fileNameVector.back() << endl;
	}

	if(_fileNameVector.size() != _filePathVector.size())
	{
		log << ERRORX << "fileNameVector != filePathVector size!" << endl;
		return false;
	}
	else
		return true;
}

std::string CXmlResourcesImport::_FormatKeyName(string& filePath)
{
	unsigned short slashTotal = 0;
	unsigned short slashCount = 0;
	string theKeyName = filePath;

	for(string::iterator it = theKeyName.begin(); it != theKeyName.end();++it)
	{
		if((*it) == '/')
			++slashTotal;
	}
	for(string::iterator it = theKeyName.begin(); it != theKeyName.end();)
	{
		if((*it) == '/')
			++slashCount;

		if(slashCount >= slashTotal)
		{
			theKeyName.erase(it);
			it = theKeyName.end();
		}
		else
			theKeyName.erase(it);
	}
	for(string::iterator it = theKeyName.end(); it != theKeyName.begin();)
	{
		--it;
		if((*it) == '.')
		{
			theKeyName.pop_back();
			it = theKeyName.begin();	
		}
		else
			theKeyName.erase(it);
	}
	return theKeyName;
}

bool CXmlResourcesImport::_AddSpriteInfo()
{
	try
	{
		ptree spriteTree = _ptFullImport.get_child("resources.sprites");

		for(ptree::iterator i = spriteTree.begin(); i != spriteTree.end(); i++)
		{
			_spriteFilePathVector.push_back(i->second.get<string>("filename"));
			log << INFO << "Loaded: " << _spriteFilePathVector.back() << endl;

			_spriteNameVector.push_back(_FormatKeyName(_spriteFilePathVector.back()));
			log << INFO << "Created formated fileName: " << _spriteNameVector.back() << endl;

			SpriteInfo* info = new SpriteInfo;
			info->runAnimDuration = i->second.get<float>("runAnimDuration", 0);
			info->runFrames = i->second.get<int>("runFrames", 0);
			info->runFrameWidth = i->second.get<int>("runFrameWidth", 0);
			info->runFrameHeight = i->second.get<int>("runFrameHeight", 0);
			
			info->jumpFrameWidth = i->second.get<int>("jumpFrameWidth", 0);
			info->jumpFrameHeight = i->second.get<int>("jumpFrameHeight", 0);
			_spriteInfo.push_back(info);
		}
		return true;
	}
	catch (ptree_bad_path)
	{
		log << ERRORX << "Boost ptree_bad_path caught: Wrong path or invalid value" << endl;
		return false;
	}
}

bool CXmlResourcesImport::_AddVideoInfo()
{
	try
	{
		ptree videoTree = _ptFullImport.get_child(_childPath);

		for(ptree::iterator i = videoTree.begin(); i != videoTree.end(); i++)
		{
			VideoInfo* _videoInfo = new VideoInfo;
			_videoInfo->height = i->second.get<int>("height", 720);
			_videoInfo->width = i->second.get<int>("width", 1280);
			_videoInfoVector.push_back(_videoInfo);
		}
		return true;
	}
	catch (ptree_bad_path)
	{
		log << ERRORX << "Boost ptree_bad_path caught: Wrong path or invalid value" << endl;
		return false;
	}
}



