#ifndef _KVI_IRCCONNECTIONSTATEDATA_H_
#define _KVI_IRCCONNECTIONSTATEDATA_H_
//=============================================================================
//
//   File : kvi_ircconnectionstatedata.h
//   Created on Sat 26 Jun 2004 09:31:52 by Szymon Stefanek
//
//   This file is part of the KVIrc IRC client distribution
//   Copyright (C) 2004 Szymon Stefanek <pragma at kvirc dot net>
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
//=============================================================================

#include "kvi_settings.h"
#include "kvi_time.h"
#include "kvi_qstring.h"

//
// This class is used as container for miscelaneous connection state variables
// that do not have a class for their own.
// If you need to add some "minor" variable to the connection state then
// this is the right place for it.
//

class KVIRC_API KviIrcConnectionStateData
{
	friend class KviIrcConnection;
	friend class KviServerParser;
	friend class KviUserParser;
public:
	KviIrcConnectionStateData();
	~KviIrcConnectionStateData();
protected:
	bool         m_bSentQuit;                     // have we sent the quit message for this connection ?
	unsigned int m_uLoginNickIndex;               // the index of the identity nicknames used until now (see KviIrcConnection::loginToIrcServer())
	QString      m_szCommandToExecAfterConnect;   // yes.. this is a special command to execute after connection
	bool         m_bSimulateUnexpectedDisconnect; // this is set to true if we have to simulate an unexpected disconnect even if we have sent a normal quit message
	kvi_time_t   m_tLastReceivedChannelWhoReply;  // the time that we have received our last channel who reply
	kvi_time_t   m_tLastSentChannelWhoRequest;    // the time that we have sent our last channel who request
public:
	bool sentQuit(){ return m_bSentQuit; };
	void setSentQuit(){ m_bSentQuit = true; };
	
	kvi_time_t lastReceivedChannelWhoReply(){ return m_tLastReceivedChannelWhoReply; };
	void setLastReceivedChannelWhoReply(kvi_time_t tTime){ m_tLastReceivedChannelWhoReply = tTime; };
	
	kvi_time_t lastSentChannelWhoRequest(){ return m_tLastSentChannelWhoRequest; };
	void setLastSentChannelWhoRequest(kvi_time_t tTime){ m_tLastSentChannelWhoRequest = tTime; };	
	
	bool simulateUnexpectedDisconnect(){ return m_bSimulateUnexpectedDisconnect; };
	void setSimulateUnexpectedDisconnect(){ m_bSimulateUnexpectedDisconnect = true; };

	unsigned int loginNickIndex(){ return m_uLoginNickIndex; };
	void setLoginNickIndex(unsigned int uNickIdx){ m_uLoginNickIndex = uNickIdx; };
	
	const QString & commandToExecAfterConnect(){ return m_szCommandToExecAfterConnect; };
	void setCommandToExecAfterConnect(const QString &szCmd){ m_szCommandToExecAfterConnect = szCmd; };
};

#endif //!_KVI_IRCCONNECTIONSTATEDATA_H_
