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
	//Ƶ�׾���Ƶ�����ͼ��ƽ��
	multiply(dst, dst, dst);
	return dst;
}

Mat WienerFilter(const Mat &src, const Mat &ref, int stddev)
{
	//��ЩͼƬ�ǹ����л��õ��ģ�pad��ԭͼ��0�����ͼ��cpx��˫ͨ��Ƶ��ͼ��mag��Ƶ���ֵͼ��dst���˲����ͼ��
	Mat pad, cpx, dst;

	//��ȡ����Ҷ�仯���ͼƬ�ߴ磬Ϊ2��ָ��
	int m = getOptimalDFTSize(src.rows);
	int n = getOptimalDFTSize(src.cols);

	//��ԭʼͼƬ��0�����������ѳߴ�ͼƬ
	copyMakeBorder(src, pad, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));

	//��òο�ͼƬƵ��
	Mat tmpR(pad.rows, pad.cols, CV_8U);
	resize(ref, tmpR, tmpR.size());
	Mat refSpectrum = GetSpectrum(tmpR);

	//�������Ƶ��
	Mat tmpN(pad.rows, pad.cols, CV_32F);
	randn(tmpN, Scalar::all(0), Scalar::all(stddev));
	Mat noiseSpectrum = GetSpectrum(tmpN);

	//��src���и���Ҷ�任
	Mat planes[] = { Mat_<float>(pad), Mat::zeros(pad.size(), CV_32F) };
	merge(planes, 2, cpx);
	dft(cpx, cpx);
	split(cpx, planes);

	//ά���˲�����
	Mat factor = refSpectrum / (refSpectrum + noiseSpectrum);
	multiply(planes[0], factor, planes[0]);
	multiply(planes[1], factor, planes[1]);

	//���ºϲ�ʵ��planes[0]���鲿planes[1]
	merge(planes, 2, cpx);

	//���з�����Ҷ�任
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

