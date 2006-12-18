//
//   File : optw_topiclabel.cpp
//   Creation date : Mon Sep 24 02:44:55 CEST 2001 by Krzysztof Godlewski
//
//   This file is part of the KVirc irc client distribution
//   Copyright (C) 2001 Szymon Stefanek (pragma at kvirc dot net)
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

#include "kvi_options.h"
#include "kvi_locale.h"
#include "optw_topiclabel.h"

KviTopicLabelLookOptionsWidget::KviTopicLabelLookOptionsWidget(QWidget *p):KviOptionsWidget(p, "topic_label_look_options_widget")
{
	createLayout(4, 1);

	addFontSelector(0, 0, 0, 0, __tr2qs_ctx("Font","options"), KviOption_fontLabel);
	addColorSelector(0, 1, 0, 1, __tr2qs_ctx("Foreground color","options"), KviOption_colorLabelForeground);
	addColorSelector(0, 2, 0, 2, __tr2qs_ctx("Background color","options"), KviOption_colorLabelBackground);
	addPixmapSelector(0, 3, 0, 3, __tr2qs_ctx("Background image","options"), KviOption_pixmapLabelBackground);
	
	layout()->setRowStretch(3, 1);
}

KviTopicLabelLookOptionsWidget::~KviTopicLabelLookOptionsWidget()
{
}

#include "m_optw_topiclabel.moc"
