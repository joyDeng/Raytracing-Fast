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

#include <string>

//! Helper object to display and update a progress bar in the terminal.
/*!
    Example usage:
        {
            Util::Progress p1("Solving", 10);
            for (int i = 0; i < 10; ++i, ++p1)
            {
                // do something
            }
        } // end progress p1
*/
class Progress
{
public:
    Progress(const std::string & str, int max);
    virtual ~Progress();

    void setNumSteps(int);
    void step(int steps = 1);
    void done();

    int progress() const
    {
        return m_curr;
    }

    Progress & operator++ ()          {step(); return *this; }
    Progress & operator+=(int steps) {step(steps); return *this; }

private:

    std::string m_str;
    int m_curr, m_max;
    float m_frac;
    bool m_done;
};

void consoleProgress(float amountDone, const char message[]);
void consoleStartProgress(const char message[]);
void consoleEndProgress(const char message[]);
void consoleStartBusy(const char message[]);
void consoleBusy();
void consoleEndBusy();
