#!/bin/sh

#################################################################################
#
# Generator for OptionsInstanceManager.h and OptionsInstanceManager.cpp
#
# This file is part of the KVirc IRC client distribution
# Copyright (C) 2001-2008 Szymon Stefanek (pragma at kvirc dot net)
#
#################################################################################


echo "" > OptionsInstanceManager.h

#################################################################################
cat >> OptionsInstanceManager.h <<EOF

#ifndef __OPTIONS_INSTANCES_H__
#define __OPTIONS_INSTANCES_H__

//=============================================================================
//
//   File : OptionsInstanceManager.h
//
//   This file is part of the KVirc IRC client distribution
//   Copyright (C) 2001-2008 Szymon Stefanek (pragma at kvirc dot net)
//
//   This program is FREE software. You can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your option) any later version.
//
//   This program is distributed in the HOPE that it will be USEFUL,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program. If not, write to the Free Software Foundation,
//   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
//=============================================================================

//
// Instance creation routines for the KVIrc options module
// DO NOT EDIT THIS FILE!! ALL CHANGES WILL BE LOST!!
// This file is automatically generated by mkcreateinstanceproc.sh
// so any change should go in that script
//

#include "KviOptionsWidget.h"
#include "KviModule.h"
#include "KviPointerList.h"
#include "KviQString.h"
#include "KviIconManager.h"

typedef struct _OptionsWidgetInstanceEntry OptionsWidgetInstanceEntry;


typedef struct _OptionsWidgetInstanceEntry
{
	KviOptionsWidget                           * (*createProc)(QWidget *);
	KviOptionsWidget                           * pWidget;   // singleton
	KviIconManager::SmallIcon                    eIcon;
	QString                                      szName;
	QString                                      szNameNoLocale;
	const char                                 * szClassName;
	int                                          iPriority;
	QString                                      szKeywords;
	QString                                      szKeywordsNoLocale;
	QString                                      szGroup;
	bool                                         bIsContainer;
	bool                                         bIsNotContained;
	KviPointerList<OptionsWidgetInstanceEntry> * pChildList;
	bool                                         bDoInsert; // a helper for OptionsDialog::fillListView()
} OptionsWidgetInstanceEntry;


class OptionsInstanceManager : public QObject
{
	Q_OBJECT
public:
	OptionsInstanceManager();
	virtual ~OptionsInstanceManager();
protected:
	KviPointerList<OptionsWidgetInstanceEntry> * m_pInstanceTree;
public:
	KviPointerList<OptionsWidgetInstanceEntry> * instanceEntryTree(){ return m_pInstanceTree; };
	KviOptionsWidget * getInstance(OptionsWidgetInstanceEntry * pEntry, QWidget * pPar);
	OptionsWidgetInstanceEntry * findInstanceEntry(const char * pcName);
	void cleanup(KviModule *);
protected:
	OptionsWidgetInstanceEntry * findInstanceEntry(const char * pcName, KviPointerList<OptionsWidgetInstanceEntry> * pList);
	OptionsWidgetInstanceEntry * findInstanceEntry(const QObject * pObj, KviPointerList<OptionsWidgetInstanceEntry> * pList);
	void deleteInstanceTree(KviPointerList<OptionsWidgetInstanceEntry> * pList);
protected slots:
	void widgetDestroyed();
};

#endif //__OPTIONS_INSTANCES_H__

EOF
######################################################################################################

CLASS_LIST=`grep -h "[ ]*\:[ ]*public[ ]*KviOptionsWidget" OptionsWidget_*.h | sed -e 's/[ ]*\:[ ]*public[ ]*KviOptionsWidget'//g | sed -e 's/[ 	]*class[ ]*//g'`


TARGET="OptionsInstanceManager.cpp"

echo "" > $TARGET

#################################################################################
cat >> OptionsInstanceManager.cpp <<EOF

//=============================================================================
//
//   File : OptionsInstanceManager.cpp
//
//   This file is part of the KVirc IRC client distribution
//   Copyright (C) 2001-2008 Szymon Stefanek (stefanek@tin.it)
//
//   This program is FREE software. You can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your option) any later version.
//
//   This program is distributed in the HOPE that it will be USEFUL,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program. If not, write to the Free Software Foundation,
//   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
//=============================================================================

