/****************************************************************************************
 ** Server is an application to manage several clients inside a thread.
 ** Copyright (C) 2013  Francesc Martinez <es.linkedin.com/in/cescmm/en>
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 2.1 of the License, or (at your option) any later version.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***************************************************************************************/

#ifndef HEADER_H
#define HEADER_H

#include <QString>
#include "QLogger.h"

using namespace QLogger;

//sent by client
const QString NEW_CLIENT = QString("New Client");
const QString APPLY_PPC = QString("Apply PPC");
const QString GIVE_BACK_PPC = QString("Give Back PPC");
const QString GIVE_BACK_PPC_RESULT = QString("Give Back PPC Result");
const QString UPDATE_PPC_STATUS = QString("Update PPC Status");


//sent by server
const QString UPDATE_CLIENT_PPC_STATUS = "Update Client PPC status";
const QString APPLY_SUCCESS = "Apply Success";
const QString APPLY_FAILED = "Apply Failed";
const QString GIVE_BACK_SUCCESS = "Give Back Success";
const QString GIVE_BACK_FAILED = "Give Back Failed";

const QString TIMEOUT = "Timeout";
const QString BUSY = "Busy";
const QString IDLE = "Idle";
const QString OCCUPY = "Occupy";
const QString UNKNOWN = "Unknown";

typedef struct  
{
	QString IP;
	QString status;
	QString owner;
}IPStatus;




#endif // PROTOCOL_H
