/*
 * Copyright (C) 2019 Damir Porobic <damir.porobic@gmx.com>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "ApplicationSettings.h"

ApplicationSettings::ApplicationSettings(const QSharedPointer<IConfig> &config) :
	mConfig(config),
	mAutoCopyToClipboardNewCapturesCheckbox(new QCheckBox(this)),
	mRememberPositionCheckbox(new QCheckBox(this)),
	mCaptureOnStartupCheckbox(new QCheckBox(this)),
	mUseTabsCheckbox(new QCheckBox(this)),
	mAutoHideTabsCheckbox(new QCheckBox(this)),
	mUseSingleInstanceCheckBox(new QCheckBox(this)),
	mAutoHideDocksCheckBox(new QCheckBox(this)),
	mAutoResizeToContentCheckBox(new QCheckBox(this)),
	mEnableDebugging(new QCheckBox(this)),
	mApplicationStyleLabel(new QLabel(this)),
	mResizeToContentDelayLabel(new QLabel(this)),
	mApplicationStyleCombobox(new QComboBox(this)),
	mResizeToContentDelaySpinBox(new CustomSpinBox(0, 1000, this)),
	mLayout(new QGridLayout)
{
	Q_ASSERT(mConfig != nullptr);

	initGui();

	loadConfig();
}

void ApplicationSettings::initGui()
{
	mAutoCopyToClipboardNewCapturesCheckbox->setText(tr("Automatically copy new captures to clipboard"));
	mRememberPositionCheckbox->setText(tr("Remember Main Window position on move and load on startup"));
	mCaptureOnStartupCheckbox->setText(tr("Capture screenshot at startup with default mode"));

	mUseTabsCheckbox->setText(tr("Use Tabs"));
	mUseTabsCheckbox->setToolTip(tr("Change requires restart."));

	mAutoHideTabsCheckbox->setText(tr("Auto hide Tabs"));
	mAutoHideTabsCheckbox->setToolTip(tr("Hide Tabbar when only one Tab is used."));

	mUseSingleInstanceCheckBox->setText(tr("Run ksnip as single instance"));
	mUseSingleInstanceCheckBox->setToolTip(tr("Enabling this option will allow only one ksnip instance to run,\n"
										         "all other instances started after the first will pass its\n"
				                                 "arguments to the first and close. Changing this option requires\n"
									             "a new start of all instances."));

	mAutoHideDocksCheckBox->setText(tr("Auto hide Docks"));
	mAutoHideDocksCheckBox->setToolTip(tr("On startup hide Toolbar and Annotation Settings.\n"
									   		"Docks visibility can be toggled with the Tab Key."));

	mAutoResizeToContentCheckBox->setText(tr("Auto resize to content"));
	mAutoResizeToContentCheckBox->setToolTip(tr("Automatically resize Main Window to fit content image."));

	mEnableDebugging->setText(tr("Enable Debugging"));
	mEnableDebugging->setToolTip(tr("Enables debug output written to the console.\n"
									   "Change requires ksnip restart to take effect."));

	mResizeToContentDelayLabel->setText(tr("Resize to content delay") + QLatin1String(":"));
	mResizeToContentDelayLabel->setToolTip(tr("Resizing to content is delay to allow the Window Manager to receive\n"
										   		 "the new content. In case that the Main Windows is not adjusted correctly\n"
												 "to the new content, increasing this delay might improve the behavior."));

	mResizeToContentDelaySpinBox->setSuffix(QLatin1String("ms"));
	mResizeToContentDelaySpinBox->setToolTip(mResizeToContentDelayLabel->toolTip());
	mResizeToContentDelaySpinBox->setSingleStep(10);

	connect(mUseTabsCheckbox, &QCheckBox::stateChanged, this, &ApplicationSettings::useTabsChanged);

	mApplicationStyleLabel->setText(tr("Application Style") + QLatin1String(":"));
	mApplicationStyleLabel->setToolTip(tr("Sets the application style which defines the look and feel of the GUI.\n"
	                                      "Change requires ksnip restart to take effect."));

	mApplicationStyleCombobox->addItems(QStyleFactory::keys());
	mApplicationStyleCombobox->setToolTip(mApplicationStyleLabel->toolTip());
	mApplicationStyleCombobox->setFixedWidth(100);

	mLayout->setAlignment(Qt::AlignTop);
	mLayout->setColumnMinimumWidth(0, 10);
	mLayout->addWidget(mAutoCopyToClipboardNewCapturesCheckbox, 0, 0, 1, 4);
	mLayout->addWidget(mRememberPositionCheckbox, 1, 0, 1, 4);
	mLayout->addWidget(mCaptureOnStartupCheckbox, 2, 0, 1, 4);
	mLayout->addWidget(mUseTabsCheckbox, 3, 0, 1, 4);
	mLayout->addWidget(mAutoHideTabsCheckbox, 4, 1, 1, 3);
	mLayout->addWidget(mUseSingleInstanceCheckBox, 5, 0, 1, 4);
	mLayout->addWidget(mAutoHideDocksCheckBox, 6, 0, 1, 4);
	mLayout->addWidget(mAutoResizeToContentCheckBox, 7, 0, 1, 4);
	mLayout->addWidget(mEnableDebugging, 8, 0, 1, 4);
	mLayout->setRowMinimumHeight(9, 15);
	mLayout->addWidget(mResizeToContentDelayLabel, 10, 0, 1, 2);
	mLayout->addWidget(mResizeToContentDelaySpinBox, 10, 2, Qt::AlignLeft);
	mLayout->setRowMinimumHeight(11, 15);
	mLayout->addWidget(mApplicationStyleLabel, 12, 0, 1, 2);
	mLayout->addWidget(mApplicationStyleCombobox, 12, 2, Qt::AlignLeft);

	setTitle(tr("Application Settings"));
	setLayout(mLayout);
}

void ApplicationSettings::loadConfig()
{
	mAutoCopyToClipboardNewCapturesCheckbox->setChecked(mConfig->autoCopyToClipboardNewCaptures());
	mRememberPositionCheckbox->setChecked(mConfig->rememberPosition());
	mCaptureOnStartupCheckbox->setChecked(mConfig->captureOnStartup());
	mUseTabsCheckbox->setChecked(mConfig->useTabs());
	mAutoHideTabsCheckbox->setChecked(mConfig->autoHideTabs());
	mUseSingleInstanceCheckBox->setChecked(mConfig->useSingleInstance());
	mAutoHideDocksCheckBox->setChecked(mConfig->autoHideDocks());
	mAutoResizeToContentCheckBox->setChecked(mConfig->autoResizeToContent());
	mEnableDebugging->setChecked(mConfig->isDebugEnabled());
	mResizeToContentDelaySpinBox->setValue(mConfig->resizeToContentDelay());
	mApplicationStyleCombobox->setCurrentText(mConfig->applicationStyle());

	useTabsChanged();
}

void ApplicationSettings::saveSettings()
{
	mConfig->setAutoCopyToClipboardNewCaptures(mAutoCopyToClipboardNewCapturesCheckbox->isChecked());
	mConfig->setRememberPosition(mRememberPositionCheckbox->isChecked());
	mConfig->setCaptureOnStartup(mCaptureOnStartupCheckbox->isChecked());
	mConfig->setUseSingleInstance(mUseSingleInstanceCheckBox->isChecked());
	mConfig->setUseTabs(mUseTabsCheckbox->isChecked());
	mConfig->setAutoHideTabs(mAutoHideTabsCheckbox->isChecked());
	mConfig->setAutoHideDocks(mAutoHideDocksCheckBox->isChecked());
	mConfig->setAutoResizeToContent(mAutoResizeToContentCheckBox->isChecked());
	mConfig->setIsDebugEnabled(mEnableDebugging->isChecked());
	mConfig->setResizeToContentDelay(mResizeToContentDelaySpinBox->value());
	mConfig->setApplicationStyle(mApplicationStyleCombobox->currentText());
}

void ApplicationSettings::useTabsChanged()
{
	mAutoHideTabsCheckbox->setEnabled(mUseTabsCheckbox->isChecked());
}
