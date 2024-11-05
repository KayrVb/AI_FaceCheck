#include <iostream>
#include "opencv2/opencv.hpp"
#include "face.h"

using namespace std;
using namespace cv;
using namespace aip;

int main()
{
    VideoCapture cap(0);
    if(!cap.isOpened())
    {
		cout << "Camera open failed!" << endl;
		return -1;
    }
    
    cout << "Camera open scuccess!" << endl;

	Mat img;
	Mat grayImg;
	Mat equalizeImg;
	Mat faceImg;

	CascadeClassifier classifier("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml");
 
	vector<Rect> allFace;
	vector<uchar> jpgBuf;

    // 设置APPID/AK/SK
    std::string app_id = "116028237";
    std::string api_key = "a7N2VGf5NQgHecNRWAElieAJ";
    std::string secret_key = "RP07XFiveunH9GUryR8I0885NNG4wzAU";

    aip::Face client(app_id, api_key, secret_key);

	Json::Value result;
	std::string base64Img;

	time_t sec;

    for(;;)
	{    	
    	cap >> img;
		cvtColor(img, grayImg, CV_BGR2GRAY);
		equalizeHist(grayImg, equalizeImg);
		classifier.detectMultiScale(equalizeImg, allFace);
		
		if(allFace.size())
		{
			rectangle(equalizeImg, allFace[0], Scalar(255,255,0));
			faceImg = equalizeImg(allFace[0]);
			imencode(".jpg", faceImg, jpgBuf);
			
			base64Img = base64_encode((char *)jpgBuf.data(), jpgBuf.size());

			result = client.search(base64Img, "BASE64", "Face_0001", aip::null);
			
			if( !result["result"].isNull() )
			{
				if(result["result"]["user_list"][0]["score"].asInt() > 80)
				{
					cout << result["result"]["user_list"][0]["user_id"] << endl;
					sec = time(NULL);
					//ctime(&sec)
					cout << ctime(&sec) << endl;
					putText(equalizeImg, result["result"]["user_list"][0]["user_id"].asString(), 
								Point(10, 50), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 255 ,255));
					putText(equalizeImg, ctime(&sec), Point(10, 100), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 255 ,255));		
				}
			}
			/*else
			{
				cout << "NO Result" << endl;
			}*/
			

			//cout << result << endl;			
		}
		

    	imshow("video0", equalizeImg);
		waitKey(40);
    }
	
    return 0;

}
