#ifndef _BROKER_H_
#define _BROKER_H_
//=======================================================================================
//
//   File : broker.h
//   Creation date : Tue Sep 19 09 2000 10:20:01 by Szymon Stefanek
//
//   This file is part of the KVirc irc client distribution
//   Copyright (C) 1999-2006 Szymon Stefanek (pragma at kvirc dot net)
//
//   This program is FREE software. You can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your opinion) any later version.
//
//   This program is distributed in the HOPE that it will be USEFUL,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program. If not, write to the Free Software Foundation,
//   Inc. ,59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
//=======================================================================================

#include "kvi_settings.h"
#include "kvi_string.h"
#include "kvi_list.h"
#include "kvi_sparser.h"

#include <qdict.h>
#include <qdatetime.h>
#include <qobject.h>

class KviConsole;
class KviDccBroker;
class KviWindow;
class KviDccBox;

#include "descriptor.h"

class KviDccZeroPortTag
{
public:
	QDateTime    m_tTimestamp;
	QString      m_szTag;
	unsigned int m_uFileSize; // outgoing file size, valid only for file transfers obviously
	unsigned int m_uResumePosition; // if 0 = no resume, valid only for file transfers obviously
};

class KviDccBroker : public QObject
{
	Q_OBJECT
public:
	KviDccBroker();
	~KviDccBroker();
protected:
	KviPtrList<KviDccBox>         * m_pBoxList;
	KviPtrList<KviWindow>         * m_pDccWindowList;
	QDict<KviDccZeroPortTag>      * m_pZeroPortTags;
public:
	KviDccZeroPortTag * addZeroPortTag();
	KviDccZeroPortTag * findZeroPortTag(const QString &szTag);
	void removeZeroPortTag(const QString &szTag);

	unsigned int dccWindowsCount(){ return m_pDccWindowList->count(); };
	unsigned int dccBoxCount();

	void unregisterDccBox(KviDccBox * box);
	void unregisterDccWindow(KviWindow *dcc);

	void rsendManage(KviDccDescriptor * dcc);
	void rsendAskForFileName(KviDccDescriptor * dcc);

	void handleChatRequest(KviDccDescriptor * dcc);


	void activeCanvasManage(KviDccDescriptor * dcc);
	void passiveCanvasExecute(KviDccDescriptor * dcc);

	void activeVoiceManage(KviDccDescriptor * dcc);
	void passiveVoiceExecute(KviDccDescriptor * dcc);

	void recvFileManage(KviDccDescriptor * dcc);
	void sendFileManage(KviDccDescriptor * dcc);

	bool handleResumeAccepted(const char * filename,const char * port,const char * szZeroPortTag);
	bool handleResumeRequest(KviDccRequest * dcc,const char * filename,const char * port,unsigned int filePos,const char * szZeroPortTag);

public slots:
	void rsendExecute(KviDccBox * box,KviDccDescriptor * dcc);
	void rsendExecute(KviDccDescriptor * dcc);

//	void activeChatExecute(KviDccBox * box,KviDccDescriptor * dcc);
	void executeChat(KviDccBox * box,KviDccDescriptor * dcc);

	void activeCanvasExecute(KviDccBox * box,KviDccDescriptor * dcc);
	void activeVoiceExecute(KviDccBox * box,KviDccDescriptor * dcc);

	void sendFileExecute(KviDccBox * box,KviDccDescriptor * dcc);
	void recvFileExecute(KviDccBox * box,KviDccDescriptor * dcc);


	void chooseSaveFileName(KviDccBox *box,KviDccDescriptor * dcc);
	void renameOverwriteResume(KviDccBox *box,KviDccDescriptor * dcc);
	void renameDccSendFile(KviDccBox *box,KviDccDescriptor * dcc);

	void cancelDcc(KviDccBox *box,KviDccDescriptor * dcc);
	void cancelDcc(KviDccDescriptor * dcc);

public:
	bool canUnload();
};

#endif
