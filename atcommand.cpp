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
#include "atcommand.h"

const char* AtCommand::DefChars::AT                              = "AT";
const char* AtCommand::DefChars::CommandDefinitionEnd            = "=:";
const char  AtCommand::DefChars::Assign                          = '=';
const char  AtCommand::DefChars::Request                         = '?';
const char  AtCommand::DefChars::Notification                    = ':';
const char* AtCommand::DefChars::ExtendedCommandIndicatorsSet    = "+*$#";
const char  AtCommand::DefChars::ExtendedCommandIndicator        = '+';
const char  AtCommand::DefChars::CommandLineTerminator           = 0x0A;
const char  AtCommand::DefChars::CommandLineTerminatorAlt        = 0x0D;
const char  AtCommand::DefChars::ParamSeparator                  = ',';

AtCommand::AtCommand()
    :command(QString(""))
    ,type(AtCommand::CommandType::NOT_DEFINED)
    ,extendedCommandFlag(false)
    ,hasATPrefix(false)
    ,extendedCommandChar(0x0)
{



}

AtCommand::~AtCommand()
{

}
bool    AtCommand::addParam(QString param)
{
    params.push_back(param);
    return true;
}
bool    AtCommand::addParam(const char *param)
{
    params.push_back(QString(param));
    return true;
}
QString AtCommand::getCommand()
{
    return command;
}
bool    AtCommand::setCommand(const QString& c)
{
    command = c;
    return true;
}


QByteArray AtCommand::toData()
{
    QByteArray toserve;

    toserve.clear();
    if(hasATPrefix)
    {
        toserve.append(AtCommand::DefChars::AT);
    }
    if(type == AtCommand::CommandType::RESPONSE_OK)
    {
        toserve+="OK";
        return toserve;
    }else if (type == AtCommand::CommandType::RESPONSE_ERROR)
    {
        toserve+="ERROR";
        return toserve;
    }
    if(extendedCommandFlag == true)
    {
        toserve.append(AtCommand::DefChars::ExtendedCommandIndicator);
    }
//    if(extendedCommandChar!=0x0)
//    {
//        toserve.append(extendedCommandChar);
//    }

    toserve+= command.toLatin1();
    switch (type) {
    case AtCommand::CommandType::COMMAND:
        toserve.append(AtCommand::DefChars::CommandLineTerminatorAlt);
        toserve.append(AtCommand::DefChars::CommandLineTerminator);
        return toserve;
        break;
    case AtCommand::CommandType::NOTIFICATION:
        toserve.append(AtCommand::DefChars::Notification);
        break;
    case AtCommand::CommandType::ASSIGNATION:
        toserve.append(AtCommand::DefChars::Assign);
        break;
    case AtCommand::CommandType::REQUEST:
    default:
        toserve.append(AtCommand::DefChars::Request);
        break;
    }
    int numParams = 0;
    for(QString param: params)
    {
        if(numParams!=0)
        {
            toserve.append(AtCommand::DefChars::ParamSeparator);
        }
        toserve+=param.toLatin1();
        ++numParams;
    }
    toserve.append(AtCommand::DefChars::CommandLineTerminatorAlt);
    toserve.append(AtCommand::DefChars::CommandLineTerminator);
    return toserve;
}

int AtCommand::numParams()
{
    return params.length();
}

QString  AtCommand::getParam(int position)
{
    if(position>=params.length())
    {
        return QString("");
    }
    return params.at(position);
}
QByteArray AtCommand::getStreamSource()
{
    return streamSource;
}
bool AtCommand::parse(const QByteArray &source)
{
    int index = 0;
    return parse(source,index);
}

/*
bool AtCommand::parse(const QByteArray& source,int& index)
{
    int offset = 0;
    int size;
    bool continueParse;

    if(index >= source.length())
    {
        return false;
    }

    QString current;
    QString buffer;

    streamSource = source;
    size = source.length()-index;
    params.clear();
    if(size < 3)
    {
        type = AtCommand::CommandType::ERROR;
        return false;
    }

    continueParse = true;
    while(continueParse == true)
    {
        current = source.at(index+ offset);
        current.toUpper();
        if(current.compare(AtCommand::Definitions::ExtendedCommandIndicator) == 0)
        {
            type = AtCommand::CommandType::RESPONSE;
            continueParse = false;
        }
        if(current.compare(AtCommand::Definitions::AT[0]))
        {
            type = AtCommand::CommandType::REQUEST;
            continueParse = false;
        }
        ++offset;
        if((index+offset)>=source.length())
        {
            return false;
        }
    }
    //ConsoleLog("parsing [" << source << "]");

    if(type == AtCommand::CommandType::REQUEST)
    {
        current = (QString(source.at(index+ offset))
                   + QString(source.at(index+ offset+1)));
        current = current.toUpper();
        if(current.compare(AtCommand::Definitions::AT) != 0)
        {
            return false;
        }
        offset+=2:
    }
    current = source[index+ offset];
    if(QString(AtCommand::Definitions::ExtendedCommandIndicatorsSet).indexOf(current)!= -1)
    {
        ++offset;
        extendedCommandFlag = true;
    }
    else
    {
        extendedCommandFlag = false;
    }
    //ConsoleLog("offset = " << offset);
    index+= offset;
    if(index>=size)
    {
        type = AtCommand::CommandType::ERROR;
        return false;
    }

    //command extraction
    continueParse = true;
    command.clear();
    while(continueParse == true)
    {
        current = source.at(index);
        //ConsoleLog("index[" << index << "] : character [" << current << "]");
        if(current.compare(QString(AtCommand::Definitions::CommandLineTerminator)) == 0)
        {
            continueParse = false;
        }
        else if(QString(AtCommand::Definitions::CommandDefinitionEnd).indexOf(current)!= -1)
        {

            switch (current.at(0).toLatin1()) {
            case AtCommand::Definitions::Assign:
                type = AtCommand::CommandType::ASSIGNATION;
                break;
            case AtCommand::Definitions::Request:
                type = AtCommand::CommandType::REQUEST;
                break;
            default:

                return false;
            }
            continueParse = false;
        }
        else
        {
            command+=current;
        }
        ++index;
        if(index>=size)
        {
            continueParse = false;
            return true;
        }
    }

    //paramsCollection
    if(index>=size)
    {
        return true;
    }
    buffer.clear();
    continueParse = true;
    params.clear();
    //ConsoleLog("parsing params since index [" << index << "]");
    while(continueParse)
    {
        current = source.at(index);
        //ConsoleLog("index[" << index << "] : character [" << current << "]");

        if(current.compare(QString(AtCommand::Definitions::ParamSeparator))!= 0)
        {
            //
            if(current.toLatin1().data()[0] != 0x13)
            {
                buffer+=source.at(index);
            }
        }
        else
        {
            params.push_back(buffer);
            buffer.clear();
            //param ended
        }

        ++index;
        if(index>=size)
        {
            if(buffer.length()>0)
            {
                params.push_back(buffer);
            }
            continueParse = false;
        }
    }
    return true;
}*/
QString _parser_helper_string("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+.");
bool AtCommand::hasAtPrefix()
{
    return hasATPrefix;
}


