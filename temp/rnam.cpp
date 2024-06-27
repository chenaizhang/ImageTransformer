//rnam.cpp

#include "rnam.h"
using namespace cv;
rnam::rnam():method(){}

rnam::rnam(method* p) :method() {
	origImg = (p->origImg).clone();
	methodId = p->methodId;
	h = p->h;
	w = p->w;
	xigema = p->xigema;
}

void rnam::process()
{
	Mat markMatrix = Mat::zeros(h, w, CV_8UC1);//初始化标记矩阵
	Mat R = Mat::zeros(h, w, CV_8UC1);
	vector<Color> colorList;
	vector<char> coordinateList;
	MyTimer mt;
	mt.Start();
	/*分块*/
	RNAMCEncoding(origImg, R, markMatrix, h, w, colorList, coordinateList, xigema, methodId);

	/*矩阵编码*/
	EnCode(R, h, w, coordinateList);
	mt.End();

	enTime = mt.costTime;
	mt.Reset();

	/*二，还原图像矩阵及图像*/
	compressedImg = (Mat::zeros(h, w, CV_8UC1));

	mt.Start();
	Decode(compressedImg, h, w, coordinateList);
	RNAMCDecoding(compressedImg, h, w, colorList, coordinateList);
	mt.End();
	deTime = mt.costTime;
	mt.Reset();

	blocknum = colorList.size();
	PSNR = psnr(origImg, compressedImg, h, w);
	BPP = bbp(colorList, h, w, coordinateList);
	CR = 8.0 / BPP;


	/*分割图*/
	segImg = Mat(h, w, CV_8UC1, Scalar::all(255)).clone();
	segmentDisplay(segImg, colorList);
}

//使用改良isSameBlock阴影法判断是否同类块
bool isSameBlock(Mat f, Mat mark, Point lt, Point rb, int xgm)
{
	uchar g1 = f.ptr<uchar>(lt.y)[lt.x];
	uchar g2 = f.ptr<uchar>(lt.y)[rb.x];
	uchar g3 = f.ptr<uchar>(rb.y)[lt.x];
	uchar g4 = f.ptr<uchar>(rb.y)[rb.x];
	//标准矩形
	bool flag = true;
	if (lt.x < rb.x && lt.y < rb.y) {
		for (int j = lt.y; j <= rb.y; j++) {
			uchar* f_ptr = f.ptr<uchar>(j);
			uchar* mark_ptr = mark.ptr<uchar>(j);
			for (int i = lt.x; i <= rb.x; i++) {
				double i1 = (j - lt.y) / (double)(rb.y - lt.y);
				double i2 = (i - lt.x) / (double)(rb.x - lt.x);
				double g5 = g1 + (g2 - g1) * i2;
				double g6 = g3 + (g4 - g3) * i2;
				double gest = g5 + (g6 - g5) * i1;
				if (abs(f_ptr[i] - gest) > xgm || mark_ptr[i] == 1) {
					flag = false;
					break;
				}
			}
			if (flag == false) break;
		}
		return flag;
	}
	// 宽为1的矩形
	else if (lt.x != rb.x && lt.y == rb.y) {
		uchar* f_ptr = f.ptr<uchar>(lt.y);
		uchar* mark_ptr = mark.ptr<uchar>(lt.y);
		for (int i = lt.x; i <= rb.x; i++) {
			double i2 = (i - lt.x) / (double)(rb.x - lt.x);
			double gest = g1 + (g4 - g1) * i2;
			if (abs(f_ptr[i] - gest) > xgm || mark_ptr[i] == 1) {
				flag = false;
				break;
			}
		}
		return flag;
	}
	// 长为1的矩形
	else if (lt.x == rb.x && lt.y != rb.y) {
		for (int j = lt.y; j <= rb.y; j++) {
			double i1 = (j - lt.y) / (double)(rb.y - lt.y);
			double gest = g1 + (g4 - g1) * i1;
			if (abs(f.ptr<uchar>(j)[lt.x] - gest) > xgm || mark.ptr<uchar>(j)[lt.x] == 1) {
				flag = false;
				break;
			}
		}
		return flag;
	}
	// 孤立点矩形
	else if (lt.x == rb.x && lt.y == rb.y) {
		if (mark.ptr<uchar>(lt.y)[lt.x] == 1) return 0;
		return 1;
	}
}

