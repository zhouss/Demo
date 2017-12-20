
// FluidsimView.h : CFluidsimView 类的接口
//

#pragma once


#include "cv.h"
#include "highgui.h"
#include "MainFrm.h"
#include <math.h>
#include "gl.h"
#include "glu.h"
#include "glaux.h"
#include "glut.h"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <vector>
#include <ctime>

#include <noise\noise.h>
using namespace noise;



#define NUMBER_OF_TRAINING_SAMPLES 1000
#define ATTRIBUTES_PER_SAMPLE 3
#define NUMBER_OF_TESTING_SAMPLES 1000
#define NUMBER_OF_CLASSES 3




#include <ml.h>		  // opencv machine learning include file
#include <stdio.h>    


//gdal库
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "ogr_srs_api.h"
#include <vector>

using namespace cv;


#define PI 3.1415926f

const int patch_width = 5;
const int patch_height = 5;

struct point2D{
	float x,y;
};

struct point3D{
	float x,y,z;
};

typedef struct
{
	float x,y;
	double z;
	int R,G,B;
	float u,v;

	//zhouss
	float H;

 }GeoPoint;

struct MapPoint
{
	int x,y;
	float dist;
	float newH;
};

typedef struct
{
	int X;
	int Y;
}point;


typedef struct
{
	/*! 南北方向分辨率 */
	double nsres;
	/*! 东西方向分辨率 */
	double ewres;
	/*! 方位角的正弦值 */
	double sin_altRadians;
	/*! 方位角的余弦值乘以Z缩放比 */
	double cos_altRadians_mul_z_scale_factor;
	/*! 太阳高度角 */
	double azRadians;
	/*! z缩放比平方 */
	double square_z_scale_factor;
} GDALHillshadeAlgData;



struct patch
{
	int patch_X;
	int patch_Y;
	float average_height;   //patch内平均高程
	float patch_Variance;   //patch的方差
	int patch_relief;       //patch的地形起伏度
	int patch_MaxH;         //
	int patch_MinH;         //
	int label;  //0; 1:mountain,    2:water
	//int label_mountain;
	//int label_water;
	int IsHandled;
	int flag;               //统计在新的尺度下的聚类

	patch()
		:IsHandled(0)
	{
		
	}

};

struct gdal_info 
{
	GDALDataType DATAType;
	char *projection_info;
	double geos[6];

};

struct patch_mountain {
	int X;
	int Y;
	int label;
};

class CFluidsimView : public CView
{
protected: // 仅从序列化创建
	CFluidsimView();
	DECLARE_DYNCREATE(CFluidsimView)

// 特性
public:
	CFluidsimDoc* GetDocument() const;

// 操作
public:
	//MyFunction
	//2D Velocity
	void DrawArrow(IplImage *image, GeoPoint *P3D, const char *window_name);

	//Draw Scene
	void DrawScene();
	void Draw3D();
	void SetViewMode();
	BOOL InitializeOpenGL(CDC* pDC);
	BOOL bSetupPixelFormat();
	void SetLogicalPalette();
	point2D GetGLValue(point2D pt);

	void Relight2D(float *H,IplImage *newImage);
	void Relight3D(float *H,CvScalar *color);

	void insertion_sort(struct MapPoint *Point);//从小到大
	void insertion_sort(double *Z);

public: //属性
	//OPENGL 显示
	CDC* m_pDC;						// Windows设备描述表
	HGLRC m_hRC;					// OpenGL渲染描述表
	CRect m_ClientRect;				// 客户区的大小
	double m_dAspectRatio;			// 窗口的比例
	HPALETTE	m_hPalette;			//调色板
	
	point3D SceneAngle;             //视图场景旋转角度
	point3D ScenePos;               //视图场景平移位置
	double  SceneZoom;              //视图比例系数
	double  SceneDepth;             //视图区深度
	CPoint  MouseDownPoint;         //鼠标落下点
	point3D view_base_point;        //鼠标按下基点GL坐标系

	//文件
	CString path;
	IplImage* image1; //彩色图像
	IplImage* image2; //彩色图像	
	IplImage* image3; //彩色图像

	IplImage* gray1;  //灰度图像
	IplImage* gray2;  //灰度图像
	IplImage* gray3;  //灰度图像



	//3D场景信息
	int nHeight,nWidth; //图像尺度


	double *Nz; 
	float *Z1,*Z2,*Z3;
	GeoPoint *P3D1,*P3D2,*P3D3;
	float *u,*v,*H;
	unsigned int iframe;
	int showflag;
	double *frame_h;
	float Zmin,Zmax;	

	//光照计算参数
	float KIa_R,KIa_G,KIa_B;
	float KIs_R,KIs_G,KIs_B;
	float KId_R,KId_G,KId_B;	
	float a_New;

	int min_grey;
    int max_grey;
	int *flagPhong;

