//=============================================================================
//
//   File : kvi_kvs_hash.cpp
//   Created on Tue 07 Oct 2003 01:22:37 by Szymon Stefanek
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

#include "kvi_kvs_hash.h"

KviKvsHash::KviKvsHash()
{
	m_pDict = new QDict<KviKvsVariant>(17,false);
	m_pDict->setAutoDelete(true);
}

KviKvsHash::KviKvsHash(const KviKvsHash &h)
{
	m_pDict = new QDict<KviKvsVariant>();
	m_pDict->setAutoDelete(true);
	QDictIterator<KviKvsVariant> it(*(h.m_pDict));
	while(it.current())
	{
		m_pDict->replace(it.currentKey(),new KviKvsVariant(*(it.current())));
		++it;
	}
}

KviKvsHash::~KviKvsHash()
{
	delete m_pDict;
}

void KviKvsHash::appendAsString(QString &szBuffer) const
{
	QDictIterator<KviKvsVariant> it(*m_pDict);
	bool bNeedComma = false;
	while(KviKvsVariant * s = it.current())
	{
		if(bNeedComma)szBuffer.append(',');
		else bNeedComma = true;
		s->appendAsString(szBuffer);
		++it;
	}
}

KviKvsVariant * KviKvsHash::get(const QString &szKey)
{
	KviKvsVariant * v = m_pDict->find(szKey);
	if(v)return v;
	v = new KviKvsVariant();
	m_pDict->replace(szKey,v);
	return v;
}
