#ifndef UNITS_H
#define UNITS_H

#include <xiCoreGlobal.h>
#include <QString>

//! \brief Class the defines all known units and measures, and includes conversions between them
class XICORE_API CxUnits
{
public:
    //! All measures defined here
    enum ExUnitGroup
    {
        eugInvalid,
        eugTime,
        eugLength,
        eugTemperature,
        eugRatio,
    };

    enum ExTime
    {
        etMicrosecond  = 0,
        etMillisecond  = 1,
        etSecond       = 2,
        etMinute       = 3,
        etHour         = 4,
    };

    enum ExLength
    {
        etMicrometre   = 0,
        etMillimetre   = 1,
        etCentimetre   = 2,
        etMetre        = 3,
    };

    enum ExTemperature
    {
        etCelsius      = 0,
        etFahrenheit   = 1,
        etKelvin       = 2,
    };

    enum ExRatio
    {
        erFraction     = 0,     //!< Ratio as decimal value between 0 and 1
        erPercent      = 1,     //!< Ratio as percent value between 0 and 100
    };

public:
    static int countInGroup(ExUnitGroup eUnitGroup);        //!< Number of units for given group
    static QString text(ExUnitGroup eUnitGroup, int iUnit); //!< Text for the unit
    static int unitFromText(ExUnitGroup eUnitGroup, const QString& sText); //!< Unit from text, returns -1 when not found
    static double convertValue(double dValue, ExUnitGroup eUnitGroup, int iUnitFrom, int iUnitTo);
};

#endif // UNITS_H
