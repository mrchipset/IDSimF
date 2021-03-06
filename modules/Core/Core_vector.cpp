/***************************
 Ion Dynamics Simulation Framework (IDSimF)

 Copyright 2020 - Physical and Theoretical Chemistry /
 Institute of Pure and Applied Mass Spectrometry
 of the University of Wuppertal, Germany

 IDSimF is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 IDSimF is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with IDSimF.  If not, see <https://www.gnu.org/licenses/>.
 ****************************/

#include "Core_vector.hpp"
#include <iostream>
#include <cmath>

Core::Vector::Vector()
        :
        x_(0),
        y_(0),
        z_(0)
{}

Core::Vector::Vector(double x,double y, double z)
:
    x_(x),
    y_(y),
    z_(z)
{}

Core::Vector::Vector(double* coord){
    x_= coord[0];
    y_= coord[1];
    z_= coord[2];
}

void Core::Vector::printState(){
    std::cout << "x="<<x_<< " y="<<y_<<" z="<<z_<<"\n";
}

// Accessors:
double Core::Vector::x() const{
    return x_;
}

double Core::Vector::y() const{
    return y_;
}

double Core::Vector::z() const{
    return z_;
}

// Setters:
void Core::Vector::set(double x, double y, double z){
    x_=x;
    y_=y;
    z_=z;
}

void Core::Vector::x(double x){
    x_ = x;
}

void Core::Vector::y(double y){
    y_ = y;
}

void Core::Vector::z(double z){
    z_ = z;
}

// mathematical methods:

/**
 * Returns the magnitude of the vector
 */
double Core::Vector::magnitude(){
    return sqrt(x_*x_ + y_*y_ + z_*z_);
}

double Core::Vector::magnitudeSquared(){
    return (x_*x_ + y_*y_ + z_*z_);
}

// overloaded operators:
Core::Vector Core::operator+(const Core::Vector &lhs, const Core::Vector & rhs){
    return Core::Vector(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_);
}

Core::Vector Core::operator-(const Core::Vector &lhs, const Core::Vector &rhs){
    return Core::Vector(lhs.x_ - rhs.x_, lhs.y_ - rhs.y_, lhs.z_ - rhs.z_);
}

double Core::operator*(const Core::Vector &lhs, const Core::Vector &rhs){
    return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_ + lhs.z_ * rhs.z_;
}

Core::Vector Core::operator*(const Core::Vector &lhs, double rhs){
    return Core::Vector(lhs.x_ * rhs, lhs.y_ * rhs, lhs.z_ * rhs);
}

Core::Vector Core::operator/(const Core::Vector &lhs, double rhs){
    return lhs * (1.0 / rhs);
}

bool Core::operator==(Core::Vector const &lhs, Core::Vector const &rhs){
    return (lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ == rhs.z_);
}

bool Core::operator!=(const Core::Vector &lhs, const Core::Vector &rhs){
    return !(lhs == rhs);
}

std::ostream& operator<< (std::ostream& os,Core::Vector const& vec)
{
    os << vec.x() << ' ' << vec.y() << ' ' << vec.z();
    return os;
}
