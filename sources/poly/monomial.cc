//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Instantiation of monomial classes
//
// This file is part of Gambit
// Copyright (c) 2002, The Gambit Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#include "monomial.imp"
#include "base/glist.imp"
#include "math/double.h"
#include "base/garray.imp"
#include "base/gblock.imp"
#include "math/gnumber.h"

template class gbtMonomial<int>;
template gbtOutput& operator << (gbtOutput& output, const gbtMonomial<int>& x);

template class gbtMonomial<double>;
template gbtOutput& operator << (gbtOutput& output, const gbtMonomial<double>& x);

template class gbtMonomial<gbtRational>;
template gbtOutput& operator << (gbtOutput& output, const gbtMonomial<gbtRational>& x);

template class gbtMonomial<gbtNumber>;
template gbtOutput& operator << (gbtOutput& output, const gbtMonomial<gbtNumber>& x);

template class gbtMonomial<gbtDouble>;
template gbtOutput& operator << (gbtOutput& output, const gbtMonomial<gbtDouble>& x);

template class gbtList< gbtMonomial<int> >;
template class gbtList< gbtMonomial<double> >;
template class gbtList< gbtMonomial<gbtRational> >;
template class gbtList< gbtMonomial<gbtNumber> >;

template class gbtList< gbtMonomial<gbtDouble> >;
