
#include "CanadaGoose.h"

using namespace std;
using namespace cv;

int main( void ) {
    CanadaGoose geese = CanadaGoose();
    geese.cascadeRecognition();
    return 0;
}




