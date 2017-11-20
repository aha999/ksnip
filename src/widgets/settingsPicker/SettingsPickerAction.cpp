/*
 * Copyright (C) 2017 Damir Porobic <https://github.com/damirporobic>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "SettingsPickerAction.h"

SettingsPickerAction::SettingsPickerAction(QWidget* parent, int colons) :
    QAction("Settings", parent),
    mPopup(nullptr)
{
    // Create color grid popup and connect to it.
    mPopup = new SettingsPickerPopup(colons);

    connect(mPopup, &SettingsPickerPopup::colorChanged,
            this, &SettingsPickerAction::updateColor);
    connect(mPopup, &SettingsPickerPopup::fillChanged,
            this, &SettingsPickerAction::updateFill);
    connect(mPopup, &SettingsPickerPopup::sizeChanged,
            this, &SettingsPickerAction::updateSize);
    connect(mPopup, &SettingsPickerPopup::hid,
            this, &SettingsPickerAction::popupClosed);

    connect(mPopup, &SettingsPickerPopup::hoverLeave,
        this, &SettingsPickerAction::popupClosed);

    // Connect this push button's pressed() signal.
    connect(this, &SettingsPickerAction::hovered,
            this, &SettingsPickerAction::buttonPressed);
}

void SettingsPickerAction::buttonPressed()
{
    // Don't try to show popup when it's empty
    if (mPopup->isEmpty()) {
        return;
    }

    const auto desktop = QApplication::desktop()->geometry();

    // Make sure the popup is inside the desktop.
    auto pos = (associatedWidgets().first()->geometry()).topRight();
    if (pos.x() < desktop.left()) {
        pos.setX(desktop.left());
    }
    if (pos.y() < desktop.top()) {
        pos.setY(desktop.top());
    }

    if ((pos.x() + mPopup->sizeHint().width()) > desktop.width()) {
        pos.setX(desktop.width() - mPopup->sizeHint().width());
    }
    if ((pos.y() + mPopup->sizeHint().height()) > desktop.bottom()) {
        pos.setY(desktop.bottom() - mPopup->sizeHint().height());
    }
    mPopup->move(pos);

    // Remove focus from this widget, preventing the focus rect
    // from showing when the popup is shown. Order an update to
    // make sure the focus rect is cleared.

//     clearFocus();
//     update();
//     setDown(true);

    // Allow keyboard navigation as soon as the popup shows.
//     mPopup->setFocus();

    // Execute the popup. The popup will enter the event loop.
    mPopup->show();
}

void SettingsPickerAction::popupClosed()
{
    mPopup->hide();
//     setDown(false);
//     setFocus();
}

QColor SettingsPickerAction::color() const
{
    return mPopup->color();
}

void SettingsPickerAction::setColor(const QColor& color)
{
    mPopup->setColor(color);
}

bool SettingsPickerAction::fill() const
{
    return mPopup->fill();
}

void SettingsPickerAction::setFill(bool  fill)
{
    mPopup->setFill(fill);
}

int SettingsPickerAction::size() const
{
    return mPopup->size();
}

void SettingsPickerAction::setSize(int  size)
{
    mPopup->setSize(size);
}

void SettingsPickerAction::addPopupColorGrid(bool  colorDialog,
        bool  fillCheckbox,
        bool  standardColor)
{
    mPopup->addColorGrid(colorDialog, fillCheckbox);
    if (standardColor) {
        insertStandardColor();
    }
}

void SettingsPickerAction::addPopupSizeSlider(int  min, int  max, int  interval)
{
    mPopup->addSizeSlider(min, max, interval);
}

void SettingsPickerAction::insertStandardColor()
{
    mPopup->insertColor(QColor("white"), "White", -1);
    mPopup->insertColor(QColor("black"), "Black", -1);
    mPopup->insertColor(QColor("green"), "Green", -1);
    mPopup->insertColor(QColor("darkGreen"), "Dark green", -1);
    mPopup->insertColor(QColor("blue"), "Blue", -1);
    mPopup->insertColor(QColor("darkBlue"), "Dark blue", -1);
    mPopup->insertColor(QColor("cyan"), "Cyan", -1);
    mPopup->insertColor(QColor("darkCyan"), "Dark cyan", -1);
    mPopup->insertColor(QColor("magenta"), "Magenta", -1);
    mPopup->insertColor(QColor("darkMagenta"), "Dark magenta", -1);
    mPopup->insertColor(QColor("red"), "red", -1);
    mPopup->insertColor(QColor("yellow"), "Yellow", -1);
    mPopup->insertColor(QColor("grey"), "Grey", -1);
}

void SettingsPickerAction::clearPopup()
{
    mPopup->clear();
}

void SettingsPickerAction::updateColor(const QColor& color)
{
    emit colorChanged(color);
}

void SettingsPickerAction::updateFill(bool  fill)
{
    emit fillChanged(fill);
}

void SettingsPickerAction::updateSize(int  size)
{
    emit sizeChanged(size);
}

void SettingsPickerAction::insertColor(const QColor& color, const QString& text, int index)
{
    mPopup->insertColor(color, text, index);
}