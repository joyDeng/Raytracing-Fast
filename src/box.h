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

//! Represents a bounded interval in arbitrary dimensions.
/*!
    TBox is an N-D interval based on Eigen::AlignedBox with additional
    functionality.
*/
template <typename _Scalar, int _AmbientDim>
struct TBox : public Eigen::AlignedBox<_Scalar, _AmbientDim>
{
    typedef Eigen::AlignedBox<_Scalar, _AmbientDim> Base;
    typedef typename Eigen::AlignedBox<_Scalar, _AmbientDim>::VectorType VectorType;

    enum
    {
        Dimension = _AmbientDim
    };

    //! Default constructor initializing a null box.
    TBox() : Base() {}

    template<typename OtherVectorType1, typename OtherVectorType2>
    TBox(const OtherVectorType1& _min, const OtherVectorType2& _max)
        : Base(_min, _max) {}


    //! Construct a TBox from AlignedBox (needed to play nice with Eigen)
    template <typename DerivedS, int DerivedD>
    TBox(const Eigen::AlignedBox<DerivedS, DerivedD> & p)
        : Base(p) {}

    //! Assign a TBox from AlignedBox (needed to play nice with Eigen)
    template <typename DerivedS, int DerivedD>
    TBox & operator=(const Eigen::AlignedBox<DerivedS, DerivedD> & p)
    {
        this->Base::operator=(p);
        return *this;
    }

    const VectorType & operator[](int i) const
    {
        assert(i == 0 || i == 1);
        return (&this->Base::m_min)[i];
    }

    VectorType & operator[](int i)
    {
        assert(i == 0 || i == 1);
        return (&this->Base::m_min)[i];
    }

    //! Calculate the n-1 dimensional volume of the boundary
    float surfaceArea() const
    {
        auto d = Base::diagonal();
        float result = 0.0f;
        for (auto i : range(Dimension))
        {
            float term = 1.0f;
            for (auto j : range(Dimension))
            {
                if (i == j)
                    continue;
                term *= d[j];
            }
            result += term;
        }
        return 2.0f * result;
    }

    //! Return the dimension index with the largest associated side length
    int majorAxis() const
    {
        auto d = Base::diagonal();
        int largest = 0;
        for (auto i : range(Dimension))
            if (d[i] > d[largest])
                largest = i;
        return largest;
    }

    //! Return the dimension index with the shortest associated side length
    int minorAxis() const
    {
        auto d = Base::diagonal();
        int shortest = 0;
        for (auto i : range(Dimension))
            if (d[i] < d[shortest])
                shortest = i;
        return shortest;
    }
};
