/***************************************************************************
    File                 : PenStyleBox.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2008 Ion Vasilief
    Email (use @ for *)  : ion_vasilief*yahoo.fr
    Description          : Pen style combo box
                           
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/
#ifndef PEN_STYLE_BOX_H
#define PEN_STYLE_BOX_H

#include <QComboBox>

//! A modified QComboBox allowing to choose a Qt::PenStyle.
/**
 * This is a simple hack on top of the QComboBox class.
 */
class PenStyleBox : public QComboBox
{
  Q_OBJECT
	
public:
		//! Constructor.
		/**
		 * \param parent parent widget (only affects placement of the widget)
		 */
		PenStyleBox(QWidget *parent = 0);
  		void setStyle(const Qt::PenStyle& style);
  		Qt::PenStyle style() const;

  		static int styleIndex(const Qt::PenStyle& style);
  		static Qt::PenStyle penStyle(int index);

private:
  		static const Qt::PenStyle patterns[];
};

#endif