//给灰度图f已经搜索过的矩形子模式做标记
void mark(Mat& mark, Point lt, Point rb) {
	for (int i = lt.y; i <= rb.y; i++) {
		uchar* mark_ptr = mark.ptr<uchar>(i);
		for (int j = lt.x; j <= rb.x; j++) {
			mark_ptr[j] = 1;
		}
	}
}

void EnCode(Mat& R, int height, int width, vector<char>& Q) {
	int c = 0;
	int offsetValue = 0;
	int count = 0;

	for (int i = 0; i < height; i++) {
		c = 0;
		offsetValue = 0;
		count = 0;
		uchar* rowPtr = R.ptr<uchar>(i); // 使用指针访问矩阵行
		for (int j = 0; j < width; j++) {
			int value = rowPtr[j]; // 直接通过指针访问矩阵元素
			if (0 == value) {
				if (j == width - 1) {
					Q.push_back('0');
				}
			}
			else {
				if (1 == value) {
					Q.push_back('1');
					Q.push_back('1');
				}
				else if (2 == value) {
					Q.push_back('1');
					Q.push_back('0');
					Q.push_back('1');
				}
				else if (3 == value) {
					Q.push_back('1');
					Q.push_back('0');
					Q.push_back('0');
				}

				int b = ceil(log((double)(width - c - count)) / log(2.0));
				if (0 == count) {
					offsetValue = j;
				}
				else {
					offsetValue = j - c - 1;
				}

				b = (b == 0) ? 1 : b;
				count = 1;
				c = j;

				for (int v = b - 1; v >= 0; v--) {
					if ((offsetValue & (1 << v)) == 0) {
						Q.push_back('0');
					}
					else {
						Q.push_back('1');
					}
				}
			}
		}
	}
}

void Decode(Mat& R, int height, int width, const vector<char>& Q)
{
	int n = 0;
	int count = 0;
	//上个非零元素的位置
	int c = 0;
	int row = 0;
	int num = 0;
	for (int pos = 0; pos < Q.size(); pos++)
	{
		if ('1' == Q[pos])
		{
			pos++;
			if ('1' == Q[pos])
			{
				n = 1;
			}
			else
			{
				pos++;
				if ('1' == Q[pos])
				{
					n = 2;
				}
				else
				{
					n = 3;
				}
			}
		}
		else
		{
			row++;
			count = 0;
			c = 0;
			num = 0;
			continue;
		}

		int b = ceil(log((double)(width - c - count)) / log(2.0));
		b = b == 0 ? 1 : b;
		count = 1;

		int value = 0;
		for (int i = b - 1; i >= 0; i--)
		{
			pos++;
			if ('1' == Q[pos])
			{
				value += 1 << i;
			}
		}

		if (0 == num)
		{
			c = c + value;
		}
		else
		{
			c = c + value + 1;
		}

		num++;

		R.at<uchar>(row, c) = n;

		if (c == (width - 1))
		{
			row++;
			count = 0;
			c = 0;
			num = 0;
		}
	}

	
}

