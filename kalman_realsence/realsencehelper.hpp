#pragma once
#include<opencv2\opencv.hpp>
#include<iostream>
#include<Windows.h>
using namespace std;
using namespace cv;
/*****************************************************
@ KillDCM ֻ���SR300����Ϊ�����ĺ������ò���
*ʹ��˼·��
���û���ҵ�������ų��������أ�Ӧ���Ƿ�������
*ʹ�ù淶��
���Է������������Ҳ���������һ�η���
������Ҳ�����˵��Ӳ�����������������
*****************************************************/
DWORD KillDCM();
Mat getMat(PXCImage* PXCImageInterface);
Mat getMat(PXCImage* PXCImageInterface, Size size, PXCImage::PixelFormat pixformat, int format);
int getMatType(PXCImage::PixelFormat fmt);
int getPXCImageFormat(int MatType);

DWORD KillDCM()
{
	cout << "��������DCM[";
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = TEXT("runas");
	sei.lpFile = TEXT("cmd.exe");
	sei.nShow = SW_HIDE;
	// ֹͣ
	sei.lpParameters = TEXT("cmd /c NET STOP RealSenseDCMSR300 & NET START RealSenseDCMSR300");

	if (!ShellExecuteEx(&sei)) {
		if (GetLastError() == ERROR_CANCELLED)
			cout << "����Ȩ�ޱ��û��ܾ�";
	}
	DWORD dwExitCode;
	GetExitCodeProcess(sei.hProcess, &dwExitCode);
	while (dwExitCode == STILL_ACTIVE) {
		Sleep(800);
		GetExitCodeProcess(sei.hProcess, &dwExitCode);
		cout << "=";
	}
	cout << "]";
	CloseHandle(sei.hProcess);
	dwExitCode ? cout << "���ʴ���(" << dwExitCode << ")" << endl : cout << "���" << endl;
	return dwExitCode;
}

Mat getMat(PXCImage* PXCImageInterface)
{
	auto info = PXCImageInterface->QueryInfo();
	PXCImage::ImageData img_dat;
	PXCImageInterface->AcquireAccess(PXCImage::Access::ACCESS_READ, info.format, &img_dat);
	Mat img = Mat(info.height, info.width, getMatType(info.format), (void*)img_dat.planes[0], img_dat.pitches[0] / sizeof(uchar)).clone();
	PXCImageInterface->ReleaseAccess(&img_dat);
	return img;
}
Mat getMat(PXCImage* PXCImageInterface, Size size, PXCImage::PixelFormat pixformat, int format)
{
	PXCImage::ImageData img_dat;
	PXCImageInterface->AcquireAccess(PXCImage::Access::ACCESS_READ, pixformat, &img_dat);
	Mat img = Mat(size.height, size.width, format, (void*)img_dat.planes[0], img_dat.pitches[0] / sizeof(uchar)).clone();
	PXCImageInterface->ReleaseAccess(&img_dat);
	return img;
}

int getMatType(PXCImage::PixelFormat fmt)
{
	switch (fmt)
	{
	case PXCImage::PIXEL_FORMAT_YUY2:
		return CV_8UC3;
	case PXCImage::PIXEL_FORMAT_NV12:
		return CV_8UC3;
	case PXCImage::PIXEL_FORMAT_RGB32:
		return CV_8UC4;
	case PXCImage::PIXEL_FORMAT_RGB24:
		return CV_8UC3;
	case PXCImage::PIXEL_FORMAT_Y8:
		return CV_8UC1;
	case PXCImage::PIXEL_FORMAT_DEPTH:
		return CV_16UC1;
	case PXCImage::PIXEL_FORMAT_DEPTH_RAW:
		return CV_16UC1;
	case PXCImage::PIXEL_FORMAT_DEPTH_F32:
		return CV_32FC1;
	case PXCImage::PIXEL_FORMAT_DEPTH_CONFIDENCE:
		return CV_8UC1;
	case PXCImage::PIXEL_FORMAT_Y16:
		return CV_16UC1;
	case PXCImage::PIXEL_FORMAT_Y8_IR_RELATIVE:
		return CV_8UC1;
	default:
		return CV_8U;
	}
}
int getPXCImageFormat(int MatType)
{
	switch (MatType)
	{
	default:
		break;
	}
}