/* $Id: vidwin.cpp 4060 2012-04-17 09:55:30Z ming $ */
/*
 * Copyright (C) 2011 Teluu Inc. (http://www.teluu.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "vidwin.h"
#include <QEvent>

#define THIS_FILE	"vidwin.cpp"
#define TRACE_(...)	PJ_LOG(4,(THIS_FILE, __VA_ARGS__))

VidWin::VidWin(const pjmedia_vid_dev_hwnd *hwnd_,
	       QWidget* parent,
	       Qt::WindowFlags f) :
    QWidget(parent, f), orig_parent(NULL),
    size_hint(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX)
{
    setAttribute(Qt::WA_NativeWindow);

    /* Make this widget a bit "lighter" */
    setAttribute(Qt::WA_UpdatesDisabled);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setUpdatesEnabled(false);

    pj_bzero(&hwnd, sizeof(hwnd));
    if (hwnd_) {
	hwnd = *hwnd_;
    }
}


VidWin::~VidWin()
{
    show_sdl(false);
    detach();
}

void VidWin::putIntoLayout(QBoxLayout *box)
{
    box->addWidget(this, 1);
    show();
    activateWindow();
}

bool VidWin::event(QEvent *e)
{
    switch(e->type()) {

    case QEvent::Resize:
	set_size();
	break;

    case QEvent::ParentChange:
	get_size();
	if (0) {
	    QRect qr = rect();
	    if (qr.width() > size_hint.width())
		size_hint.setWidth(qr.width());
	    if (qr.height() > size_hint.height())
		size_hint.setWidth(qr.height());
	}
	setFixedSize(size_hint);
	attach();
	break;

    case QEvent::Show:
	show_sdl(true);
	// revert to default size hint, make it resizable
	setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
	break;

    case QEvent::Hide:
	show_sdl(false);
	break;

    default:
	break;
    }

    return QWidget::event(e);
}

//#include <QX11Info>
//#include <stdio.h>

//#define GET_DISPLAY()	QX11Info::display()
void VidWin::attach()
{

}


void VidWin::detach()
{
}


void VidWin::set_size()
{

}

void VidWin::get_size()
{

}

void VidWin::show_sdl(bool visible)
{

}


