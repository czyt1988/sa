/***************************************************************************
    File                 : PatternBox.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Tomomasa Ohkubo, Ion Vasilief
    Email (use @ for *)  : ion_vasilief*yahoo.fr
    Description          : Pattern combox box
                           
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
#include "PatternBox.h"

#include <algorithm>
#include <qpixmap.h>
#include <qpainter.h>

const Qt::BrushStyle PatternBox::patterns[] = {
  Qt::SolidPattern,
  Qt::HorPattern,
  Qt::VerPattern,
  Qt::CrossPattern,
  Qt::BDiagPattern,
  Qt::FDiagPattern,
  Qt::DiagCrossPattern,
  Qt::Dense1Pattern,
  Qt::Dense2Pattern,
  Qt::Dense3Pattern,
  Qt::Dense4Pattern,
  Qt::Dense5Pattern,
  Qt::Dense6Pattern,
  Qt::Dense7Pattern,
  Qt::NoBrush
};

PatternBox::PatternBox(QWidget *parent) : QComboBox(parent)
{
  init();
}

void PatternBox::init()
{

  QPixmap icon = QPixmap(28, 14);
  icon.fill ( QColor (Qt::white) );
  const QRect r = QRect(0, 0, 27, 13);
  QPainter p(&icon);
  QBrush br = QBrush(QColor(Qt::darkGray), Qt::SolidPattern);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "Solid" ) );

  br = QBrush(QColor(Qt::darkGray), Qt::HorPattern);
  p.eraseRect(r);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "Horizontal" ) );

  br = QBrush(QColor(Qt::darkGray), Qt::VerPattern);
  p.eraseRect(r);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "Vertical" ) );

  br = QBrush(QColor(Qt::darkGray), Qt::CrossPattern);
  p.eraseRect(r);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "Cross" ) );

  br = QBrush(QColor(Qt::darkGray), Qt::BDiagPattern);
  p.eraseRect(r);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "BDiagonal" ) );

  br = QBrush(QColor(Qt::darkGray), Qt::FDiagPattern);
  p.eraseRect(r);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "FDiagonal" ) );

  br = QBrush(QColor(Qt::darkGray), Qt::DiagCrossPattern);
  p.eraseRect(r);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "DiagCross" ) );

  br = QBrush(QColor(Qt::darkGray), Qt::Dense1Pattern);
  p.eraseRect(r);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "Dense1" ) );

  br = QBrush(QColor(Qt::darkGray), Qt::Dense2Pattern);
  p.eraseRect(r);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "Dense2" ) );

  br = QBrush(QColor(Qt::darkGray), Qt::Dense3Pattern);
  p.eraseRect(r);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "Dense3" ) );

  br = QBrush(QColor(Qt::darkGray), Qt::Dense4Pattern);
  p.eraseRect(r);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "Dense4" ) );

  br = QBrush(QColor(Qt::darkGray), Qt::Dense5Pattern);
  p.eraseRect(r);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "Dense5" ) );

  br = QBrush(QColor(Qt::darkGray), Qt::Dense6Pattern);
  p.eraseRect(r);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "Dense6" ) );

  br = QBrush(QColor(Qt::darkGray), Qt::Dense7Pattern);
  p.eraseRect(r);
  p.fillRect(r, br);
  p.drawRect(r);
  this->addItem(icon, tr( "Dense7" ) );

  p.eraseRect(r);
  p.drawRect(r);
  this->addItem(icon, tr( "None" ) );
  p.end();
}

void PatternBox::setPattern(const Qt::BrushStyle& style)
{
  const Qt::BrushStyle*ite = std::find(patterns, patterns + sizeof(patterns), style);
  if (ite == patterns + sizeof(patterns))
    this->setCurrentIndex(14); // default pattern is none.
  else
    this->setCurrentIndex(ite - patterns);
}

Qt::BrushStyle PatternBox::brushStyle(int index)
{
  if (index < (int)sizeof(patterns))
    return patterns[index];
  else
    return Qt::NoBrush; // default patterns is none. 
}

Qt::BrushStyle PatternBox::getSelectedPattern() const
{
  size_t i = this->currentIndex();
  if (i < sizeof(patterns))
    return patterns[i];
  else
    return Qt::NoBrush; // default patterns is none. 
}

int PatternBox::patternIndex(const Qt::BrushStyle& style)
{
  const Qt::BrushStyle*ite = std::find(patterns, patterns + sizeof(patterns), style);
  if (ite == patterns + sizeof(patterns))
    return 14; // default pattern is none.
  else
    return (ite - patterns);
}
