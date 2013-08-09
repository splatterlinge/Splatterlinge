/*
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

#ifndef SCENE_AKEYLISTENER_INCLUDED
#define SCENE_AKEYLISTENER_INCLUDED


#include <QKeyEvent>


/// Abstract key listener
/**
 * Register at Scene to receive keyboard events.
 */
class AKeyListener
{
public:
	virtual void keyPressEvent( QKeyEvent * event ) = 0;
	virtual void keyReleaseEvent( QKeyEvent * event ) = 0;
};


#endif
