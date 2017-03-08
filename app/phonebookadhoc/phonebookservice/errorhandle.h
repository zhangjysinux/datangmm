#ifndef ERRORHANDLE_H
#define ERRORHANDLE_H

#include <QString>
#include <QDebug>

enum ErrorHandleType
{
    ERRORHANDLETYPE_NOERROR = 0
};


class ErrorHandle
{
public:
    inline int getLastErrorType() const
    {
        return m_lastErrorType;
    }

    inline QString getLastErrorString() const
    {
        return m_lastErrorString;
    }

    inline bool lastErrorIsNoError() const
    {
        return m_lastErrorType == ERRORHANDLETYPE_NOERROR;
    }

protected:
    inline int setLastError(const int &type, const QString &str)
    {
        m_lastErrorType = type;
        m_lastErrorString = str;
        return m_lastErrorType;
    }

private:
    int m_lastErrorType;
    QString m_lastErrorString;
};

#endif // ERRORHANDLE_H
