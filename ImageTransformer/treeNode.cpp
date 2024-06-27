#include "treeNode.h"

void InitialNode(treeNode*& a)//初始化树结点
{
	a->parent = NULL; a->nwchild = NULL; a->nechild = NULL; a->confirm = '0';
}

void InsertNode(treeNode*& root)//插入孩子结点
{
	root->nwchild = new treeNode; InitialNode(root->nwchild); root->nwchild->parent = root;
	root->nechild = new treeNode; InitialNode(root->nechild); root->nechild->parent = root;
}

void LevelOrder(vector<char>& Q, treeNode* root)//层次遍历
{
	Q.push_back(root->confirm);
	if (root->nwchild != NULL)
	{
		LevelOrder(Q, root->nwchild);
	}
	if (root->nechild != NULL)
	{
		LevelOrder(Q, root->nechild);
	}
}

void BuildTreeT(Mat& img1, treeNode*& root, vector<colorListStandard>& P, vector<doubleCoordinate>& C, int& num, double epsilon, int x1, int y1, int x2, int y2)//分割原图.建立矩阵R.颜色表P
{
	//uchar* ptr = img1.data;
	colorListStandard temp;
	doubleCoordinate tempc;
	InsertNode(root);

	if (root->nwchild == NULL || root->nechild == NULL) cout << "allocation failed" << endl;//递归建立二叉树结构

	if (num % 2 == 1)  //num不为2的整数倍，水平分隔
	{
		num = 0;

		if (y1 == y2)
		{
			root->confirm = '1';
			delete(root->nechild);
			root->nechild = NULL;
			delete(root->nwchild);
			root->nwchild = NULL;

			temp.g1 = img1.at<uchar>(y1, x1);
			temp.g2 = img1.at<uchar>(y1, x2);
			temp.g3 = img1.at<uchar>(y2, x1);
			temp.g4 = img1.at<uchar>(y2, x2);

			P.push_back(temp);
			tempc.x1 = x1; tempc.x2 = x2; tempc.y1 = y1; tempc.y2 = y2;
			C.push_back(tempc);
			return;
		}

		if (JudgeSameBlock(epsilon, img1, x1, y1, x2, (y1 + y2 - 1) / 2))   //判断上半部分是否为同类块:nwchild
		{
			root->nwchild->confirm = '1';

			int middle_y = (y1 + y2 - 1) / 2;
			temp.g1 = img1.at<uchar>(y1, x1);
			temp.g2 = img1.at<uchar>(y1, x2);
			temp.g3 = img1.at<uchar>(middle_y, x1);
			temp.g4 = img1.at<uchar>(middle_y, x2);

			P.push_back(temp);
			tempc.x1 = x1; tempc.x2 = x2; tempc.y1 = y1; tempc.y2 = (y1 + y2 - 1) / 2;
			C.push_back(tempc);


		}
		else
		{
			BuildTreeT(img1, root->nwchild, P, C, num, epsilon, x1, y1, x2, (y1 + y2 - 1) / 2);
		}//若非同类块则递归调用

		num = 0;

		if (JudgeSameBlock(epsilon, img1, x1, (y1 + y2 + 1) / 2, x2, y2))    //判断下半部分:nechild
		{
			root->nechild->confirm = '1';

			int middle_y = (y1 + y2 + 1) / 2;
			temp.g1 = img1.at<uchar>(middle_y, x1);
			temp.g2 = img1.at<uchar>(middle_y, x2);
			temp.g3 = img1.at<uchar>(y2, x1);
			temp.g4 = img1.at<uchar>(y2, x2);

			P.push_back(temp);
			tempc.x1 = x1; tempc.x2 = x2; tempc.y1 = (y1 + y2 + 1) / 2; tempc.y2 = y2;
			C.push_back(tempc);
		}
		else
		{
			BuildTreeT(img1, root->nechild, P, C, num, epsilon, x1, (y1 + y2 + 1) / 2, x2, y2);
		}//递归调用

	}

	else if (num % 2 == 0)  //num是2的整数倍，垂直分隔
	{
		num = 1;

		if (x1 == x2)
		{
			root->confirm = '1';
			delete(root->nechild);
			root->nechild = NULL;
			delete(root->nwchild);
			root->nwchild = NULL;

			int middle_y = (y1 + y2 + 1) / 2;
			temp.g1 = img1.at<uchar>(middle_y, x1);
			temp.g2 = img1.at<uchar>(middle_y, x2);
			temp.g3 = img1.at<uchar>(y2, x1);
			temp.g4 = img1.at<uchar>(y2, x2);

			P.push_back(temp);
			tempc.x1 = x1; tempc.x2 = x2; tempc.y1 = y1; tempc.y2 = y2;
			C.push_back(tempc);
			return;
		}


		if (JudgeSameBlock(epsilon, img1, x1, y1, (x1 + x2 - 1) / 2, y2))  //判断左半部分:nwchild
		{
			root->nwchild->confirm = '1';

			int middle_x = (x1 + x2 - 1) / 2;
			temp.g1 = img1.at<uchar>(y1, x1);
			temp.g2 = img1.at<uchar>(y1, middle_x);
			temp.g3 = img1.at<uchar>(y2, x1);
			temp.g4 = img1.at<uchar>(y2, middle_x);

			P.push_back(temp);
			tempc.x1 = x1; tempc.x2 = (x1 + x2 - 1) / 2; tempc.y1 = y1; tempc.y2 = y2;
			C.push_back(tempc);
		}
		else { BuildTreeT(img1, root->nwchild, P, C, num, epsilon, x1, y1, (x1 + x2 - 1) / 2, y2); }//递归调用

		num = 1;


		if (JudgeSameBlock(epsilon, img1, (x1 + x2 + 1) / 2, y1, x2, y2))   //判断右半部分:nechild
		{
			root->nechild->confirm = '1';

			int middle_x = (x1 + x2 + 1) / 2;
			temp.g1 = img1.at<uchar>(y1, middle_x);
			temp.g2 = img1.at<uchar>(y1, x2);
			temp.g3 = img1.at<uchar>(y2, middle_x);
			temp.g4 = img1.at<uchar>(y2, x2);

			P.push_back(temp);
			tempc.x1 = (x1 + x2 + 1) / 2; tempc.x2 = x2; tempc.y1 = y1; tempc.y2 = y2;
			C.push_back(tempc);
		}
		else
		{
			BuildTreeT(img1, root->nechild, P, C, num, epsilon, (x1 + x2 + 1) / 2, y1, x2, y2);
		}//递归调用
	}
	return;

}