//
// Instance creation routines for the KVIrc options module
// DO NOT EDIT THIS FILE!! ALL CHANGES WILL BE LOST!!
// This file is automatically generated by mkcreateinstanceproc.sh
// so any change should go in that script
//

EOF
#################################################################################


for afile in OptionsWidget_*.h; do
	echo "#include \"$afile\"" >> $TARGET
done

#################################################################################
cat >> OptionsInstanceManager.cpp <<EOF

#include "KviLocale.h"
#include "OptionsInstanceManager.h"

int g_iOptionWidgetInstances = 0;

EOF
#################################################################################


for aclass in $CLASS_LIST; do
	echo -n "KviOptionsWidget * class$aclass" >> $TARGET
		echo "_createInstanceProc(QWidget * parent)" >> $TARGET
	echo "{" >> $TARGET
	echo "	return new $aclass(parent);" >> $TARGET
	echo "}" >> $TARGET
	echo "" >> $TARGET
done

#################################################################################
cat >> OptionsInstanceManager.cpp <<EOF


EOF
#################################################################################

for aclass in $CLASS_LIST; do
	echo "static const char * g_szName_$aclass = KVI_OPTIONS_WIDGET_NAME_$aclass;" >> $TARGET
	echo "static const char * g_szClassName_$aclass = \"$aclass\";" >> $TARGET
done

#################################################################################
cat >> OptionsInstanceManager.cpp <<EOF

OptionsInstanceManager::OptionsInstanceManager()
: QObject(0)
{

	//qDebug("Instantiating");
	// Create the global widget dict : case sensitive, do not copy keys
	m_pInstanceTree = new KviPointerList<OptionsWidgetInstanceEntry>;
	m_pInstanceTree->setAutoDelete(true);

EOF
#################################################################################

CLASSDIR=".classtmp"

mkdir -p $CLASSDIR

rm -f $CLASSDIR/*

for aclass in $CLASS_LIST; do
	PARENT=`grep -h "KVI_OPTIONS_WIDGET_PARENT_$aclass " OptionsWidget_*.h | sed -e 's/#define//g' | sed -e s/KVI_OPTIONS_WIDGET_PARENT_$aclass//g | sed -e 's/[ ]*//g'`
	if [ -z "$PARENT" ]; then
		PARENT="NOPARENT"
	fi
	echo $aclass >> $CLASSDIR/$PARENT
done

SPACES=""

printclass()
{
	echo "" >> $TARGET
	found=
	for anum in $DECLARED; do
		if [ "$anum" = "$1" ]; then
			found="yes"
			break
		fi
	done
	if [ -z "$found" ]; then
		echo "$3	OptionsWidgetInstanceEntry * e$1;" >> $TARGET
		DECLARED="$DECLARED $1"
	fi
	echo "$3	e$1 = new OptionsWidgetInstanceEntry;"  >> $TARGET
	echo -n "$3	e$1->createProc = &class$2"  >> $TARGET
		echo "_createInstanceProc;"  >> $TARGET
	echo "$3	e$1->pWidget = 0;" >> $TARGET
	echo "$3	e$1->szClassName = g_szClassName_$2;" >> $TARGET
	echo "$3	e$1->eIcon = KVI_OPTIONS_WIDGET_ICON_$2;" >> $TARGET

	echo "$3	#ifdef KVI_OPTIONS_WIDGET_PRIORITY_$2" >> $TARGET
	echo "$3	e$1->iPriority = KVI_OPTIONS_WIDGET_PRIORITY_$2;" >> $TARGET
	echo "$3	#else" >> $TARGET
	echo "$3	e$1->iPriority = 0;" >> $TARGET
	echo "$3	#endif" >> $TARGET

	echo "$3	#ifdef KVI_OPTIONS_WIDGET_KEYWORDS_$2" >> $TARGET
	echo "$3	e$1->szKeywordsNoLocale = KVI_OPTIONS_WIDGET_NAME_$2 \",\" KVI_OPTIONS_WIDGET_KEYWORDS_$2;" >> $TARGET
	echo "$3	#else" >> $TARGET
	echo "$3	e$1->szKeywordsNoLocale = KVI_OPTIONS_WIDGET_NAME_$2;" >> $TARGET
	echo "$3	#endif" >> $TARGET
	echo "$3	e$1->szKeywords = __tr2qs_ctx_no_xgettext(e$1->szKeywordsNoLocale.toUtf8().data(),\"options\");" >> $TARGET

	echo "$3	#ifdef KVI_OPTIONS_WIDGET_GROUP_$2" >> $TARGET
	echo "$3	e$1->szGroup = KVI_OPTIONS_WIDGET_GROUP_$2;" >> $TARGET
	echo "$3	#else" >> $TARGET
	echo "$3	e$1->szGroup = \"general\";" >> $TARGET
	echo "$3	#endif" >> $TARGET

	echo "$3	#ifdef KVI_OPTIONS_WIDGET_CONTAINER_$2" >> $TARGET
	echo "$3	e$1->bIsContainer = KVI_OPTIONS_WIDGET_CONTAINER_$2;" >> $TARGET
	echo "$3	#else" >> $TARGET
	echo "$3	e$1->bIsContainer = false;" >> $TARGET
	echo "$3	#endif" >> $TARGET

	echo "$3	#ifdef KVI_OPTIONS_WIDGET_NOTCONTAINED_$2" >> $TARGET
	echo "$3	e$1->bIsNotContained = KVI_OPTIONS_WIDGET_NOTCONTAINED_$2;" >> $TARGET
	echo "$3	#else" >> $TARGET
	echo "$3	e$1->bIsNotContained = false;" >> $TARGET
	echo "$3	#endif" >> $TARGET

	echo "$3	e$1->szNameNoLocale = g_szName_$2;" >> $TARGET
	echo "$3	e$1->szName = __tr2qs_ctx_no_xgettext(g_szName_$2,\"options\");" >> $TARGET

	if [ "$1" = "0" ]; then
		echo "$3	m_pInstanceTree->append(e$1);" >> $TARGET
	else
		VAL=`expr $1 - 1`
		echo "$3	e$VAL->pChildList->append(e$1);" >> $TARGET
	fi
}

addchildren()
{
	if [ -f "$CLASSDIR/$2" ]; then
		LEVEL=`cat $CLASSDIR/$2`
		for achild in $LEVEL; do
			printclass $1 $achild "$3"
			if [ -f "$CLASSDIR/$achild" ]; then
				echo "" >> $TARGET
				echo "$3	e$1->pChildList = new KviPointerList<OptionsWidgetInstanceEntry>;" >> $TARGET
				echo "$3	e$1->pChildList->setAutoDelete(true);" >> $TARGET
				NEXTLEVEL=`expr $1 + 1`
				addchildren $NEXTLEVEL $achild "$3	"
			else
				echo "$3	e$1->pChildList = 0;" >> $TARGET
			fi
		done
	fi
}

addchildren 0 NOPARENT ""


rm -fr $CLASSDIR

#################################################################################
cat >> $TARGET <<EOF

}