	//读取数据
	GeoPoint **R3D;
	int frameNo;

	int	nWidthNew;
	int nHeightNew;
	int *heightflag;
	GeoPoint *sketchpoint;
	MapPoint *SketchPoints;
	double minZ,maxZ;
	int numZ;
	double *sortZ;
	GeoPoint *D3DNew;
	GLfloat *D;
	IplImage *sketchimg;
	GLfloat gHillHeightMap[400][300];
	GLfloat gHillHeightMapNew[400][300];
	void generateFractalHill(GLint x1,GLint y1,GLint x2,GLint y2,GLint depth);
	GLfloat funNum(GLfloat miu,GLfloat sigma);

	//zhouss
	int GetDemMaxHeight(int W, int H, unsigned short *D);
	int GetDemMinHeight(int W, int H, unsigned short *D, int Max);
	int GetDemMaxHeight(int W, int H, short *D);//重载
	int GetDemMinHeight(int W, int H, short *D, int Max);//重载
	unsigned short *data_UInt16;    //unsigned short
	short *data_Int16;
	bool withTexture;
	int maxheight;
	int minheight;
	patch_mountain *P_moun;
	int *regionflag;
	std::vector<std::vector<point>> mouintainPatch_cluster;
	std::vector<point> centerPatchPos;

	std::vector<point> high_freqPatch;
	std::vector<point> middle_freqPatch;
	std::vector<point> low_freqPatch;
	std::vector<point> pos_skeleton;


	
	patch *Patch;
	patch *NewPatch;
	void CalPatchAverageHeight(GeoPoint *Point);
	void CalPatchVariance(GeoPoint *Point);
	void CalPatchRelief(GeoPoint *Point);
	void KMeans( patch_mountain *samples,int sample_count,int &cluster_count,float threshold);//KMeans( patch *sample,int sample_count,int &cluster_count,int *labels,float threshold);
	int patch_size;
	void WritePatchData2Txt(char *filename, int patch_size, patch* p, int flag);
	void KmeansCluster(patch *P);
	void SavePatch(patch *P);
	int read_data_from_csv(const char* filename, Mat data, Mat classes, int n_samples);
	void WriteData2CSV(char *filename, patch *p);
	void TifSavePatch(patch *P);
	void GenerateNewDem();
	gdal_info GDAL_example;
	void TifSaveGenerateNewDem(char *,int ,int ,GDALDataType ,char * , short *);
	void RegionGrow(int nSeedX,int nSeedY,int sign, std::vector<point> &pointer);
	bool ChooseSelectedPatchIntoNewScene(char *filename, int width, int height, short *Data, point position,int rrr);
	void dfs(int x,int y);
	Mat Enropty(Mat s, int Row, int Column);
	Mat Enropty_Height();
	float GDALHillshadeAlg (float* afWin, float fDstNoDataValue, void* pData);
	void* GDALCreateHillshadeData(double* adfGeoTransform,double z,double scale,double alt,double az);
	



// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CFluidsimView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpen1();
	
	afx_msg void OnGeometry();
	afx_msg void OnCalculatevel();
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCalDensityFi();
		
	afx_msg void OnOpenimage();
	afx_msg void OnReadfile();
	afx_msg void OnShow2D3D();
	afx_msg void OnReadTif();
	afx_msg void OnSketchread();
	afx_msg void OnInitscene();
	afx_msg void OnInitScene();
	afx_msg void OnReadTexture();
	afx_msg void OnCalPatchAverageHeight();
	afx_msg void OnCalPatchVariance();
	afx_msg void OnCalPatchRelief();
	afx_msg void OnKmeansCluster();
	afx_msg void OnWriteData2Txt();
	afx_msg void OnSavePatch();
	afx_msg void On32821();
	afx_msg void OnRandomTree();
	afx_msg void OnWriteData2CSV();
	afx_msg void OnReadUserSketch();
	afx_msg void OnTifSavePatch();
	afx_msg void OnDrawSkeleton();
	afx_msg void OnNewDemSythesis();
	afx_msg void OnMountainPatchCluster();
	afx_msg void OnCalAllFeatures();
	afx_msg void OnCalMountainPatchFeatures();
	afx_msg void On32833();
	afx_msg void On32834();
	afx_msg void OnSaveMountainCluster();
	afx_msg void OnSaveMountainClusterPatch();
	afx_msg void OnFindContours();
	afx_msg void OnNewCluster();
	afx_msg void Onsize100();
	afx_msg void On32840();
	afx_msg void OnHeCheng1();
	afx_msg void On32842();
	afx_msg void On32843();
};

#ifndef _DEBUG  // FluidsimView.cpp 中的调试版本
inline CFluidsimDoc* CFluidsimView::GetDocument() const
   { return reinterpret_cast<CFluidsimDoc*>(m_pDocument); }
#endif