void Region_Segm(Segment* UpperLeft, Segment*& UpperRight, Segment*& PreLowerLeft, int Xleft, int Yupper, int Length, int Width, Region** all_region, vector<colorListStandard>& P, vector<doubleCoordinate>& C, vector<char> Q, int& num)
{
	Segment* UR = NULL;
	Segment* PLL = NULL;
	Segment* DUMMY = NULL;
	char bit;

	if (UpperLeft->Length > Width)
	{
		SPLIT(UpperLeft, Width, 0);
	}

	if (UpperLeft->SucLink != NULL && UpperLeft->SucLink->Length > Length)

	{
		SPLIT(UpperLeft->SucLink, Length, 1);
	}

	bit = Get_bit(Q);

	if (bit == '0')
	{
		if (num % 2 == 0) //垂直
		{
			num = 1;
			Region_Segm(UpperLeft, UR, PreLowerLeft, Xleft, Yupper, Length / 2, Width, all_region, P, C, Q, num);
			num = 1;
			Region_Segm(UR, UpperRight, DUMMY, Xleft + Length / 2, Yupper, Length - Length / 2, Width, all_region, P, C, Q, num);
		}
		else //水平
		{
			num = 0;
			Region_Segm(UpperLeft, UpperRight, PLL, Xleft, Yupper, Length, Width / 2, all_region, P, C, Q, num);
			num = 0;
			Region_Segm(PLL, UR, PreLowerLeft, Xleft, Yupper + Width / 2, Length, Width - Width / 2, all_region, P, C, Q, num);
		}
	}
	else
	{
		cur_block++;
		if (Xleft == C[cur_block].x1 && Yupper == C[cur_block].y1)
		{
			if ((Xleft + Length - 1) != C[cur_block].x2 && (Yupper + Width - 1) != C[cur_block].y2)
			{
				cout << cur_block << endl;
			}
		}
		Leaf_Operation(UpperLeft, UpperRight, PreLowerLeft, Xleft, Yupper, Length, Width, P, C, all_region);
	}
}

