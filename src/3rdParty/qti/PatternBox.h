/***************************************************************************
    File                 : PatternBox.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Tomomasa Ohkubo, Ion Vasilief
    Email (use @ for *)  : ion_vasilief*yahoo.fr
    Description          : Pattern combo box
                           
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
#ifndef PATTERNBOX_H
#define PATTERNBOX_H

#include <QComboBox>

//! A modified QComboBox allowing to choose a Qt::BrushStyle style.
/**
 * This is a simple hack on top of the QComboBox class.
 */
class PatternBox : public QComboBox
{
  Q_OBJECT
	
public:
		//! Constructor.
		/**
		 * \param parent parent widget (only affects placement of the widget)
		 */
		PatternBox(QWidget *parent = 0);
		void setPattern(const Qt::BrushStyle& c);
		Qt::BrushStyle getSelectedPattern() const;

		static int patternIndex(const Qt::BrushStyle& style);
		static Qt::BrushStyle brushStyle(int index);

protected:
		void init();

private:
		static const Qt::BrushStyle patterns[];
};

#endif
