/*Copyright Iidre SAS 01 janvier 2019 tech@iidre.com

Ce logiciel est un programme informatique servant à superviser et configurer le réseau UWB du kit Iidre.

Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILLtelle que diffusée par le CEA, le CNRS et l'INRIA
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe à
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.*/
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
