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

#ifndef UTILITY_BILBOARD_INCLUDED
#define UTILITY_BILBOARD_INCLUDED

#include <GLWidget.hpp>


/// Screen-align geometry
/**
 *
 */
namespace Bilboard
{
	inline QMatrix4x4 matrix( const QMatrix4x4 & modelView )
		{ QMatrix4x4 bilboard; bilboard.setColumn( 3, modelView.column(3) ); return bilboard; }

	inline void begin( const QMatrix4x4 & modelView )
		{ glPushMatrix(); glLoadMatrix( matrix( modelView ) ); }

	inline void end()
		{ glPopMatrix(); }
}


#endif
