#ifndef XICOREVERSION_H
#define XICOREVERSION_H

#include <xiCoreGlobal.h>


/*!
 \brief Gets the xiCore version string for display.

 \fn xiCoreVersionLongString()
 \return The version string in format M.m build b, where "M" means "Major version",
 the "m" means "Minor version" and "b" means build number.
*/
QString XICORE_API xiCoreVersionLongString();



/*!
 \brief Gets the xiCore version string.

 \fn xiCoreVersion()
 \return The version string in format M.m.b, where "M" means "Major version",
 the "m" means "Minor version" and "b" means build number.
*/
QString XICORE_API xiCoreVersion();



/*!
 \brief Gets the xiCore version.

 \fn xiCoreVersion(int &major, int &minor, int build)
 \param major Major version.
 \param minor Minor version.
 \param build Build number.
*/
void XICORE_API xiCoreVersion(int &major, int &minor, int &build);


#endif // XICOREVERSION_H
