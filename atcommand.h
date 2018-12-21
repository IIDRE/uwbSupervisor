#ifndef ATCOMMAND_H
#define ATCOMMAND_H

#include <QList>
#include <QString>
#include <QByteArray>

class AtCommand {
public:
    enum fsmstate{
        req,
        searchingExtendedCommandChar,
        parsingCommand,
        parsingParam
    };
    enum class CommandType{
        NOT_DEFINED,
        ASSIGNATION,
        REQUEST,
        NOTIFICATION,
        RESPONSE,
        INFO,
        COMMAND,
      //  ERROR,
        RESPONSE_ERROR,
        RESPONSE_OK
    };
    class DefChars{
    public:
        static const char* AT;

        static const char* CommandDefinitionEnd;
        static const char* ExtendedCommandIndicatorsSet;
        static const char  Assign;
        static const char  Request;
        static const char  Notification;
        static const char  Command;
        static const char  ExtendedCommandIndicator;
        static const char  CommandLineTerminator;
        static const char  CommandLineTerminatorAlt;
        static const char  ParamSeparator;
    };


    AtCommand();

    virtual ~AtCommand();

    QString         getCommand();
    bool            setCommand(const QString& c);

    AtCommand::CommandType  getType();
    bool                    setType(AtCommand::CommandType ct);

    bool addParam(QString param);
    bool addParam(const char* param);
    template<typename T>
    bool addParam(T numericParam)
    {
        QString param;
        param = QString::number(numericParam);
        params.push_back(param);
        return true;
    }

    QByteArray  toData();
    bool        parse(const QByteArray& source);
    bool        parse(const QByteArray &source,int& index);

    int         numParams();
    QString     getParam(int position);

    QByteArray  getStreamSource();
    bool                hasAtPrefix();
    bool                setAtPrefix(bool p);


protected:
    QString                     command;
    QList<QString>              params;

    AtCommand::CommandType      type;
    bool                        extendedCommandFlag;

    QByteArray                  streamSource;
    bool                        hasATPrefix;
    char                        extendedCommandChar;


private:
    bool                        terminatingAtcChar(QChar& c);



};
#endif // ATCOMMAND_H
