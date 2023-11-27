#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    /*cout << (void *) argv[argc] << endl;
    cout <<  argv[argc + 1] << endl;*/
    // this works because, environment variables are stored on stack like this.
    //*(char *) 0 = 0;
    *reinterpret_cast<char*>(nullptr) = 0;
    // gcc puts a trap over here, so seg fault
    return 0;
}