void Leaf_Operation(Segment* UpperLeft, Segment*& UpperRight, Segment*& PreLowerLeft, int Xleft, int Yupper, int Length, int Width, vector<colorListStandard>& P, vector<doubleCoordinate>& C, Region** all_region)
{
	//find lower left = start_segment;and find prelowerleft
	//int cur_block = bitnum - 1;
	int side_length = Width;
	Segment* start_segment = UpperLeft;
	while ((side_length = side_length - start_segment->Length) > 0)
		start_segment = start_segment->PreLink;

	PreLowerLeft = start_segment->PreLink;
	side_length = 0;

	//allocate new region,allocate new edge
	Region* cur_region = new Region;
	all_region[cur_block] = cur_region;

	Edge* active_right = new Edge;
	Edge* inactive = new Edge;
	Edge* active_down = new Edge;

	active_right->SucLink = active_down;
	active_right->PreLink = inactive;
	active_right->Reg = cur_region;


	active_down->SucLink = inactive;
	active_down->PreLink = active_right;
	active_down->Reg = cur_region;


	inactive->SucLink = active_right;
	inactive->PreLink = active_down;
	inactive->Reg = NULL;



	cur_region->Father = NULL;//
	cur_region->Count = 0;
	cur_region->EdgeLink = inactive;//
	cur_region->Mean = Ubi(P[cur_block], C[cur_block]);//
	cur_region->Var = Varbi(P[cur_block], C[cur_block]);//
	//cur_region->Size = Size*Size;
	cur_region->Size = Length * Width;
	cur_region->SegmentCount = 2;

	reg_num++;
	//遍历每个SEGMENT
	while (true)
	{
		if (start_segment->ActiveELink != NULL && start_segment->ActiveELink->Reg != NULL)
		{
			Region* neighbour_region = start_segment->ActiveELink->Reg;
			Region* neighbour_father = FindParent(neighbour_region);
			Region* cur_father = FindParent(cur_region);
			//同一区域
			if (neighbour_father == cur_father)
			{
			}
			//不同区域
			else
			{
				double varnew = Varc(cur_father->Size, neighbour_father->Size, cur_father->Var, neighbour_father->Var, cur_father->Mean, neighbour_father->Mean);
				//可以合并
				if (neighbour_father->Mean - cur_father->Mean >= -thresU && neighbour_father->Mean - cur_father->Mean <= thresU && varnew <= thresVar)
				{

					neighbour_father->Mean = Uc(cur_father->Size, neighbour_father->Size, cur_father->Mean, neighbour_father->Mean);
					neighbour_father->Size += cur_father->Size;
					neighbour_father->Var = varnew;
					cur_father->Father = neighbour_father;
					reg_num--;
				}
			}
		}

		if ((side_length = side_length + start_segment->Length) == (Length + Width))
			break;
		else
		{
			start_segment = start_segment->SucLink;
		}

	}

	//此块遍历完成,重新构造waveform,Upperright

	if (start_segment != NULL && start_segment->SucLink != NULL)
	{
		start_segment = start_segment->SucLink;
	}
	Segment* downseg = new Segment;
	Segment* rightseg = new Segment;
	downseg->ActiveELink = active_down;
	downseg->Length = Length;
	downseg->PreLink = PreLowerLeft;
	if (PreLowerLeft != NULL)
		PreLowerLeft->SucLink = downseg;
	downseg->SucLink = rightseg;

	rightseg->ActiveELink = active_right;
	rightseg->Length = Width;
	rightseg->PreLink = downseg;
	rightseg->SucLink = start_segment;
	if (start_segment != NULL)
		start_segment->PreLink = rightseg;

	UpperRight = rightseg;
}

void SPLIT(Segment* UpperLeft, int Size, int direction)
{
	Segment* late_half = new Segment;
	late_half->ActiveELink = UpperLeft->ActiveELink;
	if (direction == 0)
	{
		late_half->Length = (UpperLeft->Length + 1) / 2;
		UpperLeft->Length /= 2;
	}
	else
	{
		late_half->Length = (UpperLeft->Length) / 2;
		UpperLeft->Length = (UpperLeft->Length + 1) / 2;
	}
	late_half->SucLink = UpperLeft;
	late_half->PreLink = UpperLeft->PreLink;
	UpperLeft->PreLink = late_half;
	if (late_half->PreLink != NULL)
		late_half->PreLink->SucLink = late_half;
}