void OptionsInstanceManager::deleteInstanceTree(KviPointerList<OptionsWidgetInstanceEntry> * pList)
{
	if(pList)
	{
		for(OptionsWidgetInstanceEntry * pEntry = pList->first(); pEntry; pEntry = pList->next())
		{
			if(pEntry->pWidget)
			{
				if(pEntry->pWidget->parent()->inherits("OptionsWidgetContainer"))
				{
					disconnect(pEntry->pWidget,SIGNAL(destroyed()),this,SLOT(widgetDestroyed()));
					delete pEntry->pWidget->parent();
					pEntry->pWidget =  0;
				} else {
					qDebug("Ops...i have deleted the options dialog ?");
				}
			} //else qDebug("Class %s has no widget",e->szName);
			if(pEntry->pChildList)
				deleteInstanceTree(pEntry->pChildList);
		}
		delete pList;
	}
}

OptionsInstanceManager::~OptionsInstanceManager()
{
	if(m_pInstanceTree)
		qDebug("Ops...OptionsInstanceManager::cleanup() not called ?");
}

void OptionsInstanceManager::cleanup(KviModule *)
{
	deleteInstanceTree(m_pInstanceTree);
	m_pInstanceTree = 0;
}

void OptionsInstanceManager::widgetDestroyed()
{
	OptionsWidgetInstanceEntry * pEntry = findInstanceEntry(sender(),m_pInstanceTree);
	if(pEntry)
		pEntry->pWidget = 0;
	if(g_iOptionWidgetInstances > 0)
		g_iOptionWidgetInstances--;

}

