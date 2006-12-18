//=============================================================================
//
//   File : kvi_kvs_scriptaddonmanager.cpp
//   Created on Thu 31 Mar 2005 01:21:23 by Szymon Stefanek
//
//   This file is part of the KVIrc IRC client distribution
//   Copyright (C) 2005 Szymon Stefanek <pragma at kvirc dot net>
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

#include "kvi_kvs_scriptaddonmanager.h"
#include "kvi_kvs_script.h"
#include "kvi_config.h"
#include "kvi_window.h"
#include "kvi_iconmanager.h"

KviKvsScriptAddonManager * KviKvsScriptAddonManager::m_pInstance = 0;




KviKvsScriptAddon::KviKvsScriptAddon(
			const QString &szName,
			const QString &szVersion,
			const QString &szVisibleNameCode,
			const QString &szDescriptionCode,
			const QString &szUninstallCallbackCode,
			const QString &szIconId
) : KviHeapObject(), m_szName(szName), m_szVersion(szVersion), m_szIconId(szIconId)
{
	allocateScripts(szVisibleNameCode,szDescriptionCode,szUninstallCallbackCode);
	m_pConfigureCallback = 0;
	m_pHelpCallback = 0;
}

KviKvsScriptAddon::KviKvsScriptAddon(const KviKvsScriptAddon &a)
: KviHeapObject()
{
	m_szName = a.m_szName;
	m_szVersion = a.m_szVersion;
	m_szIconId = a.m_szIconId;
	allocateScripts(a.m_pVisibleNameScript->code(),a.m_pDescriptionScript->code(),a.m_pUninstallCallback ? a.m_pUninstallCallback->code() : QString::null);
	m_pConfigureCallback = 0;
	setConfigureCallback(a.m_pConfigureCallback ? a.m_pConfigureCallback->code() : QString::null);
	m_pHelpCallback = 0;
	setHelpCallback(a.m_pHelpCallback ? a.m_pHelpCallback->code() : QString::null);
}

KviKvsScriptAddon::KviKvsScriptAddon()
: KviHeapObject()
{
	m_pVisibleNameScript = 0;
	m_pDescriptionScript = 0;
	m_pUninstallCallback = 0;
	m_pConfigureCallback = 0;
	m_pHelpCallback = 0;
}

KviKvsScriptAddon::~KviKvsScriptAddon()
{
	if(m_pVisibleNameScript)delete m_pVisibleNameScript;
	if(m_pDescriptionScript)delete m_pDescriptionScript;
	if(m_pUninstallCallback)delete m_pUninstallCallback;
	if(m_pConfigureCallback)delete m_pConfigureCallback;
	if(m_pHelpCallback)delete m_pHelpCallback;
}

QPixmap * KviKvsScriptAddon::icon()
{
	return g_pIconManager->getBigIcon(m_szIconId.isEmpty() ? QString(KVI_BIGICON_KVS) : m_szIconId);
}

const QString & KviKvsScriptAddon::visibleName()
{
	if(!m_pVisibleNameScript)return m_szVisibleName;
	if(!m_pVisibleNameScript->run(g_pActiveWindow,0,m_szVisibleName))m_szVisibleName = m_pVisibleNameScript->code();
	return m_szVisibleName;
}

const QString & KviKvsScriptAddon::description()
{
	if(!m_pDescriptionScript)return m_szDescription;
	if(!m_pDescriptionScript->run(g_pActiveWindow,0,m_szDescription))m_szDescription = m_pDescriptionScript->code();
	return m_szDescription;
}

const QString & KviKvsScriptAddon::visibleNameCode()
{
	return m_pVisibleNameScript->code();
}

const QString & KviKvsScriptAddon::descriptionCode()
{
	return m_pDescriptionScript->code();
}

const QString & KviKvsScriptAddon::uninstallCallbackCode()
{
	return m_pUninstallCallback->code();
}

const QString & KviKvsScriptAddon::configureCallbackCode()
{
	if(m_pConfigureCallback)return m_pConfigureCallback->code();
	return QString::null;
}

const QString & KviKvsScriptAddon::helpCallbackCode()
{
	if(m_pHelpCallback)return m_pHelpCallback->code();
	return QString::null;
}