Region* FindParent(Region* p)
{
	if (p->Father != NULL)
	{
		p->Father = FindParent(p->Father);
		return p->Father;
	}
	else return p;
}

void MakeImggest(Mat& imggest, vector<colorListStandard>& P, vector<doubleCoordinate> C, Region** all_region)//建立gest图像
{
	uchar g1 = 0, g2 = 0, g3 = 0, g4 = 0;
	for (size_t i = 0; i < P.size(); i++)
	{
		g1 = P[i].g1; g2 = P[i].g2;
		g3 = P[i].g3; g4 = P[i].g4;
		int x1 = C[i].x1, x2 = C[i].x2;
		int y1 = C[i].y1, y2 = C[i].y2;

		if (x1 == x2 && y1 == y2)
		{
			// 单点，直接赋值
			imggest.at<uchar>(y1, x1) = g1;
		}
		else if (x1 == x2)
		{
			// 垂直线，使用线性插值
			for (int y = y1; y <= y2; y++)
			{
				double i1 = (double)(y - y1) / (double)(y2 - y1);
				imggest.at<uchar>(y, x1) = saturate_cast<uchar>(g1 + (g2 - g1) * i1);
			}
		}
		else if (y1 == y2)
		{
			// 水平线，使用线性插值
			for (int x = x1; x <= x2; x++)
			{
				double i2 = (double)(x - x1) / (double)(x2 - x1);
				imggest.at<uchar>(y1, x) = saturate_cast<uchar>(g1 + (g2 - g1) * i2);
			}
		}
		else
		{
			// 矩形区域，双线性插值
			for (int y = y1; y <= y2; y++) {
				uchar* rowPtr = imggest.ptr<uchar>(y);
				for (int x = x1; x <= x2; x++) {
					double i1 = (double)(y - y1) / (double)(y2 - y1);
					double i2 = (double)(x - x1) / (double)(x2 - x1);
					double g5 = g1 + (g2 - g1) * i2;
					double g6 = g3 + (g4 - g3) * i2;
					rowPtr[x] = cv::saturate_cast<uchar>(g5 + (g6 - g5) * i1);
				}
			}
		}

	}
}

double psnr(Mat& img1, Mat& imggest) {
	int M = img1.rows;
	int N = img1.cols;

	double temp = 0.0;
	double psnr = 0;

	for (int y = 0; y < M; y++) {
		uchar* rowPtr1 = img1.ptr<uchar>(y);
		uchar* rowPtr2 = imggest.ptr<uchar>(y);
		for (int x = 0; x < N; x++) {
			int val1 = rowPtr1[x];
			int val2 = rowPtr2[x];
			// 计算误差的平方
			temp += std::pow((double)(val1 - val2), 2);
		}
	}

	if (temp == 0) { // Avoid division by zero
		return std::numeric_limits<double>::infinity(); // Infinite PSNR means no difference
	}

	psnr = 10 * log10((255.0 * 255 * M * N) / temp);
	return psnr;
}

bool JudgeSameBlock(double epsilon, Mat& img1, int x1, int y1, int x2, int y2)//判断矩形是否为同类块
{
	// 计算区域四个角的像素值
	uchar g1 = img1.at<uchar>(y1, x1);
	uchar g2 = img1.at<uchar>(y1, x2);
	uchar g3 = img1.at<uchar>(y2, x1);
	uchar g4 = img1.at<uchar>(y2, x2);

	for (int y = y1; y <= y2; y++)
	{
		uchar* ptr = img1.ptr<uchar>(y);

		for (int x = x1; x <= x2; x++)
		{
			uchar g = ptr[x];
			double i1 = (double)(y - y1) / (y2 - y1);
			double i2 = (double)(x - x1) / (x2 - x1);
			double g5 = g1 + (g2 - g1) * i2;
			double g6 = g3 + (g4 - g3) * i2;
			double gest = g5 + (g6 - g5) * i1;

			// 如果当前像素与插值的差值超过epsilon，则不是同类块
			if (abs(g - gest) > epsilon)
			{
				return false;
			}
		}
	}
	return true;
}

double bbp(vector<colorListStandard>& P, int M, int N, vector<char>& Q)  //计算BPP
{
	double b = (Q.size() + 32.0 * P.size()) / (M * N);
	return b;
}