KviOptionsWidget * OptionsInstanceManager::getInstance(OptionsWidgetInstanceEntry * pEntry, QWidget * pPar)
{
	if(!pEntry)
		return NULL;

#if 0
	if(pEntry->pWidget)
	{
		if(pEntry->pWidget->parent() != pPar)
		{
			QWidget * pOldPar = (QWidget *)pEntry->pWidget->parent();
			pEntry->pWidget->setParent(pPar);
			pOldPar->deleteLater();
			pEntry->pWidget = 0;
		}
	}
#endif

	if(!(pEntry->pWidget))
	{
		pEntry->pWidget = pEntry->createProc(pPar);
		g_iOptionWidgetInstances++;
		connect(pEntry->pWidget,SIGNAL(destroyed()),this,SLOT(widgetDestroyed()));
	}

	if(pEntry->pWidget->parent() != pPar)
	{
		QWidget * pOldPar = (QWidget *)pEntry->pWidget->parent();
		pEntry->pWidget->setParent(pPar); //reparent(pPar,QPoint(0,0));
		if(pOldPar->inherits("OptionsWidgetContainer"))
			delete pOldPar;
		// else it's very likely a QStackedWidget, child of a KviOptionsWidget: don't delete
	}

	if(pEntry->bIsContainer)
	{
		// need to create the container structure!
		pEntry->pWidget->createTabbedPage();
		if(pEntry->pChildList)
		{
			KviPointerList<OptionsWidgetInstanceEntry> tmpList;
			tmpList.setAutoDelete(false);

			for(OptionsWidgetInstanceEntry * pEntry2 = pEntry->pChildList->first(); pEntry2; pEntry2 = pEntry->pChildList->next())
			{
				// add only non containers and widgets not explicitly marked as noncontained
				if((!pEntry2->bIsContainer) && (!pEntry2->bIsNotContained))
				{
					OptionsWidgetInstanceEntry * pEntry3 = tmpList.first();
					int iIdx = 0;
					while(pEntry3)
					{
						if(pEntry3->iPriority >= pEntry2->iPriority)
							break;
						iIdx++;
						pEntry3 = tmpList.next();
					}
					tmpList.insert(iIdx,pEntry2);
				}
			}

			for(OptionsWidgetInstanceEntry * pEntry4 = tmpList.last(); pEntry4; pEntry4 = tmpList.prev())
			{
				KviOptionsWidget * pOpt = getInstance(pEntry4,pEntry->pWidget->tabWidget());
				pEntry->pWidget->addOptionsWidget(pEntry4->szName,*(g_pIconManager->getSmallIcon(pEntry4->eIcon)),pOpt);
			}
		}
	}
	return pEntry->pWidget;
}

OptionsWidgetInstanceEntry * OptionsInstanceManager::findInstanceEntry(const QObject * pObj, KviPointerList<OptionsWidgetInstanceEntry> * pList)
{
	if(pList)
	{
		for(OptionsWidgetInstanceEntry * pEntry = pList->first(); pEntry; pEntry = pList->next())
		{
			if(pObj == pEntry->pWidget)
				return pEntry;
			if(pEntry->pChildList)
			{
				OptionsWidgetInstanceEntry * pEntry2 = findInstanceEntry(pObj,pEntry->pChildList);
				if(pEntry2)
					return pEntry2;
			}
		}
	}
	return 0;
}

OptionsWidgetInstanceEntry * OptionsInstanceManager::findInstanceEntry(const char * pcName, KviPointerList<OptionsWidgetInstanceEntry> * pList)
{
	if(pList)
	{
		for(OptionsWidgetInstanceEntry * pEntry = pList->first(); pEntry; pEntry = pList->next())
		{
			if(kvi_strEqualCI(pEntry->szClassName,pcName))
				return pEntry;
			if(pEntry->pChildList)
			{
				OptionsWidgetInstanceEntry * pEntry2 = findInstanceEntry(pcName,pEntry->pChildList);
				if(pEntry2)
					return pEntry2;
			}
		}
	}
	return 0;
}

OptionsWidgetInstanceEntry * OptionsInstanceManager::findInstanceEntry(const char * pcName)
{
	return findInstanceEntry(pcName,m_pInstanceTree);
}

EOF
#################################################################################
