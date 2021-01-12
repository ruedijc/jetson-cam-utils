#ifndef VALUETYPES_H
#define VALUETYPES_H

#include <xiCoreGlobal.h>
#include <QString>

//! \brief Class for storing values similar to variant. Used in camera parameters.
class XICORE_API CxValue
{
public:
    enum Type
    {
        etInvalid,      //!< Marks an error, or undefined value
        etInt,          //!< Use m_intVal
        etFloat,        //!< Use m_floarVal
        etBool,         //!< Use m_boolVal
        etString,       //!< Use m_stringVal
    } m_eType;   //!< Value type

    union
    {
        int m_intVal;
        float m_floatVal;
        bool m_boolVal;
    };

    QString m_stringVal;    // we also need string in this class (and it cannot be inside union)

public:
    CxValue();
    CxValue(int nVal);
    CxValue(float fVal);
    CxValue(bool bVal);
    CxValue(const QString &sVal);
    bool operator ==(const CxValue& rOther) const;
    bool operator !=(const CxValue& rOther) const;
    bool operator <(const CxValue& rOther) const;
    bool operator >(const CxValue& rOther) const;
    CxValue operator +(const CxValue& rOther) const;
    CxValue operator -(const CxValue& rOther) const;
    CxValue operator *(const CxValue& rOther) const;
    CxValue operator /(const CxValue& rOther) const;

public:
    int toInt() const;      //!< Returns value as int (not implemented for etString)
    float toFloat() const;  //!< Returns value as float (for etInt and etFloat)
    bool toBool() const;   //!< Returns value as bool (value is 1)
    bool fromString(const QString& sString);    //!< Loading from string using the type already in m_eType
    QString toString() const;                   //!< Save value to string
};

#endif // VALUETYPES_H
