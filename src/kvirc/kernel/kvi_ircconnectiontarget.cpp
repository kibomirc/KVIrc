//=============================================================================
//
//   File : kvi_ircconnectiontarget.cpp
//   Created on Tue 08 Jun 2004 14:11:59 by Szymon Stefanek
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

#define __KVIRC__

#include "kvi_ircconnectiontarget.h"
#include "kvi_ircserver.h"
#include "kvi_proxydb.h"

KviIrcConnectionTarget::KviIrcConnectionTarget(const KviIrcNetwork * pNetwork,
							const KviIrcServer * pServer,
							const KviProxy * pProxy,
							const QString &szBindAddress)
{
	m_pNetwork = new KviIrcNetwork(*pNetwork);
	m_pServer = new KviIrcServer(*pServer);
	m_pProxy = pProxy ? new KviProxy(*pProxy) : 0;
	m_szBindAddress = szBindAddress;
}

KviIrcConnectionTarget::~KviIrcConnectionTarget()
{
	delete m_pNetwork;
	delete m_pServer;
	if(m_pProxy)delete m_pProxy;
}

const QString & KviIrcConnectionTarget::networkName()
{
	return m_pNetwork->name();
}

void KviIrcConnectionTarget::setNetworkName(const QString &szNetName)
{
	m_pNetwork->setName(szNetName);
}


void KviIrcConnectionTarget::clearProxy()
{
	if(!m_pProxy)return;
	delete m_pProxy;
	m_pProxy = 0;
}