bool KviKvsScriptAddon::load(KviConfig * cfg,const QString &szName)
{
	m_szName = szName;
	cfg->setGroup(m_szName);
	m_szVersion = cfg->readQStringEntry("Version");
	m_szIconId = cfg->readQStringEntry("IconId");
	if(m_szVersion.isEmpty())return false;
	QString tmp1,tmp2,tmp3;
	tmp1 = cfg->readQStringEntry("VisibleNameCode");
	tmp2 = cfg->readQStringEntry("DescriptionCode");
	tmp3 = cfg->readQStringEntry("UninstallCallback");
	if(tmp1.isEmpty())return false;
	allocateScripts(tmp1,tmp2,tmp3);
	tmp1 = cfg->readQStringEntry("ConfigureCallback");
	if(!tmp1.isEmpty())
		setConfigureCallback(tmp1);
	tmp1 = cfg->readQStringEntry("HelpCallback");
	if(!tmp1.isEmpty())
		setHelpCallback(tmp1);
	return true;
}

void KviKvsScriptAddon::setConfigureCallback(const QString &szConfigureCallbackCode)
{
	if(m_pConfigureCallback)delete m_pConfigureCallback;
	if(szConfigureCallbackCode.isEmpty())
	{
		m_pConfigureCallback = 0;
		return;
	}

	QString szKvsName = "addon::";
	szKvsName += m_szName;

	QString szTmp;

	szTmp = szKvsName;
	szTmp += "::configure";
	m_pConfigureCallback = new KviKvsScript(szTmp,szConfigureCallbackCode,KviKvsScript::InstructionList);
}

void KviKvsScriptAddon::setHelpCallback(const QString &szHelpCallbackCode)
{
	if(m_pHelpCallback)delete m_pHelpCallback;
	if(szHelpCallbackCode.isEmpty())
	{
		m_pHelpCallback = 0;
		return;
	}

	QString szKvsName = "addon::";
	szKvsName += m_szName;

	QString szTmp;

	szTmp = szKvsName;
	szTmp += "::help";
	m_pHelpCallback = new KviKvsScript(szTmp,szHelpCallbackCode,KviKvsScript::InstructionList);
}

void KviKvsScriptAddon::allocateScripts(const QString &szVisibleNameCode,const QString &szDescriptionCode,const QString &szUninstallCallbackCode)
{
	QString szKvsName = "addon::";
	szKvsName += m_szName;

	QString szTmp;

	szTmp = szKvsName;
	szTmp += "::name";
	m_pVisibleNameScript = new KviKvsScript(szTmp,szVisibleNameCode,KviKvsScript::Parameter);
	szTmp = szKvsName;
	szTmp += "::description";
	m_pDescriptionScript = new KviKvsScript(szTmp,szDescriptionCode,KviKvsScript::Parameter);
	szTmp = szKvsName;
	szTmp += "::uninstall";
	m_pUninstallCallback = new KviKvsScript(szTmp,szUninstallCallbackCode,KviKvsScript::InstructionList);
}

void KviKvsScriptAddon::save(KviConfig * cfg)
{
	cfg->setGroup(m_szName);
	cfg->writeEntry("Version",m_szVersion);
	cfg->writeEntry("VisibleNameCode",visibleNameCode());
	cfg->writeEntry("DescriptionCode",descriptionCode());
	cfg->writeEntry("UninstallCallback",uninstallCallbackCode());
	cfg->writeEntry("ConfigureCallback",configureCallbackCode());
	cfg->writeEntry("HelpCallback",helpCallbackCode());
	cfg->writeEntry("IconId",m_szIconId);
}

void KviKvsScriptAddon::executeUninstallCallback(KviWindow * pWnd)
{
	if(!m_pUninstallCallback)return;
	m_pUninstallCallback->run(pWnd);
}

void KviKvsScriptAddon::executeConfigureCallback(KviWindow * pWnd)
{
	if(!m_pConfigureCallback)return;
	m_pConfigureCallback->run(pWnd);
}

void KviKvsScriptAddon::executeHelpCallback(KviWindow * pWnd)
{
	if(!m_pHelpCallback)return;
	m_pHelpCallback->run(pWnd);
}





KviKvsScriptAddonManager::KviKvsScriptAddonManager()
{
	m_pInstance = this;
	m_bLoaded = false;
	m_pAddonDict = new QDict<KviKvsScriptAddon>(17,false);
	m_pAddonDict->setAutoDelete(true);
}

KviKvsScriptAddonManager::~KviKvsScriptAddonManager()
{
	delete m_pAddonDict;
}

void KviKvsScriptAddonManager::init()
{
	if(KviKvsScriptAddonManager::instance())
	{
		debug("WARNING: Trying to create the KviKvsScriptAddonManager twice!");
		return;
	}
	(void)new KviKvsScriptAddonManager();
}

void KviKvsScriptAddonManager::done()
{
	if(!KviKvsScriptAddonManager::instance())
	{
		debug("WARNING: Trying to destroy the KviKvsScriptAddonManager twice!");
		return;
	}
	delete KviKvsScriptAddonManager::instance();
}

