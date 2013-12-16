#include "test.h"
#include "../view/matrix.h"

using namespace std;

int main(int argc, char** argv) {
    
    Matrix<int> m;
    std::stringstream ss (" 4 2 1 2 3 4 5 6 7 8");
    ss >> m;
    std::cout << m << std::endl << m(2,1);
    Point<int>p = m.find(4);
    std::cout << p.getX() << "," << p.getY() << std::endl;
    
    return 0;
}

