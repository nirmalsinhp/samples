#include "UF.h"
using namespace std;

int main()
{
    UF uf(100);
    cout << uf.same_component(1,99) << endl;
    return 0;
}