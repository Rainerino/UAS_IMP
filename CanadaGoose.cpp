/**
 *
 */

#include "CanadaGoose.h"
//#include "fastBilateral.cpp"
using namespace std;
using namespace cv;

CanadaGoose::CanadaGoose() {
    smallSize = Size(WIDTH, HEIGHT);
}

vector<Mat> CanadaGoose::edgeDetection(Mat src) {
    //src is raw -> colored
    Mat input;
    Mat input_gray;
    Mat src_gray;
    GaussianBlur(src, src_gray, Size(7, 7), 0, 0);
    cvtColor(src, input_gray, CV_BGR2GRAY);
    //medianBlur(src, src_gray, 5);
    cvtColor(src_gray, src_gray, CV_BGR2GRAY);
    //equalizeHist(src_gray, src_gray);
    //cv_extend::bilateralFilter(src_gray, src_gray, 100, 10);
    threshold(src_gray, input, 123, 255, 0);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(input, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    vector<Rect> boundRect(contours.size());
    vector<vector<Point>> contours_poly( contours.size() );


    double area = 0;
    Mat drawing = Mat::zeros( input.size(), CV_8UC3 );

    for (int i = 0 ; i < contours.size(); i ++) {
        area = contourArea(contours[i]);
        if (area < 1000) {
            contours.erase(contours.begin() + i);
            i--;
        }
    }
    for( int i = 0; i < contours.size(); i++ ) {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect(Mat(contours_poly[i]));
    }
    for (int i = 0; i < contours.size(); i ++) {
        area = boundRect[i].area();
        if(area >= (src.cols * src.rows)) {
            contours.erase(contours.begin() + i);
            i--;
        }
    }
    for (int i = 0 ; i < contours.size(); i ++) {
        drawContours( drawing, contours, i, Scalar(0, 0, 255), 0, 8, vector<Vec4i>(), 0, Point() );

        rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 2, 8, 0);
    }
    //imshow("drawinng", drawing);
    //waitKey(0);

    Mat temp;
    vector<Mat> output;
    for (int i = 0 ; i < contours.size(); i ++) {

        int x = boundRect[i].tl().x ;
        int y = boundRect[i].tl().y;
        int w = boundRect[i].br().x - x;
        int h = boundRect[i].br().y - y;
        Rect rect = Rect(x, y, w, h);
        //Rect rect = Rect(100, 100, 20, 200);
        output.push_back(input_gray(rect));
        rectangle(input_gray, boundRect[i], Scalar(0, 0 ,255), 0, 8);
    }
    //imshow("src", input_gray);
    //waitKey(0);
    cout << output.size() << endl;

    return output;
}


void CanadaGoose::cascadeRecognition(){
    string temp, lastfile;
    geese_cascade.load(geese_cascade_name);
    Mat image;
    int num = 0;
    vector<Mat> output;
    while(true) {
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir("//home/yiyi/Documents/CLionProjects/UAS/Sample/")) != NULL) {
            while (( ent = readdir(dir)) != NULL ) {
                temp = PATHNAME +  ent->d_name;
                if(lastfile == temp) break;
                if (temp.find_first_of(NUMBER) != string::npos ) {
                    printf("%s\n" , temp.c_str());
                    inputImage = imread(temp.c_str());
                    inputImageArray = imageCutting(inputImage);
                    vector<Mat> tempArray;
                    for(int i = 0 ; i < inputImageArray.size(); i ++) {
                        cout << i << endl;
                        tempArray = edgeDetection(inputImageArray[i]);
                        for(int j = 0 ; j < tempArray.size(); j ++) {
                            outputIMageArray.push_back(tempArray[j]);
                        }
                        tempArray.clear();
                    }
                    for (int i = 0 ; i < outputIMageArray.size(); i ++) {
                        num += recognition(outputIMageArray[i]);
                    }
                    cout << num << endl;
                    displayOutput();

                }
                lastfile = temp;
                temp = "";
                num = 0;
                cin.get();
            }
            closedir(dir);
        }else{
            perror("");
            return; EXIT_FAILURE;
        }


    }

}
int CanadaGoose::recognition(Mat& frame) {

    std::vector<Rect> geese;
    geese_cascade.detectMultiScale(frame, geese, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(50, 50));

    for (size_t i = 0; i < geese.size(); i++) {
        Point center(geese[i].x + geese[i].width / 2, geese[i].y + geese[i].height / 2);
        //putText(frame, "Geese", center, FONT_HERSHEY_SIMPLEX, 1.0, CV_RGB(255, 0, 0), 2.0);
        ellipse( frame, center, Size( geese[i].width/2, geese[i].height/2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        Mat faceROI = frame(geese[i]);
    }
    return geese.size();

}

vector<Mat> CanadaGoose::imageCutting(Mat bigImage) {

    vector<Mat> smallImages;
    for (int i = 0 ; i < bigImage.rows; i += smallSize.height) {
        for (int j = 0 ; j < bigImage.cols; j += smallSize.width ) {
            Rect rect = Rect(j, i, smallSize.width, smallSize.height);
            smallImages.push_back(Mat(bigImage, rect));
        }
    }
    return smallImages;
}
void CanadaGoose::displayOutput() {
    String name = "";
    cout << outputIMageArray.size() << endl;
    for (int i = 0 ; i < outputIMageArray.size(); i ++) {
        name = to_string(i);
        imshow(name, outputIMageArray.at(i));
        name = "";
        waitKey(0);
    }
    //waitKey(0);
}

