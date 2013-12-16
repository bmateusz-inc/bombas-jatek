#ifndef POINT_H
#define POINT_H

enum WAY { STILL, UP, RIGHT, DOWN, LEFT };

#include <cmath>
#include <iostream>

/** Templace class of point (x, y) */
template<typename T>
class Point
{
    private:
        T x;
        T y;

    public:
        /** Default constructor */
        Point() { x=y=0; }
        
        /**
         * Constructor
         * @param ix coordinate x
         * @param iy coordinate y
         */
        Point(T ix, T iy) : x(ix), y(iy) {}

        /** Getter for X coordinate */
        T getX() const { return x; }
        
        /** Getter for Y coordinate */
        T getY() const { return y; }
        
        /** Setter for X coordinate */
        void setX(T ix) { x = ix; }
        
        /** Setter for Y coordinate */
        void setY(T iy) { y = iy; }

        /** Equality check */
        friend bool operator== (const Point<T>& a, const Point<T>& b)
        {
            if(a.getX() == b.getX() && a.getY() == b.getY()) return true;
            else
            return false;
        }

        /** Non-equality check */
        friend bool operator!= (const Point<T>& a, const Point<T>& b)
        {
            if(a.getX() != b.getX() || a.getY() != b.getY()) return true;
            else
            return false;
        }

        /** Addition */
        friend Point<T> operator+ (const Point<T>& a, const Point<T>& b)
        {
            return Point<T>(a.getX() + b.getX(), a.getY() + b.getY());
        }

        /** Division by number */
        friend Point<T> operator/ (const Point<T>& a, const int& b)
        {
            return Point<T>(a.getX() / b, a.getY() / b);
        }

        /** Distance between two points */
        friend T distance(const Point<T>& a, const Point<T>& b)
        {
            T u = a.getX() - b.getX(), v = a.getY() - b.getY();
            return sqrt(u*u+v*v);
        }

        /** 
         * A special function, calculates the direction of a vector
         * based on previous and current position
         */
        friend WAY Way(const Point<T>& a, const Point<T>& b)
        {
            const float EPSION = 0.01;
            T x = a.getX() - b.getX();
            T y = a.getY() - b.getY();
            T absX = fabs(x);
            T absY = fabs(y);
            if(absX<EPSION && absY<EPSION) return STILL;
            if(absX > absY+EPSION)
            {
                if(x > EPSION) return RIGHT;
                else return LEFT;
            } else {
                if(y > EPSION) return DOWN;
                else return UP;
            }
        }
        
        /** Input stream operator */
        friend std::istream& operator>>(std::istream &input, Point<T> &point)
        {
            T temp;
            input >> temp;
            point.setX(temp);
            input >> temp;
            point.setY(temp);
            return input;
        }
        
        /** Output stream operator */
        friend std::ostream& operator<<(std::ostream &output, const Point<T> &point)
        {
            output << "(" << point.getX() << ", " << point.getY() << ")";
            return output;
        }
};

#endif