void KviKvsScriptAddonManager::load(const QString &szFileName)
{
	// in fact we implement delayed loading
	// so this function only stores the filename
	// from which we will load at the first request
	m_szFileName = szFileName;
	// this to make sure that we reload the addons
	// if someone explicitly requests a load after we have already loaded
	// (this does not happen in kvirc tough at the moment)
	m_bLoaded = false;
}

void KviKvsScriptAddonManager::save(const QString &szFileName)
{
	if(!m_bLoaded)return; // nothing to store anyway
	// we're stored here from now on...
	m_szFileName = szFileName;

	KviConfig cfg(szFileName,KviConfig::Write);

	cfg.clear();

	QDictIterator<KviKvsScriptAddon> it(*m_pAddonDict);
	
	while(KviKvsScriptAddon * a = it.current())
	{
		cfg.setGroup(a->name());
		a->save(&cfg);
		++it;
	}
}

void KviKvsScriptAddonManager::delayedLoad()
{
	if(m_bLoaded)return; // already loaded
	m_bLoaded = true;
	// ::load() might be never called if we don't have
	// a scriptaddons.kvc file on disk, KviApp checks that.
	// So finally m_szFileName may be empty here
	if(m_szFileName.isEmpty())return;

	KviConfig cfg(m_szFileName,KviConfig::Read);

	QDict<KviConfigGroup> * d = cfg.dict();
	if(!d)return;
	
	QDictIterator<KviConfigGroup> it(*d);
	while(it.current())
	{
		QString szName = it.currentKey();
		KviKvsScriptAddon * a = new KviKvsScriptAddon();
		if(a->load(&cfg,szName))
			m_pAddonDict->replace(szName,a);
		else
			delete a;
		++it;
	}
}

QDict<KviKvsScriptAddon> * KviKvsScriptAddonManager::addonDict()
{
	if(!m_bLoaded)delayedLoad();
	return m_pAddonDict;
}


bool KviKvsScriptAddonManager::isValidVersionString(const QString &szVersion)
{
	QStringList sl = QStringList::split(".",szVersion);
	if(sl.isEmpty())return false;
	// must all be numbers
	for(QStringList::Iterator it = sl.begin();it != sl.end();++it)
	{
		bool bOk;
		int i = (*it).toInt(&bOk);
		if(!bOk)return false;
		if(i < 0)return false;
	}
	return true;
}

int KviKvsScriptAddonManager::compareVersions(const QString &szVersion1,const QString &szVersion2)
{
	QStringList sl1 = QStringList::split(".",szVersion1);
	QStringList sl2 = QStringList::split(".",szVersion2);

	QStringList::Iterator it1 = sl1.begin();
	QStringList::Iterator it2 = sl2.begin();
	while((it1 != sl1.end()) && (it2 != sl2.end()))
	{
		bool bOk;
		int i1 = (*it1).toInt(&bOk);
		if(!bOk)return 1;
		int i2 = (*it2).toInt(&bOk);
		if(!bOk)return -1;
		if(i1 != i2)
		{
			// field not equal
			if(i1 > i2)return -1;
			else return 1;
		}
		it1++;
		it2++;
	}
	// both are equal until now
	if(it1 != sl1.end())return -1; // 1 has at least one field more
	if(it2 != sl2.end())return 1;  // 2 has at least one field more
	// both are equal also in length
	return 0;
}

bool KviKvsScriptAddonManager::registerAddon(KviKvsScriptAddonRegistrationData * d)
{
	if(findAddon(d->szName))return false;
	KviKvsScriptAddon * a = new KviKvsScriptAddon(
			d->szName,
			d->szVersion,
			d->szVisibleNameScript,
			d->szDescriptionScript,
			d->szUninstallCallbackScript,
			d->szIconId);
	m_pAddonDict->replace(d->szName,a);
	return true;
}

KviKvsScriptAddon * KviKvsScriptAddonManager::findAddon(const QString &szName)
{
	if(!m_bLoaded)delayedLoad();
	return m_pAddonDict->find(szName);
}

bool KviKvsScriptAddonManager::unregisterAddon(const QString &szName,KviWindow * pWnd,bool bExecuteUninstallCallback)
{
	KviKvsScriptAddon * a = findAddon(szName);
	if(!a)return false;

	// remove the addon before executing the uninstall callback
	// so the user effectively can't call addon.unregister on itself in the uninstall callback code :D
	m_pAddonDict->setAutoDelete(false);
	m_pAddonDict->remove(szName);
	m_pAddonDict->setAutoDelete(true);

	if(bExecuteUninstallCallback)
		a->executeUninstallCallback(pWnd);

	delete a;
	return true;
}

void KviKvsScriptAddonManager::clear()
{
	if(!m_bLoaded)delayedLoad();
	m_pAddonDict->clear();
}
