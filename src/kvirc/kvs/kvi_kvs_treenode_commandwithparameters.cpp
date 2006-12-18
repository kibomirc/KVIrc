//=============================================================================
//
//   File : kvi_kvs_treenode_commandwithparameters.cpp
//   Created on Thu 09 Oct 2003 01:57:10 by Szymon Stefanek
//
//   This file is part of the KVIrc IRC client distribution
//   Copyright (C) 2003 Szymon Stefanek <pragma at kvirc dot net>
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

#include "kvi_kvs_treenode_commandwithparameters.h"
#include "kvi_kvs_treenode_datalist.h"

KviKvsTreeNodeCommandWithParameters::KviKvsTreeNodeCommandWithParameters(const QChar * pLocation,const QString &szCmdName,KviKvsTreeNodeDataList * params)
: KviKvsTreeNodeCommand(pLocation,szCmdName)
{
	m_pParams = params;
	m_pParams->setParent(this);
}

KviKvsTreeNodeCommandWithParameters::~KviKvsTreeNodeCommandWithParameters()
{
	delete m_pParams;
}

void KviKvsTreeNodeCommandWithParameters::contextDescription(QString &szBuffer)
{
#ifdef COMPILE_NEW_KVS
	KviQString::sprintf(szBuffer,"Command With Parameters \"%s\"",&m_szCmdName);
#endif
}


void KviKvsTreeNodeCommandWithParameters::dump(const char * prefix)
{
#ifdef COMPILE_NEW_KVS
	debug("%s CommandWithParameters(%s)",prefix,m_szCmdName.utf8().data());
	dumpSwitchList(prefix);
	dumpParameterList(prefix);
#endif
}

void KviKvsTreeNodeCommandWithParameters::dumpParameterList(const char * prefix)
{
#ifdef COMPILE_NEW_KVS
	QString tmp = prefix;
	tmp.append("  ");
	m_pParams->dump(tmp.utf8().data());
#endif
}
