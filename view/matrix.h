#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

#include "../point.h"

/** Template class of n x m matrix */
template<typename T>
class Matrix
{
    private:
        std::vector< std::vector<T> > v;
        
        /** Number of rows (Y) */
        int n;
        
        /** Number of columns (X) */
        int m;

    public:

        /** Find the first occurence of element in matrix */
        Point<int> find(const T& s)
        {
            for(int i=0;i<m;++i)
                for(int j=0;j<n;++j)
                    if(s == v[i][j]) return Point<int>(j,i);
            return Point<int>(m-1,n-1);
        }
        
        /** Default constructor */
        Matrix() { n=m=0; }

        /**
         * Constructor 
         * @param in number of rows
         * @param im number of columns
         */
        Matrix(int in, int im) { setSize(in,im); }

        /** Number of rows (Y) */
        int getN() { return n; }
        
        /** Number of columns (X) */
        int getM() { return m; }

        /** Set size of matrix */
        void setSize(int in, int im)
        {
            n=in;
            m=im;
            v.resize(m);
            for(int i=0; i<m; ++i) v[i].resize(n);
        }

        /** The given point is in a valid range or not */
        bool valid(Point<int> p)
        {
            return valid(p.getX(), p.getY());
        }
        
        /** The given coordinates is in a valid range or not */
        bool valid(int x, int y)
        {
            return x>=0 && y>=0 && x<n && y<m;
        }

        /** Count an element's number of occurences in matrix */
        unsigned int count(const T& e)
        {
            unsigned int c=0;
            for(int i=0;i<n;++i)
                for(int j=0;j<m;++j)
                {
                    if(e == operator()(i,j)) ++c;
                }
            return c;
        }

        /** Get element at point */
        T& operator()(Point<int> p)
        {
            return operator()(p.getX(), p.getY());
        }

        /** Get element at coordinates */
        T& operator()(int i, int j)
        {
            try{
                return v.at(j).at(i);
            }
            catch(std::exception& e)
            {
                if (v.size() == 0) std::cout << e.what() << std::endl;
                else std::cout << " " << e.what()
                        << " " << j << "x" << i << " - max: "
                        << v.size()-1 << "x" << v[0].size()-1 << std::endl;
            }
            return v[j][i];
        }

        /** Get element at point (const) */
        const T& operator()(Point<int> p) const
        {
            operator()(p.getX(), p.getY());
        }

        /** Get element at coordinates (const) */
        const T& operator()(int i, int j) const
        {
            try{
                return v.at(j).at(i);
            }
            catch(std::exception& e)
            {
                if (v.size() == 0) std::cout << e.what() << std::endl;
                else std::cout << " " << e.what()
                        << " " << j << "x" << i << " - max: "
                        << v.size()-1 << "x" << v[0].size()-1 << std::endl;
            }
            return v[j][i];
        }

        /** Input stream operator */
        friend std::istream& operator>>(std::istream &input, Matrix<T> &p)
        {
            int n,m;
            input >> n >> m;
            //std::cout << n << " x " << m << std::endl;
            p.setSize(n,m);
            for(int i=0;i<m;i++)
            {
                for(int j=0;j<n;j++)
                {
                    T t;
                    input >> t;
                    p(j,i) = t;
                    //std::cout << ":" << t;
                }
                //std::cout << std::endl;
            }
            return input;
        }

        /** Output stream operator */
        friend std::ostream& operator<<(std::ostream& output, const Matrix<T> &p)
        {
            for(int i=0;i<p.m;i++)
            {
                for(int j=0;j<p.n;j++)
                    output << p(j,i) << "\t";
                output << std::endl;
            }
            return output;
        }
};

#endif
