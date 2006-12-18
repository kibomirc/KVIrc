//=============================================================================
//
//   File : kvi_ircdatastreammonitor.cpp
//   Created on Sun 04 Jul 2004 21:27:22 by Szymon Stefanek
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

#include "kvi_ircdatastreammonitor.h"
#include "kvi_irccontext.h"

KviIrcDataStreamMonitor::KviIrcDataStreamMonitor(KviIrcContext * pContext)
: KviHeapObject()
{
	m_pMyContext = pContext;
	m_pMyContext->registerDataStreamMonitor(this);
}

KviIrcDataStreamMonitor::~KviIrcDataStreamMonitor()
{
	m_pMyContext->unregisterDataStreamMonitor(this);
}

