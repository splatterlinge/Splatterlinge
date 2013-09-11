/*
 * Copyright (C) 2013
 * Branimir Djordjevic <branimir.djordjevic@gmail.com>
 * Tobias Himmer <provisorisch@online.de>
 * Michael Wydler <michael.wydler@gmail.com>
 * Karl-Heinz Zimmermann <karlzimmermann3787@gmail.com>
 *
 * This file is part of Splatterlinge.
 *
 * Splatterlinge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Splatterlinge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Splatterlinge. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STARTMENUWINDOW_HPP
#define STARTMENUWINDOW_HPP

#include <QWidget>

class Scene;
class OptionWindow;
class QPushButton;

class StartMenuWindow : public QWidget
{
	Q_OBJECT
public:
	StartMenuWindow( Scene * scene, OptionWindow * options, QWidget * parent = 0 );
	~StartMenuWindow();

private:
	static const int WIDTH = 740;
	static const int HEIGHT = 550;
	static const int BUTTON_WIDTH = 350;
	static const int BUTTON_HEIGHT = 150;
	QPushButton * mNewGame;
	QPushButton * mOptions;
	QPushButton * mEnd;
	Scene * mScene;

	OptionWindow * mOptionWindow;

	void initMenu();

private slots:
	void handleNewGameButton();
	void handleOptionsButton();
	void handleEndGameButton();

};

#endif // STARTMENUWINDOW_HPP
