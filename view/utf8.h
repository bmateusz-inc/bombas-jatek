#ifndef UTF8_H
#define UTF8_H

#include <sstream>

/**
 * Class for UTF-8 character handling
 */
class Utf8
{
    private:
        /** First byte */
        unsigned char a;
        
        /** Second byte*/
        unsigned char b;

    public:
        /** Constructor */
        Utf8() { a=b=0; }

        /**
         * Output operator, don't write second byte if it is null
         * @return standard ostream
         */
        friend std::ostream& operator<<(std::ostream& output, const Utf8 &u)
        {
            output << u.a;
            if(u.b) output << u.b;
            return output;
        }
        
        /**
         * Input operator, smartly determines UTF-8 characters and only read
         * the second byte if it is valid UTF-8 character
         * @return standard istream
         */
        friend std::istream& operator>>(std::istream& input, Utf8 &u)
        {
            do
            {
                input >> std::noskipws >> u.a;
                if(u.a > 0x7f) input >> u.b;
            } while(u.a == '\n');
            return input;
        }

        /** Equality operator */
        friend bool operator==(const Utf8 &u, const Utf8 &v)
        {
            return u.a==v.a && u.b==v.b;
        }
        
        /** Greater than operator */
        friend bool operator>(const Utf8 &u, const Utf8 &v)
        {
            if(u.a==v.a) return u.b>v.b;
            else return u.a>v.a;
        }

        /** Less than operator */
        friend bool operator<(const Utf8 &u, const Utf8 &v)
        {
            if(u.a==v.a) return u.b<v.b;
            else return u.a<v.a;
        }

        /** Character is not space */
        bool notSpace() { return a != ' ' || b != 0; }
        
        /** Character is not null */
        bool notNull() { return a != 0 || b != 0; }
        
        /** Character is not line-break */
        bool isBr() { return false; }

};

#endif
