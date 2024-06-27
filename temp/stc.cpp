#include "stc.h"

stc::stc() :method() {}

stc::stc(method* p) :method() {
	origImg = (p->origImg).clone();
	methodId = p->methodId;
	h = p->h;
	w = p->w;
	xigema = p->xigema;
	thresU_ = p->thresU_;
	thresVar_ = p->thresVar_;
	num_ = p->num_;
}

int reg_num = 0;
int bitnum = 0;
double thresU = 0;
double thresVar = 0;
int cur_block = -1;
int nmb = 0;

void stc::process()
{

	reg_num = 0;
	bitnum = 0;
	thresU = 0;
	thresVar = 0;
	cur_block = -1;
	nmb = 0;


	time_t start, end, time, converge_start, converge_end, converge_time;

	Mat imggest = Mat::zeros(origImg.size(), origImg.type());

	int num = num_;
	int M = origImg.rows;
	int N = origImg.cols;

	vector<doubleCoordinate> C;
	vector<colorListStandard> P;
	vector<char> Q;
	treeNode* root = new treeNode;
	InitialNode(root);
	thresU = thresU_;
	thresVar = thresVar_;

	start = clock();
	//cout << "building tree" << endl;
	BuildTreeT(origImg, root, P, C, num, xigema, 0, 0, M - 1, N - 1);
	//cout << "making Q" << endl;
	LevelOrder(Q, root);

	end = clock();
	time = end - start;
	enTime = time;
	blocknum = P.size();
	BPP = bbp(P, M, N, Q);
	CR = 8.0 / BPP;
	converge_start = clock();
	Region** all_region = new Region * [P.size()];

	Segment* UpperLeft = new Segment;
	Segment* Upper = new Segment;

	UpperLeft->Length = M;
	UpperLeft->ActiveELink = NULL;
	UpperLeft->PreLink = NULL;
	UpperLeft->SucLink = Upper;

	Upper->Length = M;
	Upper->ActiveELink = NULL;
	Upper->PreLink = UpperLeft;
	Upper->SucLink = NULL;

	Segment* UpperRight = NULL;
	Segment* PreLowerLeft = NULL;

	num = num_;
	Region_Segm(UpperLeft, UpperRight, PreLowerLeft, 0, 0, M, N, all_region, P, C, Q, num);

	converge_end = clock();
	converge_time = converge_end - converge_start;
	deTime = converge_time;	

	Region** pixel_region = new Region * [M * N];

	Mat seg = Mat::zeros(origImg.size(), origImg.type());
	for (unsigned int i = 0; i < P.size(); i++) {
		int x1 = C[i].x1, x2 = C[i].x2;
		int y1 = C[i].y1, y2 = C[i].y2;

		for (int y = y1; y <= y2; y++) {
			uchar* ptrsketch = seg.ptr<uchar>(y);

			for (int x = x1; x <= x2; x++) {
				ptrsketch[x] = FindParent(all_region[i])->Mean;
				pixel_region[y * N + x] = FindParent(all_region[i]);
			}
		}
	}

	Mat seg_line = Mat::zeros(origImg.size(), origImg.type());
	for (int y = 0; y < M; y++) {
		uchar* ptrsketch = seg_line.ptr<uchar>(y);

		for (int x = 0; x < N; x++) {
			if (x == N - 1 || y == M - 1) {
				ptrsketch[x] = 0;
				break;
			}
			ptrsketch[x] = (pixel_region[y * N + x] != pixel_region[y * N + x + 1] || pixel_region[y * N + x] != pixel_region[(y + 1) * N + x]) ? 0 : 255;
		}
	}

	for (int y = 0; y < M; y++) {
		uchar* ptrsketch = seg_line.ptr<uchar>(y);

		for (int x = 0; x < N; x++) {
			if ((y == 0) || (y == M - 1))
				ptrsketch[x] = 0;
			else if ((x == 0))
				ptrsketch[x] = 0;
		}
	}

	segImg = seg_line.clone();
	MakeImggest(imggest, P, C, all_region);
	PSNR = psnr(origImg, imggest);
	compressedImg = imggest.clone();
	delete[] all_region;
	delete[] pixel_region;
}