bool AtCommand::setAtPrefix(bool p)
{
    hasATPrefix = p;
    return true;
}

bool AtCommand::parse(const QByteArray &source, int &index)
{
    int offset;
    AtCommand::fsmstate state;
    int sz = source.length();
    bool continueParse;
    QChar current;
    QChar buffer;
    QString paramBuffer;
    hasATPrefix = false;

    state = AtCommand::req;
    continueParse = true;
    command.clear();
    params.clear();
    if(index>=sz)
    {
        return false;
    }
    while(continueParse == true)
    {
        current = source.at(index);
        if(_parser_helper_string.indexOf(current) != -1)
        {
            continueParse = false;
        }
        else
        {
            ++index;
        }
        if(index>=sz)
        {
            return false;
        }
    }

    continueParse = true;
    while(continueParse == true)
    {
        current = source.at(index);
        current = current.toUpper();
        switch(state)
        {
        case AtCommand::req:
            if(current == 'A')
            {
                if((index+1)>=sz)
                {
                    //end of command.
                }
                else
                {
                    buffer = source.at(index+1);
                    buffer = buffer.toUpper();

                    if(buffer == 'T')
                    {
                        state = AtCommand::searchingExtendedCommandChar;
                        ++index;
                        type = AtCommand::CommandType::REQUEST;
                        hasATPrefix = true;
                    }
                    else {
                        state = AtCommand::parsingCommand;
                        type = AtCommand::CommandType::NOTIFICATION;
                        command.push_back(current);
                    }
                }
            }
            else if(current == '+')
            {
                if(command.length()>0)
                {
                    //error of command
                    return false;
                }
                extendedCommandChar = current.toLatin1();
                state = AtCommand::parsingCommand;
                extendedCommandFlag = true;
                type = AtCommand::CommandType::NOTIFICATION;
            }
            else if(QString("BCDEFGHIJKLMNOPQRSTUVWXYZ0123456789").indexOf(current) != -1)
            {
                state = AtCommand::parsingCommand;
                command.push_back(current);
            }




            break;
        case AtCommand::searchingExtendedCommandChar:
            if(current != AtCommand::DefChars::ExtendedCommandIndicator)
            {
                extendedCommandFlag = false;
                command.push_back(current);
            }
            else
            {
                extendedCommandFlag = true;
            }
            state = AtCommand::parsingCommand;
            break;
        case AtCommand::parsingCommand:
            if(current == AtCommand::DefChars::Notification)
            {
                type = AtCommand::CommandType::NOTIFICATION;
                state = AtCommand::parsingParam;
                paramBuffer.clear();

            } else if(current == AtCommand::DefChars::Assign)
            {
                type = AtCommand::CommandType::ASSIGNATION;
                state = AtCommand::parsingParam;
            } else if(terminatingAtcChar(current) == true)
            {
                return true;
            }else if(current == '?')
            {
                type = AtCommand::CommandType::REQUEST;
                state = AtCommand::parsingParam;
            } else {
                command.push_back(current);
            }

            break;
        case AtCommand::parsingParam:
            if(current == AtCommand::DefChars::ParamSeparator)
            {
                params.push_back(paramBuffer);
                paramBuffer.clear();
            }else if(terminatingAtcChar(current))
            {
                if(paramBuffer.length()>0)
                    params.push_back(paramBuffer);
                return true; /**< ! ret point*/
            }else{
                paramBuffer.push_back(current);
            }
            break;
        }

        ++index;
        if(index >= sz)
        {
            continueParse = false;
        }
    }
    return true;


}
bool    AtCommand::terminatingAtcChar(QChar &c)
{
    if((c == AtCommand::DefChars::CommandLineTerminator)
            || (c== AtCommand::DefChars::CommandLineTerminatorAlt))
    {
        return true;
    }
    return false;
}
AtCommand::CommandType AtCommand::getType()
{
    return type;
}
bool AtCommand::setType(AtCommand::CommandType ct)
{
    type = ct;
    switch (type) {
    case AtCommand::CommandType::NOTIFICATION:
        hasATPrefix = false;

        break;
    default:
        hasATPrefix = true;
        break;
    }
    return true;
}