//对角线混合法寻找最大的矩形子模式，谁的子模式大选谁
Point diagonalPriority(Mat f, Mat mark, int M, int N, Point start, int xgm) {
	Point end = start;
	Point end1, end2;

	// 对角线移动式检索是否是同类块，在理想情况下遇到同类块跳出并回退一格
	while (end.x < M && end.y < N && (mark.ptr<uchar>(end.y)[end.x] == 0) && (mark.ptr<uchar>(start.y)[start.x] == 0)) {
		if (!isSameBlock(f, mark, start, end, xgm)) break;
		end.x++;
		end.y++;
	}
	end.x--;
	end.y--;

	end1 = end;
	// 横向检索是否是同类块，防止右边还有同类块
	while (end1.x < M && (mark.ptr<uchar>(end1.y)[end1.x] == 0)) {
		if (!isSameBlock(f, mark, start, end1, xgm)) break;
		end1.x++;
	}
	end1.x--;

	end2 = end;
	// 竖向检索是否是同类块，防止下边还有同类块
	while (end2.y < N && (mark.ptr<uchar>(end2.y)[end2.x] == 0)) {
		if (!isSameBlock(f, mark, start, end2, xgm)) break;
		end2.y++;
	}
	end2.y--;

	// 根据横向和纵向扩展的面积大小来确定最后返回的结束点
	if ((end2.x - start.x + 1) * (end2.y - start.y + 1) > (end1.x - start.x + 1) * (end1.y - start.y + 1))
		return end2;
	else
		return end1;
}

//对角线混合法寻找最大的矩形子模式,优先x
Point diagonalPriority_x(Mat f, Mat mark, int M, int N, Point start, int xgm) {
	Point end = start;
	Point end1;

	// 对角线移动式检索是否是同类块，在理想情况下遇到同类块跳出并回退一格
	while (end.x < M && end.y < N && (mark.ptr<uchar>(end.y)[end.x] == 0) && (mark.ptr<uchar>(start.y)[start.x] == 0)) {
		if (!isSameBlock(f, mark, start, end, xgm)) break;
		end.x++;
		end.y++;
	}
	end.x--;
	end.y--;

	end1 = end;
	// 横向检索是否是同类块，防止右边还有同类块
	while (end1.x < M && (mark.ptr<uchar>(end1.y)[end1.x] == 0)) {
		if (!isSameBlock(f, mark, start, end1, xgm)) break;
		end1.x++;
	}
	end1.x--;
	return end1;
}

//对角线混合法寻找最大的矩形子模式,优先y
Point diagonalPriority_y(Mat f, Mat mark, int M, int N, Point start, int xgm) {
	Point end = start;
	Point end2;

	// 对角线移动式检索是否是同类块，在理想情况下遇到同类块跳出并回退一格
	while (end.x < M && end.y < N && (mark.ptr<uchar>(end.y)[end.x] == 0) && (mark.ptr<uchar>(start.y)[start.x] == 0)) {
		if (!isSameBlock(f, mark, start, end, xgm)) break;
		end.x++;
		end.y++;
	}
	end.x--;
	end.y--;
	end2 = end;
	// 竖向检索是否是同类块，防止下边还有同类块
	while (end2.y < N && (mark.ptr<uchar>(end2.y)[end2.x] == 0)) {
		if (!isSameBlock(f, mark, start, end2, xgm)) break;
		end2.y++;
	}
	end2.y--;
return end2;
}

//水平竖直混合法寻找最大的矩形子模式，谁的子模式大选谁
Point hirizonalverticalPriority(Mat f, Mat mark, int M, int N, Point start, int xgm) {
	Point end2, end1 = end2 = start;

	while (end1.x < M && (mark.at<uchar>(end1) != 1)) {
		if (!isSameBlock(f, mark, start, end1, xgm)) break;
		end1.x++;
	}
	end1.x--;
	while (end1.y < N && (mark.at<uchar>(end1) != 1)) {
		if (!isSameBlock(f, mark, start, end1, xgm)) break;
		end1.y++;
	}
	end1.y--;

	while (end2.y < N && (mark.at<uchar>(end2) != 1)) {
		if (!isSameBlock(f, mark, start, end2, xgm)) break;
		end2.y++;
	}
	end2.y--;
	while (end2.x < M && (mark.at<uchar>(end2) != 1)) {
		if (!isSameBlock(f, mark, start, end2, xgm)) break;
		end2.x++;
	}
	end2.x--;


	if ((end2.x - start.x + 1) * (end2.y - start.y + 1) > (end1.x - start.x + 1) * (end1.y - start.y + 1))return end2;
	else return end1;
}

