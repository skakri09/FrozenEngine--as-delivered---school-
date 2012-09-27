#include "StaticBackground.h"

CStaticBackground::CStaticBackground(std::string surfaceName)
	: log("StaticBackgorund", CXmlSettings::Inst()->GetLoggerSettings()->logLevel),
	CRenderableNoAnimation(surfaceName, true)
{

}

CStaticBackground::~CStaticBackground()
{

}

void CStaticBackground::UpdateSurface()
{

}
