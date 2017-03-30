/*
    This file is part of Dirt, the Dartmouth introductory ray tracer, used in
    Dartmouth's COSC 77/177 Computer Graphics course.

    Copyright (c) 2016 by Wojciech Jarosz

    Dirt is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Dirt is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "common.h"
#include "color.h"

//! A floating-point RGB image
struct Image3f
{
    //! Default Constructor (empty image)
    Image3f();
    //! Size Constructor (sets width and height)
    Image3f(int w, int h);

    //! Size Constructor with initialization (sets width and height and initialize pixels)
    Image3f(int w, int h, const Color3f &v);

    //! load an image from file
    bool load(const std::string & filename);
    //! save an image to file
    bool save(const std::string & filename, float gain = 1.0f);

    // image width
    int width() const               {return m_width;}
    // image height
    int height() const              {return m_height;}

    void resize(int w, int h);

    //! element access
    Color3f & operator()(int i, int j)             {return m_data[j*m_width+i];}
    //! element access
    const Color3f & operator()(int i, int j) const {return m_data[j*m_width+i];}

    //! raw data access
    Color3f* data()                 {return m_data.data();}
    //! raw data access
    const Color3f* data() const     {return m_data.data();}

private:
    int m_width, m_height;
    vector<Color3f> m_data;
};