//水平法寻找最大的矩形子模式
Point hirizonalPriority(Mat f, Mat mark, int M, int N, Point start, int xgm) {
	Point  end1 = start;

	while (end1.x < M && (mark.at<uchar>(end1) != 1)) {
		if (!isSameBlock(f, mark, start, end1, xgm)) break;
		end1.x++;
	}
	end1.x--;
	while (end1.y < N && (mark.at<uchar>(end1) != 1)) {
		if (!isSameBlock(f, mark, start, end1, xgm)) break;
		end1.y++;
	}
	end1.y--;
return end1;
}

//竖直法寻找最大的矩形子模式
Point verticalPriority(Mat f, Mat mark, int M, int N, Point start, int xgm) {
	Point end2 = start;

	while (end2.y < N && (mark.at<uchar>(end2) != 1)) {
		if (!isSameBlock(f, mark, start, end2, xgm)) break;
		end2.y++;
	}
	end2.y--;
	while (end2.x < M && (mark.at<uchar>(end2) != 1)) {
		if (!isSameBlock(f, mark, start, end2, xgm)) break;
		end2.x++;
	}
	end2.x--;
	return end2;

}

//总混合法寻找最大的矩形子模式函数
Point sizePriority(Mat f, Mat mark, int M, int N, Point start, int xgm) {
	Point re = diagonalPriority(f, mark, M, N, start, xgm);
	Point ma = hirizonalverticalPriority(f, mark, M, N, start, xgm);
	if ((re.x - start.x + 1) * (re.y - start.y + 1) > (ma.x - start.x + 1) * (ma.y - start.y + 1)) {
		return re;
	}
	else {
		return ma;
	}
}

//灰度图f编码
void RNAMCEncoding(Mat f, Mat& R, Mat& markMatrix, int M, int N, vector<Color>& p, vector<char>& q, int xgm, int methodId) {
	Point start, end;
	for (int i = 0; i < M; i++) {
		uchar* rowPtr = markMatrix.ptr<uchar>(i);
		for (int j = 0; j < N; j++) {
			uchar& pixel = rowPtr[j];
			if (pixel == 0) {
				start = Point(j, i);

				//根据methodId选择方法
				switch (methodId)
				{
				case 1:
					end = diagonalPriority(f, markMatrix, M, N, start, xgm);
					break;
				case 2:
					end = diagonalPriority_x(f, markMatrix, M, N, start, xgm);
					break;
				case 3:
					end = diagonalPriority_y(f, markMatrix, M, N, start, xgm);
					break;
				case 4:
					end = hirizonalverticalPriority(f, markMatrix, M, N, start, xgm);
					break;
				case 5:
					end = hirizonalPriority(f, markMatrix, M, N, start, xgm);
					break;
				case 6:
					end = verticalPriority(f, markMatrix, M, N, start, xgm);
					break;
				case 7:
					end = sizePriority(f, markMatrix, M, N, start, xgm);
					break;

				}

				Color p1;
				p1.setLt(start);
				p1.setRb(end);
				uchar g1 = f.ptr<uchar>(start.y)[start.x];
				uchar g2 = f.ptr<uchar>(start.y)[end.x];
				uchar g3 = f.ptr<uchar>(end.y)[start.x];
				uchar g4 = f.ptr<uchar>(end.y)[end.x];
				if (start.x < end.x && start.y < end.y) {
					p1.setGrey(Scalar(g1, g2, g3, g4));
					R.ptr<uchar>(start.y)[start.x] = 1;
					R.ptr<uchar>(end.y)[end.x] = 2;
				}
				// 宽为1的矩形
				if (start.x != end.x && start.y == end.y) {
					p1.setGrey(Scalar(g1, 0, 0, g4));
					R.ptr<uchar>(start.y)[start.x] = 1;
					R.ptr<uchar>(end.y)[end.x] = 2;
				}
				// 长为1的矩形
				if (start.x == end.x && start.y != end.y) {
					p1.setGrey(Scalar(g1, 0, 0, g4));
					R.ptr<uchar>(start.y)[start.x] = 1;
					R.ptr<uchar>(end.y)[end.x] = 2;
				}
				// 孤立点矩形
				if (start.x == end.x && start.y == end.y) {
					p1.setGrey(Scalar(g1, 0, 0, 0));
					R.ptr<uchar>(start.y)[start.x] = 3;
				}
				p.push_back(p1);
				mark(markMatrix, start, end);
			}
		}
	}
}

