#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat GetSpectrum(const Mat &src)
{
	Mat dst, cpx;
	Mat planes[] = { Mat_<float>(src), Mat::zeros(src.size(), CV_32F) };
	merge(planes, 2, cpx);
	dft(cpx, cpx);
	split(cpx, planes);
	magnitude(planes[0], planes[1], dst);
	//频谱就是频域幅度图的平方
	multiply(dst, dst, dst);
	return dst;
}

Mat WienerFilter(const Mat &src, const Mat &ref, int stddev)
{
	//这些图片是过程中会用到的，pad是原图像0填充后的图像，cpx是双通道频域图，mag是频域幅值图，dst是滤波后的图像
	Mat pad, cpx, dst;

	//获取傅里叶变化最佳图片尺寸，为2的指数
	int m = getOptimalDFTSize(src.rows);
	int n = getOptimalDFTSize(src.cols);

	//对原始图片用0进行填充获得最佳尺寸图片
	copyMakeBorder(src, pad, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));

	//获得参考图片频谱
	Mat tmpR(pad.rows, pad.cols, CV_8U);
	resize(ref, tmpR, tmpR.size());
	Mat refSpectrum = GetSpectrum(tmpR);

	//获得噪声频谱
	Mat tmpN(pad.rows, pad.cols, CV_32F);
	randn(tmpN, Scalar::all(0), Scalar::all(stddev));
	Mat noiseSpectrum = GetSpectrum(tmpN);

	//对src进行傅里叶变换
	Mat planes[] = { Mat_<float>(pad), Mat::zeros(pad.size(), CV_32F) };
	merge(planes, 2, cpx);
	dft(cpx, cpx);
	split(cpx, planes);

	//维纳滤波因子
	Mat factor = refSpectrum / (refSpectrum + noiseSpectrum);
	multiply(planes[0], factor, planes[0]);
	multiply(planes[1], factor, planes[1]);

	//重新合并实部planes[0]和虚部planes[1]
	merge(planes, 2, cpx);

	//进行反傅里叶变换
	idft(cpx, dst, DFT_SCALE | DFT_REAL_OUTPUT);

	dst.convertTo(dst, CV_8UC1);
	return dst;
}

int main(int argc, char** argv)
{
    //if (argc != 2)
    //{
    //    cout << "Usage: opencv_test <image path>" << endl;
    //    return -1;
    //}

    //char *imgName = argv[1]; 
	char *imgName = "C:\\Users\\VINNO\\Desktop\\src0\\die.jpg";
    Mat inputImg;

    inputImg = imread(imgName, 1);
    if (!inputImg.data)
    {
        cout << "No image data" << endl;
        return -1;
    }
    Mat resultImg;

	fastNlMeansDenoisingColored(inputImg, resultImg, 10, 10, 7, 21);
    imwrite("./result.jpg", resultImg);
	imshow("", resultImg);
	waitKey(0);

    return 0;
}

