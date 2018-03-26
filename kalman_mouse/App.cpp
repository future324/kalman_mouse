#include "opencv2/video/tracking.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include <iostream>  
using namespace cv;
using namespace std;
const int winWidth = 800;
const int winHeight = 600;
Point mousePosition = Point(winWidth >> 1, winHeight >> 1);

//mouse call back  
void mouseEvent(int event, int x, int y, int flags, void *param)
{
	if (event == CV_EVENT_MOUSEMOVE)
	{
		mousePosition = Point(x, y);
	}
}
int main()
{
	//1.kalman filter setup     
	const int stateNum = 4;
	const int measureNum = 2;

	KalmanFilter KF(stateNum, measureNum, 0);
	Mat state(stateNum, 1, CV_32FC1); //state(x,y,detaX,detaY)  
	Mat processNoise(stateNum, 1, CV_32F);
	Mat measurement = Mat::zeros(measureNum, 1, CV_32F);    //measurement(x,y)  
	randn(state, Scalar::all(0), Scalar::all(0.1)); //�������һ������������0����׼��Ϊ0.1;  
	KF.transitionMatrix = (Mat_<float>(4, 4) <<
		1, 0, 1, 0,
		0, 1, 0, 1,
		0, 0, 1, 0,
		0, 0, 0, 1);//Ԫ�ص�����󣬰���;  

					//setIdentity: ���ŵĵ�λ�ԽǾ���;  
					//!< measurement matrix (H) �۲�ģ��  
	setIdentity(KF.measurementMatrix);

	//!< process noise covariance matrix (Q)  
	// wk �ǹ������������ٶ�����Ͼ�ֵΪ�㣬Э�������ΪQk(Q)�Ķ�Ԫ��̬�ֲ�;  
	setIdentity(KF.processNoiseCov, Scalar::all(1e-5));

	//!< measurement noise covariance matrix (R)  
	//vk �ǹ۲����������ֵΪ�㣬Э�������ΪRk,�ҷ�����̬�ֲ�;  
	setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
	//!< priori error estimate covariance matrix (P'(k)): P'(k)=A*P(k-1)*At + Q)*/  A����F: transitionMatrix  
	//Ԥ�����Э�������;  
	setIdentity(KF.errorCovPost, Scalar::all(1));
	//!< corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))  
	//initialize post state of kalman filter at random   
	randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));
	Mat showImg(winWidth, winHeight, CV_8UC3);
	for (;;)
	{
		setMouseCallback("Kalman", mouseEvent);
		showImg.setTo(0);
		Point statePt = Point((int)KF.statePost.at<float>(0), (int)KF.statePost.at<float>(1));
		//2.kalman prediction     
		Mat prediction = KF.predict();
		Point predictPt = Point((int)prediction.at<float>(0), (int)prediction.at<float>(1));
		//3.update measurement  
		measurement.at<float>(0) = (float)mousePosition.x;
		measurement.at<float>(1) = (float)mousePosition.y;
		//4.update  
		KF.correct(measurement);
		//randn( processNoise, Scalar(0), Scalar::all(sqrt(KF.processNoiseCov.at<float>(0, 0))));  
		//state = KF.transitionMatrix*state + processNoise;  
		//draw  
		circle(showImg, statePt, 5, CV_RGB(255, 0, 0), 1);//former point  
		circle(showImg, predictPt, 5, CV_RGB(0, 255, 0), 1);//predict point  
		circle(showImg, mousePosition, 5, CV_RGB(0, 0, 255), 1);//ture point  
																//          CvFont font;//����  
																//          cvInitFont(&font, CV_FONT_HERSHEY_SCRIPT_COMPLEX, 0.5f, 0.5f, 0, 1, 8);  
		putText(showImg, "Red: Former Point", cvPoint(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(255));
		putText(showImg, "Green: Predict Point", cvPoint(10, 60), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(255));
		putText(showImg, "Blue: Ture Point", cvPoint(10, 90), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(255));
		imshow("Kalman", showImg);
		int key = waitKey(3);
		if (key == 27)
		{
			break;
		}
	}
}