//恢复矩形子模式的值
void RDHelper(Mat& R, Color p1) {
	Point lt = p1.getLt();
	Point rb = p1.getRb();
	uchar g1 = p1.getGrey()[0];
	uchar g2 = p1.getGrey()[1];
	uchar g3 = p1.getGrey()[2];
	uchar g4 = p1.getGrey()[3];
	//标准矩形
	if (lt.x < rb.x && lt.y < rb.y)
	{

		for (int j = lt.y; j <= rb.y; j++)
		{
			for (int i = lt.x; i <= rb.x; i++)
			{
				double i1 = (j - lt.y) / (double)(rb.y - lt.y);
				double i2 = (i - lt.x) / (double)(rb.x - lt.x);
				double g5 = g1 + (g2 - g1) * i2;
				double g6 = g3 + (g4 - g3) * i2;
				double gest = g5 + (g6 - g5) * i1;
				R.at<uchar>(Point(i, j)) = gest;
			}
		}
	}
	//宽为1的矩形
	if (lt.x != rb.x && lt.y == rb.y)
	{
		for (int i = lt.x; i <= rb.x; i++)
		{
			double i2 = (i - lt.x) / (double)(rb.x - lt.x);
			double gest = g1 + (g4 - g1) * i2;
			R.at<uchar>(Point(i, lt.y)) = gest;
		}
	}
	//长为1的矩形
	if (lt.x == rb.x && lt.y != rb.y)
	{
		for (int j = lt.y; j <= rb.y; j++)
		{
			double i1 = (j - lt.y) / (double)(rb.y - lt.y);
			double gest = g1 + (g4 - g1) * i1;
			R.at<uchar>(Point(lt.x, j)) = gest;
		}
	}
	//孤立点矩形
	if (lt.x == rb.x && lt.y == rb.y)
	{
		R.at<uchar>(Point(lt.x, lt.y)) = g1;
	}
}

//灰度图f解码
void RNAMCDecoding(Mat& R, int M, int N, vector<Color> p, vector<char> q) {
	for (int i = 0; i < p.size(); i++)
		RDHelper(R, p[i]);
}

//PSNR
double psnr(Mat f, Mat R, int M, int N) {
	double mid = 0;;
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++) {
			mid += pow(f.at<uchar>(i, j) - R.at<uchar>(i, j), 2);
		}
	return 10 * log10f(pow(255., 2) * M * N / mid);
}

//BPP
double bbp(vector<Color> p, int M, int N, vector<char> q) {
	int a1 = 0, a2 = 0, a3 = 0;
	for (int i = 0; i < p.size(); i++) {
		if (p[i].getGrey()[0] != -1 && p[i].getGrey()[3] != -1) a1++;
		else if (p[i].getGrey()[1] == -1 && p[i].getGrey()[2] == -1) a3++;
		else  a2++;
	}
	double BPPValue = (q.size() + 16 * a1 + 8 * a2 + 4.0 * a3) / (M * N);
	return BPPValue;
}

void segmentDisplay(Mat& display, vector<Color> p) {
	for (int i = 0; i < p.size(); i++) {
		rectangle(display, p[i].getLt(), p[i].getRb(), Scalar(0), 1);
	}
}

