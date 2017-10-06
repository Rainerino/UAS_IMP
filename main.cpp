
#include "CanadaGoose.h"

using namespace std;
using namespace cv;

int main( void ) {
    CanadaGoose geese = CanadaGoose();
    geese.getPath("/home/yiyi/Documents/CLionProjects/UAS/Sample/29.jpg");
    geese.setSavePath("/home/yiyi/Documents/CLionProjects/UAS/Sample/29.jpg");
    geese.cascadeRecognition();
    return 0;
}




