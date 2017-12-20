#include "stdafx.h"

#ifndef SHARED_HANDLERS
#include "Fluidsim.h"
#endif

#include "FluidsimDoc.h"
#include "FluidsimView.h"
#include "iostream"
#include "fstream"
#include "math.h"
#include "cv.h"
#include "highgui.h"
#include "stdlib.h"

#include "iostream"
#include "fstream"
#include <vector>
#include <map>


using namespace std;

#include "skeletonization.hpp"

static void on_Mouse(int event, int x, int y, int flags, void*);


string Int_to_String(int n)
{
	ostringstream stream;
	stream<<n;  //n为int类型
	return stream.str();
}

float VariancePredict(float sampleData)
{     
	float trainingData[29]={12339.530, 13607.689, 3781.9065, 6870.8091, 686.74377,  
                                16306.232,  5777.2671, 1664.4569, 588.57202, 15876.461, 5792.9067,  
                                7949.7935, 1626.4124, 15948.693, 9803.2861, 19332.822,  
                                7941.9150, 5683.3105, 31013.320, 17742.313, 20165.893,  
                                35727.395, 19329.584, 9629.5901, 15451.046, 25006.967,  
                                36184.969, 57125.000, 39090.117};

	CvMat trainingDataCvMat = cvMat( 29, 1, CV_32FC1, trainingData );
  
    float responses[29] = { 73641, 214483, 9991, 17319, 71634, 191656, 98113, 153605, 76809,  
                        56338, 250953, 204772, 107444, 64690, 98322, 232246,  
                        180624, 111225, 104240, 127580, 93548, 96377, 190088,   
                        164743, 79712, 83561, 75329, 121652, 8912};
    CvMat responsesCvMat = cvMat( 29, 1, CV_32FC1, responses );  
	//float priors[] = {0.95,0.05}; 
    //CvRTParams params= CvRTParams(10, 2, 0, false,16, priors, true, 0, 100, 0, CV_TERMCRIT_ITER ); 
	CvRTParams params= CvRTParams(10, 2, 0, false,16, 0, true, 0, 100, 0, CV_TERMCRIT_ITER );
      
    CvRTrees rtrees;
	
	Mat var_type = Mat(1 + 1, 1, CV_8UC1 );
    var_type.setTo(Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical
    rtrees.train(&trainingDataCvMat, CV_ROW_SAMPLE, &responsesCvMat,Mat(), Mat(), var_type, Mat(),params);  
    //float sampleData[2]={3781.9065, 2586.7231};  //16589.530, 2000.0   39090.117, 1814.3832
	//float sampleData=29090.117;
    Mat sampleMat(1, 1, CV_32FC1, sampleData);  
    float r = rtrees.predict(sampleMat);  
   
    return r;
 }  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFluidsimView

IMPLEMENT_DYNCREATE(CFluidsimView, CView)

BEGIN_MESSAGE_MAP(CFluidsimView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFluidsimView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OPEN1, &CFluidsimView::OnOpen1)

	ON_COMMAND(ID_GEOMETRY, &CFluidsimView::OnGeometry)
	ON_COMMAND(ID_CALCULATEVEL, &CFluidsimView::OnCalculatevel)
	ON_WM_DRAWITEM()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_CAL_DENSITY_FI, &CFluidsimView::OnCalDensityFi)
			
	ON_COMMAND(ID_OPENIMAGR, &CFluidsimView::OnOpenimage)
	ON_COMMAND(ID_READFILE, &CFluidsimView::OnReadfile)
	ON_COMMAND(ID_SHOW2D3D, &CFluidsimView::OnShow2D3D)
	ON_COMMAND(ID_READTIF , &CFluidsimView::OnReadTif)
	ON_COMMAND(ID_SKETCHREAD, &CFluidsimView::OnSketchread)
	ON_COMMAND(ID_INITSCENE, &CFluidsimView::OnInitscene)
	ON_COMMAND(ID_InitScene, &CFluidsimView::OnInitScene)
	ON_COMMAND(ID_READTEXTURE, &CFluidsimView::OnReadTexture)
	ON_COMMAND(ID_CALPATCHAVERAGEHEIGHT, &CFluidsimView::OnCalPatchAverageHeight)
	ON_COMMAND(ID_CALPATCHVARIANCE, &CFluidsimView::OnCalPatchVariance)
	ON_COMMAND(ID_CALPATCHRELIEF, &CFluidsimView::OnCalPatchRelief)
	ON_COMMAND(ID_KMEANSCLUSTER, &CFluidsimView::OnKmeansCluster)
	ON_COMMAND(ID_WRITEDATA2TXT, &CFluidsimView::OnWriteData2Txt)
	ON_COMMAND(ID_32819, &CFluidsimView::OnSavePatch)
	ON_COMMAND(ID_32821, &CFluidsimView::On32821)
	ON_COMMAND(ID_RANDOMTREE, &CFluidsimView::OnRandomTree)
	ON_COMMAND(ID_32823, &CFluidsimView::OnWriteData2CSV)
	ON_COMMAND(ID_REAdUSERSKETCH, &CFluidsimView::OnReadUserSketch)
	ON_COMMAND(ID_32826, &CFluidsimView::OnTifSavePatch)
	ON_COMMAND(ID_32827, &CFluidsimView::OnDrawSkeleton)
	ON_COMMAND(ID_32828, &CFluidsimView::OnNewDemSythesis)
	ON_COMMAND(ID_32830, &CFluidsimView::OnMountainPatchCluster)
	ON_COMMAND(ID_32831, &CFluidsimView::OnCalAllFeatures)
	ON_COMMAND(ID_32832, &CFluidsimView::OnCalMountainPatchFeatures)
	ON_COMMAND(ID_32833, &CFluidsimView::On32833)
	ON_COMMAND(ID_32834, &CFluidsimView::On32834)
	ON_COMMAND(ID_32835, &CFluidsimView::OnSaveMountainCluster)
	ON_COMMAND(ID_32836, &CFluidsimView::OnSaveMountainClusterPatch)
	ON_COMMAND(ID_32837, &CFluidsimView::OnFindContours)
	ON_COMMAND(ID_32838, &CFluidsimView::OnNewCluster)
	ON_COMMAND(ID_32839, &CFluidsimView::Onsize100)
	ON_COMMAND(ID_32840, &CFluidsimView::On32840)
	ON_COMMAND(ID_32841, &CFluidsimView::OnHeCheng1)
	ON_COMMAND(ID_32842, &CFluidsimView::On32842)
	ON_COMMAND(ID_32843, &CFluidsimView::On32843)
END_MESSAGE_MAP()

// CFluidsimView 构造/析构

CFluidsimView::CFluidsimView()
{
	ScenePos.x=0.0;
	ScenePos.y=0.0;
	ScenePos.z=0.0;
	
	SceneAngle.x=0.0;
	SceneAngle.y=0.0;
	SceneAngle.z=0.0;
	
	SceneZoom=3.0;
	SceneDepth=300.0;

	iframe =0;

	showflag =0;
	
}

CFluidsimView::~CFluidsimView()
{

	cvDestroyWindow("First Image");
	cvDestroyWindow("Second Image");
	cvDestroyWindow("Third Image");
	cvDestroyWindow("Velocity-1th Frame");
	cvDestroyWindow("Velocity-2th Frame");
	cvDestroyWindow("Relighting");
	cvDestroyWindow("Non-Normalized Geometry Relighting");

	image1 = NULL;
	gray1 = NULL;

	image2 = NULL;
	gray2 = NULL;

	image3 = NULL;
	gray3 = NULL;
	
}

BOOL CFluidsimView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CFluidsimView 绘制

void CFluidsimView::OnDraw(CDC* /*pDC*/)
{
	CFluidsimDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//MyCode
	DrawScene();
}
void CFluidsimView::Draw3D()
{	
	int x,y;
	x=0,y=0;

	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f,1.0f,1.0f);
	glPointSize(2.0f);
	//点的形式
	glBegin(GL_POINTS); //GL_POINTS

	if( showflag == 1 )  //显示第一帧   || showflag == 3
	{
		for(y=0;y<nHeight ;y++) //for(y=0;y<nHeight ;y++)
		for(x=0;x<nWidth;x++)   //for(x=0;x<nWidth;x++)
		{
			int pos = y*nHeight+x;//int pos = y*nWidth+x;
		
			
			glColor3f((float)P3D1[pos].R/255,(float)P3D1[pos].G/255,(float)P3D1[pos].B/255);
	    	glVertex3f((float)y/100,(float)x/100,(float)P3D1[pos].z); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);

		}
	}
	/*
	glBegin(GL_TRIANGLES); //三角网格

	if( showflag == 1 )  //显示第一帧   //|| showflag == 3)
	{
		
		for(y=0;y<nHeight-1 ;y++) //for(y=0;y<nHeight ;y++)
		for(x=0;x<nWidth-1;x++)   //for(x=0;x<nWidth;x++)
		{
			int pos = y*nWidth+x;//int pos = y*nWidth+x;
			int pos_right = y*nWidth+(x+1);
			int pos_down = (y+1)*nWidth+x;
			int pos_rdown = (y+1)*nWidth+(x+1);

			glColor3f((float)P3D1[pos].R/255,(float)P3D1[pos].G/255,(float)P3D1[pos].B/255);
			glVertex3f((float)x/150,(float)y/150,(float)P3D1[pos].z); 

			glColor3f((float)P3D1[pos_down].R/255,(float)P3D1[pos_down].G/255,(float)P3D1[pos_down].B/255);
			glVertex3f((float)x/150,(float)(y+1)/150,(float)P3D1[pos_down].z); 

			glColor3f((float)P3D1[pos_right].R/255,(float)P3D1[pos_right].G/255,(float)P3D1[pos_right].B/255);
			glVertex3f((float)(x+1)/150,(float)y/150,(float)P3D1[pos_right].z); 



			glColor3f((float)P3D1[pos_down].R/255,(float)P3D1[pos_down].G/255,(float)P3D1[pos_down].B/255);
			glVertex3f((float)x/150,(float)(y+1)/150,(float)P3D1[pos_down].z); 

			glColor3f((float)P3D1[pos_rdown].R/255,(float)P3D1[pos_rdown].G/255,(float)P3D1[pos_rdown].B/255);
			glVertex3f((float)(x+1)/150,(float)(y+1)/150,(float)P3D1[pos_rdown].z);

			glColor3f((float)P3D1[pos_right].R/255,(float)P3D1[pos_right].G/255,(float)P3D1[pos_right].B/255);
			glVertex3f((float)(x+1)/150,(float)y/150,(float)P3D1[pos_right].z); 

			

			
			//glColor3f((float)P3D1[pos_down].R/255,(float)P3D1[pos_down].G/255,(float)P3D1[pos_down].B/255);
			//glColor3f((float)P3D1[pos_rdown].R/255,(float)P3D1[pos_rdown].G/255,(float)P3D1[pos_rdown].B/255);
	
	    	//glVertex3f((float)x/150,(float)y/150,(float)P3D1[pos].z*1.2); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);
			//glVertex3f((float)x/150,(float)(y+1)/150,(float)P3D1[(y+1)*1000+x].z*1.2); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);
			//glVertex3f((float)(x+1)/150,(float)y/150,(float)P3D1[y*1000+(x+1)].z*1.2); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);
			//glVertex3f((float)x/150,(float)(y+1)/150,(float)P3D1[(y+1)*1000+x].z*1.2); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);
			//glVertex3f((float)(x+1)/150,(float)(y+1)/150,(float)P3D1[(y+1)*1000+(x+1)].z*1.2); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);
			//glVertex3f((float)(x+1)/150,(float)y/150,(float)P3D1[y*1000+(x+1)].z*1.2); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);
			
		}
	}
	*/
	else if(showflag == 2 )
	{
		for(y=0;y<nHeight ;y++) //for(y=0;y<nHeight ;y++)
		for(x=0;x<nWidth;x++)   //for(x=0;x<nWidth;x++)
		{
			int pos = y*nHeight+x;//int pos = y*nWidth+x;
		
			
			glColor3f((float)P3D1[pos].R/255,(float)P3D1[pos].G/255,(float)P3D1[pos].B/255);
	    	glVertex3f((float)y/100,(float)x/100,(float)P3D1[pos].z); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);

		}
		
		
		
		
		
		
		/*原来的
		for(y=0;y<nHeight ;y++)
		for(x=0;x<nWidth;x++)		
		{
			int pos = y*nWidth+x;

			glColor3f((float)P3D1[pos].R/255,(float)P3D1[pos].G/255,(float)P3D1[pos].B/255);
	
	    	glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);  //
		}
		*/
	}
	else if(showflag == 4)
	{
		for(y=0;y<nHeight ;y++)
		for(x=0;x<nWidth;x++)
		{
			int pos = y*nWidth+x;

			glColor3f((float)P3D1[pos].R/255,(float)P3D1[pos].G/255,(float)P3D1[pos].B/255);
	
	    	glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)frame_h[pos]/12);  
		}
	}
	else if(showflag == 5)
	{
		
		for(y=0;y<nHeight ;y++)
		for(x=0;x<nWidth;x++)
		{
			int pos = y*nWidth+x;

			glColor3f((float)R3D[frameNo][pos].R/255,(float)R3D[frameNo][pos].G/255,(float)R3D[frameNo][pos].B/255);
	
	    	glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)R3D[frameNo][pos].z/12);  
		}
	}
	else if(showflag == 6)
	{
		for(y=0;y<nHeight ;y++)
		for(x=0;x<nWidth;x++)		
		{
			int pos = y*nWidth+x;

			glColor3f((float)R3D[iframe][pos].R/255,(float)R3D[iframe][pos].G/255,(float)R3D[iframe][pos].B/255);
	
	    	glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);  
		}
	}
  	glEnd();


	/*
	glBegin(GL_TRIANGLES); //三角网格

	if( showflag == 1 )  //显示第一帧   //|| showflag == 3)
	{
		
		for(y=0;y<nHeight ;y++) //for(y=0;y<nHeight ;y++)
		for(x=0;x<nWidth;x++)   //for(x=0;x<nWidth;x++)
		{
			int pos = y*nWidth+x;//int pos = y*nWidth+x;
			int pos_right = y*nWidth+(x+1);
			int pos_down = (y+1)*nWidth+x;
			int pos_rdown = (y+1)*nWidth+(x+1);

			glColor3f((float)P3D1[pos].R/255,(float)P3D1[pos].G/255,(float)P3D1[pos].B/255);
			glVertex3f((float)x/150,(float)y/150,(float)P3D1[pos].z); 

			glColor3f((float)P3D1[pos_down].R/255,(float)P3D1[pos_down].G/255,(float)P3D1[pos_down].B/255);
			glVertex3f((float)x/150,(float)(y+1)/150,(float)P3D1[pos].z); 

			glColor3f((float)P3D1[pos_right].R/255,(float)P3D1[pos_right].G/255,(float)P3D1[pos_right].B/255);
			glVertex3f((float)(x+1)/150,(float)y/150,(float)P3D1[pos].z); 



			glColor3f((float)P3D1[pos_down].R/255,(float)P3D1[pos_down].G/255,(float)P3D1[pos_down].B/255);
			glVertex3f((float)x/150,(float)(y+1)/150,(float)P3D1[pos].z); 

			glColor3f((float)P3D1[pos_rdown].R/255,(float)P3D1[pos_rdown].G/255,(float)P3D1[pos_rdown].B/255);
			glVertex3f((float)(x+1)/150,(float)(y+1)/150,(float)P3D1[pos].z); 

			glColor3f((float)P3D1[pos_right].R/255,(float)P3D1[pos_right].G/255,(float)P3D1[pos_right].B/255);
			glVertex3f((float)(x+1)/150,(float)y/150,(float)P3D1[pos].z); 

			

			
			//glColor3f((float)P3D1[pos_down].R/255,(float)P3D1[pos_down].G/255,(float)P3D1[pos_down].B/255);
			//glColor3f((float)P3D1[pos_rdown].R/255,(float)P3D1[pos_rdown].G/255,(float)P3D1[pos_rdown].B/255);
	
	    	//glVertex3f((float)x/150,(float)y/150,(float)P3D1[pos].z*1.2); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);
			//glVertex3f((float)x/150,(float)(y+1)/150,(float)P3D1[(y+1)*1000+x].z*1.2); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);
			//glVertex3f((float)(x+1)/150,(float)y/150,(float)P3D1[y*1000+(x+1)].z*1.2); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);
			//glVertex3f((float)x/150,(float)(y+1)/150,(float)P3D1[(y+1)*1000+x].z*1.2); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);
			//glVertex3f((float)(x+1)/150,(float)(y+1)/150,(float)P3D1[(y+1)*1000+(x+1)].z*1.2); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);
			//glVertex3f((float)(x+1)/150,(float)y/150,(float)P3D1[y*1000+(x+1)].z*1.2); //glVertex3f((float)x/70,(float)(nHeight - y)/70,(float)H[pos]/12);
			
		}
	}*/

	
	

	// 绘制三个坐标轴
/*	glBegin(GL_LINES);
	// 绘制红色的x轴
	glColor3f(1.f,0.f,0.f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(5,0.0f,0.0f);
	// 绘制绿色的y轴
	glColor3f(0.f,1.f,0.f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,5,0.0f);
	// 绘制蓝色的z轴
	glColor3f(0.f,0.f,1.f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,0.0f,5);
	glEnd(); */

}
void CFluidsimView::DrawScene()
{
	CMainFrame *pMainfrm=(CMainFrame*)AfxGetMainWnd();
	// 清除颜色缓冲区和深度缓冲区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetViewMode();
	
	if(pMainfrm->m_bShowCoordin)  
		Draw3D();

	if(showflag!=0)
	{
		float tmp = -0.2; //Zmin*0.01; 
		for(int i=-1;i<5;i++)
			{
				for(int j=-1;j<4;j++)
				{
					if(i%2==0)
					{
						if(j%2==0)			
						{
							glBegin(GL_QUADS);
							glColor3f(0.66f,0.66f,0.66f);
							glVertex3f(i+0.5, j+1.5, 0);
							glVertex3f( i+1.5,j+1.5, 0);
							glVertex3f( i+1.5,j+0.5, 0);
							glVertex3f(i+0.5,j+0.5, 0);						
							glEnd();
						}
						else
						{
							glBegin(GL_QUADS);
							glColor3f(0.82f,0.82f,0.82f);
							glVertex3f(i+0.5, j+1.5, 0);
							glVertex3f( i+1.5,j+1.5, 0);
							glVertex3f( i+1.5,j+0.5, 0);
							glVertex3f(i+0.5,j+0.5, 0);						
							glEnd();				
						}
					}
					if(i%2!=0)
					{
						if(j%2!=0)
						{
							glBegin(GL_QUADS);
							glColor3f(0.66f,0.66f,0.66f);
							glVertex3f(i+0.5, j+1.5, 0);
							glVertex3f( i+1.5,j+1.5, 0);
							glVertex3f( i+1.5,j+0.5, 0);
							glVertex3f(i+0.5,j+0.5, 0);
							glEnd();
						}
						else
						{
							glBegin(GL_QUADS);
							glColor3f(0.82f,0.82f,0.82f);
							glVertex3f(i+0.5, j+1.5, 0);
							glVertex3f( i+1.5,j+1.5, 0);
							glVertex3f( i+1.5,j+0.5, 0);
							glVertex3f(i+0.5,j+0.5, 0);						
							glEnd();
						}
						
					}
				}
			}

  

					glBegin(GL_QUADS);
					glColor3f(0.3098f,0.4667f,0.4275f);//	glColor3f(0.054,0.278f,0.121f);	

				//	glColor3f(0.6275,0.3922,0.2627);
	 				glVertex3f( 5, 4,0.0f);			// 四边形的右上顶点 (顶面)
					glVertex3f(0.0f, 4,0.0f);			// 四边形的左上顶点 (顶面)
					glVertex3f(0.0f, 4.0f, tmp);			// 四边形的左下顶点 (顶面)265f //0.265f
					glVertex3f(5, 4, tmp); //	0.265f		// 四边形的右下顶点 (顶面)


					glColor3f(0.3098f,0.4667f,0.4275f);// glColor3f(0.054,0.278f,0.121f);		
				//	glColor3f(0.6275,0.3922,0.2627);
				glVertex3f(5,0,tmp );			// 四边形的右上顶点(底面)0.265f
				glVertex3f(0,0.0f, tmp);			// 四边形的左上顶点(底面)0.265f
				glVertex3f(0,0,0.0f);			// 四边形的左下顶点(底面)
				glVertex3f( 5,0,0.0f);			// 四边形的右下顶点(底面)

			 		glColor3f(0.3098f,0.4667f,0.4275f);//  glColor3f(0.054,0.278f,0.121f);
				//glColor3f(0.6275,0.3922,0.2627);
				glVertex3f( 5, 4,tmp);			// 四边形的右上顶点(前面) 0.265f
				glVertex3f(0, 4,tmp );			// 四边形的左上顶点(前面)0.265f
				glVertex3f(0,0, tmp);			// 四边形的左下顶点(前面)0.265f
				glVertex3f( 5,0,tmp);			// 四边形的右下顶点(前面) 0.265f

		 			glColor3f(0.3098f,0.4667f,0.4275f);//glColor3f(0.054,0.278f,0.121f);
				//glColor3f(0.6275,0.3922,0.2627);
				glVertex3f(0, 4, tmp);			// 四边形的右上顶点(左面)0.265f
				glVertex3f(0, 4,0.0f);			// 四边形的左上顶点(左面)
				glVertex3f(0,0,0.0f);			// 四边形的左下顶点(左面)
				glVertex3f(0,0, tmp);			// 四边形的右下顶点(左面)0.265f

			glColor3f(0.3098f,0.4667f,0.4275f);//glColor3f(0.054,0.278f,0.121f);
				//		glColor3f(0.6275,0.3922,0.2627);
				glVertex3f( 5,4,0.0f);			// 四边形的右上顶点(右面)
				glVertex3f( 5, 4,tmp);			// 四边形的左上顶点(右面) 0.265f
				glVertex3f( 5,0, tmp);			// 四边形的左下顶点(右面)0.265f
				glVertex3f( 5,0,0.0f);		// 四边形的右下顶点(右面) 

			glEnd();					// 立方体绘制结束     	 
			glFinish();
	}

	// 启用双缓冲
	SwapBuffers(m_pDC->GetSafeHdc());
}
void CFluidsimView::SetViewMode()
{
	
	//其他参数
	/* 录制参数
	m_ClientRect. bottom = 639;
	m_ClientRect. right= 1362;
	SceneZoom = 3.7671192248276659;
	SceneDepth = 300;
	ScenePos.x = -7.47477;
	ScenePos.y = 0.000000;
	ScenePos.z = 1.70386;
	SceneAngle.x = 24.1667;
	SceneAngle.y = 0.000000;
	SceneAngle.z = -39.1667;  */


	m_dAspectRatio=double(m_ClientRect.right)/double(m_ClientRect.bottom);
	// 设置视口变换
	glViewport(0,0,m_ClientRect.right,m_ClientRect.bottom);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if(m_ClientRect.right<=m_ClientRect.bottom)
		glOrtho(-1.0*SceneZoom,SceneZoom,-1.0*SceneZoom*m_ClientRect.bottom/m_ClientRect.right,SceneZoom*m_ClientRect.bottom/m_ClientRect.right,-1.0*SceneDepth,SceneDepth);
	else
		glOrtho(-1.0*SceneZoom*m_ClientRect.right/m_ClientRect.bottom,SceneZoom*m_ClientRect.right/m_ClientRect.bottom,-1.0*SceneZoom,SceneZoom,-1.0*SceneDepth,SceneDepth);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(-90,1.0f,0.0f,0.0f);
	// 旋转对象一个给定的角度
	glTranslatef(ScenePos.x,ScenePos.y,ScenePos.z);
	glRotated(SceneAngle.x,1.0,0.0,0.0);
	glRotated(SceneAngle.y,0.0,1.0,0.0);
	glRotated(SceneAngle.z,0.0,0.0,1.0);
}

void CFluidsimView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CFluidsimView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFluidsimView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CFluidsimView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CFluidsimView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CFluidsimView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CFluidsimView 诊断

#ifdef _DEBUG
void CFluidsimView::AssertValid() const
{
	CView::AssertValid();
}

void CFluidsimView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFluidsimDoc* CFluidsimView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFluidsimDoc)));
	return (CFluidsimDoc*)m_pDocument;
}
#endif //_DEBUG


// CFluidsimView 消息处理程序




void CFluidsimView::OnOpen1()
{
	
	CFileDialog dlg(TRUE, _T("*.bmp"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",NULL);
	char title[]= {"Open Image"};
	dlg.m_ofn.lpstrTitle = title;
	
	if (dlg.DoModal() == IDOK) {
		
		path= dlg.GetPathName();		
		image1=cvLoadImage(path,1);//color
		gray1 = cvLoadImage(path,0);//gray
		
		cvNamedWindow( "First Image", 1 ); 
		cvShowImage( "First Image", image1 ); 
	}


	//第二个图像的文件路径
	char path2[255];
	strcpy(path2, (LPCTSTR)path);
	int len = strlen(path2);

	if (path2[len-5]-48 <9)
		path2[len-5] = path2[len-5]+1;

	else if(path2[len-5]-48 == 9) 
	{
		path2[len-5] = 0;
		path2[len-6] = path2[len-6]+1;
	}

	image2=cvLoadImage(path2,1);//color
	gray2 = cvLoadImage(path2,0);//gray

	if(image1!=NULL && gray1!=NULL) 
	{
		cvNamedWindow( "Second Image", 1 ); 
		cvShowImage( "Second Image", image2 ); 
	}

	//第三个图像的文件路径
	char path3[255];
	strcpy(path3, (LPCTSTR)path2);
	len = strlen(path3);

	if (path3[len-5]-48<9)
		path3[len-5] = path3[len-5]+1;

	else if (path3[len-5]-48 == 9 )
	{
		path3[len-5] = '0';
		path3[len-6] = path3[len-6]+1;
	}

	image3=cvLoadImage(path3,1);//color
	gray3 = cvLoadImage(path3,0);//gray

	if(image1!=NULL && gray1!=NULL) 
	{
		cvNamedWindow( "Third Image", 1 ); 
		cvShowImage( "Third Image", image3 ); 
	}

}


void CFluidsimView::OnGeometry()
{


	showflag =1; //显示3D几何
}


void CFluidsimView::OnCalculatevel()
{
	showflag =2;

		/*	P3D2[pos].u = u[pos];
			P3D2[pos].v = v[pos];

			P3D2[pos].z= Z1[pos]; */

		
}

void CFluidsimView::DrawArrow(IplImage *image, GeoPoint *P3D, const char *window_name)
{
	IplImage *showImage; 
	showImage = cvCloneImage(image);
	CvSize img_sz = cvGetSize(image);
	nHeight = img_sz.height;
	nWidth = img_sz.width;

	for(int y=0;y<nHeight;y++)              
	for(int x=0;x<nWidth;x++)			
	{	
		int pos = y*nWidth+ x;
		if( x%8== 0 && y%8== 0 )
		{
			int line_thickness = 1;		
			CvScalar line_colorR=CV_RGB(255,0,0);
			CvScalar line_colorB=CV_RGB(0,255,0);
			if ( P3D[pos].u *P3D[pos].u + P3D[pos].v * P3D[pos].v !=0 )
			{					
				CvPoint p,q;
				p.x = x;
				p.y = y;
				q.x = x + P3D[pos].u;    
				q.y = y + P3D[pos].v;    
					
				float pi = 3.14159265;
				double angle;		
				angle = atan2( (double) p.y - q.y, (double) p.x - q.x );
				double hypotenuse;	
				
				hypotenuse = sqrt( pow( P3D[pos].u,2) + pow(P3D[pos].v,2) );
					
				// Here we lengthen the arrow by a factor of three. 
				q.x = (int) (p.x - 3 * hypotenuse * cos(angle));
				q.y = (int) (p.y - 3 * hypotenuse * sin(angle));
				
				if(angle > -pi/2  && angle < pi/2)
					cvLine( showImage, p, q, line_colorR, line_thickness, CV_AA, 0 );
				else
					cvLine( showImage, p, q, line_colorB, line_thickness, CV_AA, 0 );
					
				p.x = (int) (q.x + 3 * cos(angle + pi / 4));
				p.y = (int) (q.y + 3 * sin(angle + pi / 4));
					
				if(angle > -pi/2  && angle < pi/2)
					cvLine( showImage, p, q, line_colorR, line_thickness, CV_AA, 0 );
				else
					cvLine( showImage, p, q, line_colorB, line_thickness, CV_AA, 0 );
					
				p.x = (int) (q.x + 3 * cos(angle - pi / 4));
				p.y = (int) (q.y + 3 * sin(angle - pi / 4));

				if(angle > -pi/2  && angle < pi/2)
					cvLine( showImage, p, q, line_colorR, line_thickness, CV_AA, 0 );
				else
					cvLine( showImage, p, q, line_colorB, line_thickness, CV_AA, 0 );
			}
			else
			{
				CvPoint   centerpoint;
				centerpoint.x=x;
				centerpoint.y=y;

				line_thickness = 1;				
			}
		}
	}
		cvNamedWindow(window_name, 1 );
		cvShowImage(window_name,showImage);
}



int CFluidsimView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;


	//Mycode
	m_pDC = new CClientDC(this);
	InitializeOpenGL(m_pDC);

	SetTimer(1,30,NULL);
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	

	return 0;
}

BOOL CFluidsimView::InitializeOpenGL(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int n;
	m_pDC=pDC;
	ASSERT(m_pDC!=NULL);
	
	bSetupPixelFormat();
	
	n=::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(),n,sizeof(pfd),&pfd);
	m_hRC=wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(),m_hRC);
	
	glShadeModel(GL_FLAT);
	glDepthFunc(GL_LESS);   
	glEnable(GL_DEPTH_TEST);   
	
	return TRUE;
}

BOOL CFluidsimView::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			24,
			0,0,0,0,0,0,
			0,
			0,
			0,
			0,0,0,0,
			32,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0,0,0
	};
	
	int pixelformat;
	if((pixelformat=ChoosePixelFormat(m_pDC->GetSafeHdc(),&pfd))==0)
	{
		MessageBox("Choose Pixel Format Failed!");
		return FALSE;
	}
	if(SetPixelFormat(m_pDC->GetSafeHdc(),pixelformat,&pfd)==FALSE)
	{
		MessageBox("Set Pixel Format Failed!");
		return FALSE;
	}
	if(pfd.dwFlags&PFD_NEED_PALETTE)
		SetLogicalPalette();
	return TRUE;
}

void CFluidsimView::SetLogicalPalette()
{
	struct 
	{
		WORD Version;
		WORD NumberOfEntries;
		PALETTEENTRY aEntries[256];
	}logicalPalette={0x300,256};
	
	BYTE reds[]={0,36,72,109,145,182,218,255};
	BYTE greens[]={0,36,72,109,145,182,218,255};
	BYTE blues[]={0,85,170,255};
	
	for(int colorNum=0;colorNum<256;++colorNum)
	{
		logicalPalette.aEntries[colorNum].peRed=reds[colorNum&0x07];
		logicalPalette.aEntries[colorNum].peGreen=greens[(colorNum>>0x03)&0x07];
		logicalPalette.aEntries[colorNum].peBlue=blues[(colorNum>>0x06)&0x03];
		logicalPalette.aEntries[colorNum].peFlags=0;
	}
	m_hPalette=CreatePalette((LOGPALETTE*)&logicalPalette);
}


void CFluidsimView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	if ( 0 < cx && 0 < cy )
	{
		// 更新客户区，计算窗口的比例
		m_ClientRect.right = cx;
		m_ClientRect.bottom = cy;
	};	
}


void CFluidsimView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMainFrame *pMainfrm=(CMainFrame*)AfxGetMainWnd();
	CFluidsimDoc* pDoc=GetDocument();
	point2D p_Cur;
	
	if(pMainfrm->m_operator==OPERATOR_VIEW_ROTATE)
	{
		MouseDownPoint=point;
		SetCapture();
	}
	else if(pMainfrm->m_operator==OPERATOR_VIEW_TRANSLATE)
	{
		p_Cur.x=point.x;
		p_Cur.y=point.y;
		p_Cur=GetGLValue(p_Cur);
		view_base_point.x=p_Cur.x;
		view_base_point.y=p_Cur.y;
		SetCapture();
	}
	else if(pMainfrm->m_operator==OPERATOR_VIEW_ZOOM)
	{
		MouseDownPoint=point;
		SetCapture();
	} 

	CView::OnLButtonDown(nFlags, point);
}
point2D CFluidsimView::GetGLValue(point2D pt)
{
	point2D p;
	int w=m_ClientRect.right,h=m_ClientRect.bottom;
	if(w<h)
	{
		p.x=(pt.x-w/2)*SceneZoom*2/w;
		p.y=(h/2-pt.y)*SceneZoom*2/w;
	}
	else
	{
		p.x=(pt.x-w/2)*SceneZoom*2/h;
		p.y=(h/2-pt.y)*SceneZoom*2/h;
	}
	return p;
}

void CFluidsimView::OnDestroy()
{
	CView::OnDestroy();

	// 释放OpenGL绘制描述表
    wglMakeCurrent(NULL,NULL);
}


void CFluidsimView::OnLButtonUp(UINT nFlags, CPoint point)
{
	MouseDownPoint=CPoint(0,0);
	ReleaseCapture(); 
	CView::OnLButtonUp(nFlags, point);
}


void CFluidsimView::OnMouseMove(UINT nFlags, CPoint point)
{
	CMainFrame *pMainfrm=(CMainFrame*)AfxGetMainWnd();
	point2D p_Cur;
	
	if(pMainfrm->m_operator==OPERATOR_VIEW_ROTATE&&GetCapture()==this)
	{
		SceneAngle.x+=double(point.y-MouseDownPoint.y)/3.6;
		SceneAngle.z+=double(point.x-MouseDownPoint.x)/3.6;
		MouseDownPoint=point;
	}
	else if(pMainfrm->m_operator==OPERATOR_VIEW_ZOOM&&GetCapture()==this)
	{
		SceneZoom+=(double(point.y-MouseDownPoint.y)/200)*SceneZoom;
		if(SceneZoom>=200.0)
			SceneZoom=200.0;
		else if(SceneZoom<=0.1)
			SceneZoom=0.1;
		MouseDownPoint=point;
	}
	else if(pMainfrm->m_operator==OPERATOR_VIEW_TRANSLATE&&GetCapture()==this)
	{
		p_Cur.x=point.x;
		p_Cur.y=point.y;
		p_Cur=GetGLValue(p_Cur);
		ScenePos.x+=p_Cur.x-view_base_point.x;
		ScenePos.z+=p_Cur.y-view_base_point.y;
		view_base_point.x=p_Cur.x;
		view_base_point.y=p_Cur.y;
	}
	DrawScene();

	CView::OnMouseMove(nFlags, point);
}


void CFluidsimView::OnCalDensityFi()
{
}



void CFluidsimView::Relight3D(float *H,CvScalar *color)
{
/*	float maxh=0;  //当前帧
	float minh =1000;
		
	for(int y=0; y<nHeight; y++)
	for(int x=0; x<nWidth; x++)
	{

		int i = y*nWidth+x;
			
		if (H[i]>0 && H[i] > maxh ) 
			maxh = H[i];

		if (H[i]>0 && H[i] < minh )
			minh = H[i]; 
	}

	for( int j = 0; j < nHeight; j++)
	for( int i = 0; i < nWidth; i++)
	{
		int pos =j*nWidth +i;

		float tmpH = (H[pos]-minh)/(maxh-minh)*(Zmax-Zmin)+Zmin;

		color[pos].val[2] = 	KIa_R + KId_R*(2*tmpH*tmpH -1)+KIs_R * pow(tmpH,a_New);     
		if ( color[pos].val[2] >255 )color[pos].val[2] =255;

		color[pos].val[1] = 	KIa_G +   KId_G*(2*tmpH*tmpH -1) +KIs_G * pow(tmpH,a_New);
		if ( color[pos].val[1] >255 ) color[pos].val[1] =255;

		color[pos].val[0] = 	KIa_B +KId_B* (2*tmpH*tmpH -1) + KIs_B *  pow(tmpH,a_New);  
		if ( color[pos].val[0] >255  )color[pos].val[0] =255;
	}*/
	
}
void CFluidsimView::Relight2D(float *H,IplImage *newImage) 
{


//	cvSaveImage("out.jpg", newImage); */
	

}  




void CFluidsimView::OnOpenimage()
{
	CFileDialog dlg(TRUE, _T("*.bmp"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",NULL);
	char title[]= {"Open Image"};
	dlg.m_ofn.lpstrTitle= title;
	
	if (dlg.DoModal() == IDOK) {
		
		path= dlg.GetPathName();		
		image1=cvLoadImage(path,1);//color
		gray1 = cvLoadImage(path,0);//gray
		
		cvNamedWindow( "First Image", 1 ); 
		cvShowImage( "First Image", image1 ); 
	}


	//第二个图像的文件路径
	char path2[255];
	strcpy(path2, (LPCTSTR)path);
	int len = strlen(path2);

	if (path2[len-5]-48 <9)
		path2[len-5] = path2[len-5]+1;

	else if(path2[len-5]-48 == 9) 
	{
		path2[len-5] = 0;
		path2[len-6] = path2[len-6]+1;
	}

	image2=cvLoadImage(path2,1);//color
	gray2 = cvLoadImage(path2,0);//gray

	if(image1!=NULL && gray1!=NULL) 
	{
		cvNamedWindow( "Second Image", 1 ); 
		cvShowImage( "Second Image", image2 ); 
	}

	
}


void CFluidsimView::OnReadfile()
{
	char filename[512];
	strcpy(filename,path);
	int len = strlen(filename);

	for (int k = 0 ; k < 9 ; k++)  //100帧
	{
		if (k<=9)
			filename[len-5]= k+'0';
		
		else if (k<=99)
		{
			filename[len-5]= k%10+'0';
			filename[len-6]= k/10+'0';
		}
		else if (k>99) //取不到
		{
			filename[len-5]= k%10+'0';
			filename[len-6]= k%100/10+'0';
			filename[len-7]= k%100/100+'0';
		} 
		
		//申请单元
		if(k==0 && image1!=NULL && image2!=NULL)
		{
			CvSize imgtmpsize;
			imgtmpsize = cvGetSize( gray1);
			nHeight = imgtmpsize.height;
			nWidth = imgtmpsize.width;

			//R3D申请内存
			R3D = (GeoPoint**)malloc(100* sizeof(GeoPoint*));  //100帧 
			for( int i=0; i<100; i++)   
				R3D[i] = (GeoPoint*)malloc(nWidth*nHeight*sizeof(GeoPoint));  
		}

		//读取文件数据---高度场
		char Hfilename[512];
		strncpy(Hfilename,filename,len-8);
		Hfilename[len-9] = '\0';
		strcat(Hfilename,"\\H\\");
		strcat(Hfilename,filename+(len-8)); //末尾8个
		Hfilename[len-1+2]='t';
		Hfilename[len-2+2]='x';
		Hfilename[len-3+2]='t';

		ifstream myfile(Hfilename);

		for(int y=0;y<nHeight;y++)
		for(int x=0;x<nWidth;x++)
		{
			int pos = y*nWidth+x;
			myfile >> R3D[k][pos].z;
		}
		myfile.close();

		//读取文件数据---速度场
		char Velfilename[512];
		strncpy(Velfilename,filename,len-8);
		Velfilename[len-9] = '\0';
		strcat(Velfilename,"\\Vel\\");
		strcat(Velfilename,filename+(len-8)); //末尾8个
		Velfilename[len-1+4]='t';
		Velfilename[len-2+4]='x';
		Velfilename[len-3+4]='t';

		myfile.open(Velfilename);

		for(int y=0;y<nHeight;y++)
		for(int x=0;x<nWidth;x++)
		{
			int pos = y*nWidth+x;
			myfile>> R3D[k][pos].u ;
			myfile>> R3D[k][pos].v;
		}
		
		myfile.close();

		if(k==0)
		{
			////读取文件数据---Phong光照参数
			char Phongfilename[512];
			strncpy(Phongfilename,filename,len-8);
			Phongfilename[len-9] = '\0';
			strcat(Phongfilename,"\\Phong\\");
			strcat(Phongfilename,filename+(len-8)); //末尾8个
			Phongfilename[len-1+6]='t';
			Phongfilename[len-2+6]='x';
			Phongfilename[len-3+6]='t';

			ifstream myfile(Phongfilename);
		
			myfile>> KIa_R >> KId_R >> KIs_R ;
			myfile>> KIa_G >> KId_G >> KIs_G;
			myfile>> KIa_B >> KId_B >> KIs_B;
			myfile>> a_New >> Zmax >> Zmin;

			myfile.close();

		}
	 } //100帧的数据

}
void CFluidsimView::OnShow2D3D()
{
	frameNo = 0; // 帧号：第0帧

	H = new float[nWidth*nHeight];
	
	for( int j = 0; j < nHeight; j++) 
	for( int i = 0; i < nWidth; i++)
	{
		int pos =j*nWidth +i;
		H[pos] = R3D[frameNo][pos].z; 
	}
	CvSize Newsize = cvSize(nWidth,nHeight);
	IplImage *newImage = cvCreateImage( Newsize, 8, 3 );  

	Relight2D(H,newImage);  //newImage为结果图像     

	const char *window_name = "2D-Relighting" ;
	cvNamedWindow(window_name, 1 );
	cvShowImage(window_name,newImage);

	//3D-Relighting
	showflag =5;
	CvScalar *color = new CvScalar[nWidth*nHeight] ;

	Relight3D(H,color);

	for(int y=0; y<nHeight; y++)
	for(int x=0; x<nWidth; x++)
	{
		int pos =y*nWidth +x;
		R3D[frameNo][pos].R  = color[pos].val[2];
		R3D[frameNo][pos].G  = color[pos].val[1];
		R3D[frameNo][pos].B  = color[pos].val[0];
	}	
}



void CFluidsimView::OnReadTif()
{
	
	CFileDialog dlg(TRUE, _T("*.tif"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.tif) |*.tif;|All Files (*.*)|*.*||",NULL);
	char title[]= {"打开高程数据"};
	dlg.m_ofn.lpstrTitle= title;
	
	if (dlg.DoModal() != IDOK) 
		return;

	char filename[150];
	strcpy(filename,dlg.GetPathName());
	int len = strlen(filename);

	GDALAllRegister();  // 注册
	const char *cstr = filename; 
	GDALDataset *poDataset = (GDALDataset *)GDALOpen(cstr,GA_ReadOnly);
	GDALDataType dataType = poDataset->GetRasterBand(1)->GetRasterDataType();
	

	GDAL_example.DATAType = dataType;
	GDAL_example.projection_info = (char*)(poDataset->GetProjectionRef());
	poDataset->GetGeoTransform(GDAL_example.geos);
	
	
	if( poDataset == NULL )
	{
		std::cout << "Failed To Open DEM File!";  //打开文件失败
	}
	int i,j,bandNum;
	nWidth =  poDataset->GetRasterXSize();  //影像宽度
	nHeight = poDataset->GetRasterYSize(); //影像高度
	bandNum = poDataset->GetRasterCount(); //影像波段数量

	switch (dataType)
	{
		case GDT_UInt16: 
			{
				unsigned short *data = new unsigned short[nWidth*nHeight];
				GDALRasterBand *band = poDataset->GetRasterBand(1);//获取第一波段，波段从1开始
				band->RasterIO( GF_Read, 0, 0, nWidth, nHeight, 
                      data, nWidth, nHeight, dataType, 
                      0, 0 );
				GDALClose(poDataset);

				P3D1 = new GeoPoint[nWidth*nHeight];

				maxheight = 0; 
				maxheight = GetDemMaxHeight(nWidth, nHeight, data);
   
	
				minheight = maxheight;
				minheight = GetDemMinHeight(nWidth, nHeight, data, maxheight);

				for(int i = 0; i < nWidth; ++i)//nWidth/
				{	
					for(int j = 0; j < nHeight; ++j)//nHeight/
					{
						P3D1[i*nWidth+j].z = (double)(data[i*nWidth+j] - minheight)/(maxheight-minheight);//P3D1[i*nWidth+nHeight].z = data[i*nWidth+nHeight];
						P3D1[i*nWidth+j].H = data[i*nWidth+j];
						/*
						P3D1[i*nWidth+j].R = P3D1[i*nWidth+j].z * 255;
						P3D1[i*nWidth+j].G = P3D1[i*nWidth+j].z * 255;
						P3D1[i*nWidth+j].B = P3D1[i*nWidth+j].z * 255;
						*/
					}
				}
				break;
			}
		case GDT_Int16: 
			{
				short *data = new short[nWidth*nHeight];
				GDALRasterBand *band = poDataset->GetRasterBand(1);//获取第一波段，波段从1开始
				band->RasterIO( GF_Read, 0, 0, nWidth, nHeight, 
                      data, nWidth, nHeight, dataType, 
                      0, 0 );
				GDALClose(poDataset);

				P3D1 = new GeoPoint[nWidth*nHeight];

				maxheight = 0;//读取图像最高值
				maxheight = GetDemMaxHeight(nWidth, nHeight, data);
   
	
				minheight = maxheight;//读取图像最低值
				minheight = GetDemMinHeight(nWidth, nHeight, data, maxheight);

				for(int i = 0; i < nWidth; ++i)//nWidth/
				{	
					for(int j = 0; j < nHeight; ++j)//nHeight/
					{
						P3D1[i*nWidth+j].z = (double)(data[i*nWidth+j] - minheight)/(maxheight-minheight);//P3D1[i*nWidth+nHeight].z = data[i*nWidth+nHeight];
						P3D1[i*nWidth+j].H = data[i*nWidth+j];
					}
				}
				break;
			}

	}

	patch_size = ((nWidth < nHeight) ? nWidth : nHeight) / patch_width;    //计算有多少个patch

	Patch = (patch *)malloc(patch_size*patch_size*sizeof(struct patch));       //动态声明结构体数组


	for(int i = 0;i < patch_size; i++)
	{
		for(int j = 0; j < patch_size; j++)
		{
			Patch[i*patch_size+j].patch_X = i;
			Patch[i*patch_size+j].patch_Y = j;
			Patch[i*patch_size+j].label = -1;
			Patch[i*patch_size+j].flag = -1;
		}
	}

	IplImage* gray_image= cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,1);

	CvScalar s;
	for(int i = 0; i < nWidth; ++i)//nWidth/
	{	
		for(int j = 0; j < nHeight; ++j)//nHeight/
		{
			s=cvGet2D(gray_image,j,i);
			s.val[0] = int (P3D1[j*nHeight+i].z * 255);
			cvSet2D(gray_image,j,i,s);
		}
	}

	cvNamedWindow( "Tif image", 0); 
	cvShowImage( "Tif image", gray_image);
	cvSaveImage("20171017.jpg",gray_image);

	cvReleaseImage(&gray_image);

	withTexture = false;

	//float scale = 0.1;
	//Size size = Size(gray_image.cols*scale, gray_image.rows*scale);
	//Mat imagedst = Mat(size, gray_image.depth());  
    //resize(gray_image, imagedst, size); 


	
	
	/*CFileDialog dlg(TRUE, _T("*.bmp"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",NULL);
	char title[]= {"Open Image"};
	dlg.m_ofn.lpstrTitle= title;
	
	if (dlg.DoModal() != IDOK) 
		return;

	char filename[512];
	strcpy(filename,dlg.GetPathName());
	int len = strlen(filename);

	for (int k = 0 ; k < 1 ; k++)  //100帧  //30
	{

		if (k<=9)
			filename[len-5]= k+'0';
		
		else if (k<=99)
		{
			filename[len-5]= k%10+'0';
			filename[len-6]= k/10+'0';
		}
		else if (k>99) //取不到
		{
			filename[len-5]= k%10+'0';
			filename[len-6]= k%100/10+'0';
			filename[len-7]= k%100/100+'0';
		} 

			
		//当前帧文件路径filename
		image1=cvLoadImage(filename,1);//color
		gray1 = cvLoadImage(filename,0);//gray

		//下一帧文件路径
		char path2[255];
		strcpy(path2, (LPCTSTR)filename);
		int len = strlen(path2);

		if (path2[len-5]-48 <9)
			path2[len-5] = path2[len-5]+1;

		else if(path2[len-5]-48 == 9) 
		{
			path2[len-5] = '0';
			path2[len-6] = path2[len-6]+1;
		}

		image2=cvLoadImage(path2,1);//color
		gray2 = cvLoadImage(path2,0);//gray

		if(k==0 && image1!=NULL && image2!=NULL)
		{
			cvNamedWindow( "First Image", 1 ); 
			cvShowImage( "First Image", image1 );
			cvNamedWindow( "Second Image", 1 ); 
			cvShowImage( "Second Image", image2 ); 

			//////////////////////
			CvSize imgtmpsize;
			imgtmpsize = cvGetSize( gray1);
			nHeight = imgtmpsize.height;
			nWidth = imgtmpsize.width;

			P3D1 = new GeoPoint [nWidth*nHeight];
			P3D2 = new GeoPoint [nWidth*nHeight];
		}
		 
		GeoSuccessiveTwoFrame();
		//利用当前两帧计算几何及速度    
		if (k==0)
		{	
			showflag =1;
			DrawScene();

			//计算Phong光照模型
		//	OnCalPhong(); 	
		}

		////////////////////

		//VelSuccessiveTwoFrame();

			//写文件---高度场		
		///////////////////

	

		if (k==0) //2D Relightting---Normalized Geometry
		{	
			for( int j = 0; j < nHeight; j++)
			for(int i = 0; i < nWidth; i++)
			{
				int pos =j*nWidth +i;
				H[pos] = P3D1[pos].z;   
			}
			CvSize Newsize = cvSize(nWidth,nHeight);
			IplImage *newImage = cvCreateImage( Newsize, 8, 3 );  

			Relight2D(H,newImage);  //newImage为结果图像     

			const char *window_name = "Non-Normalized Geometry Relighting" ;
			cvNamedWindow(window_name, 1 );
			cvShowImage(window_name,newImage); //2D Relightting---end
		} 

		cvReleaseImage(&image1);
		image1 = NULL;
		cvReleaseImage(&gray1);
		gray1 = NULL;
		cvReleaseImage(&image2);
		image2 = NULL;
		cvReleaseImage(&gray2);
		gray2 = NULL;
	}*/
}


void CFluidsimView::OnSketchread()
{
	
	//载入sketch草图
	CString path;
	CFileDialog dlg(TRUE, _T("*.bmp"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",NULL);
	char title[]= {"Open Sketch Image"};
	dlg.m_ofn.lpstrTitle= title;
	
	if (dlg.DoModal() == IDOK) {
		
		path = dlg.GetPathName();		
		sketchimg = cvLoadImage(path,0);
		
		cvNamedWindow( "Sketch Image", 1 ); 
		cvShowImage( "Sketch Image", sketchimg ); 
	}

	CvSize imgtmpsize;
	imgtmpsize = cvGetSize(sketchimg);

	nWidthNew = imgtmpsize.width;
	nHeightNew = imgtmpsize.height;

	//计数 初始化heightflag
	int num = 0;
	CvScalar cvColor; 	
	heightflag = new int[nWidthNew*nHeightNew];
	for(int y=0; y<nHeightNew; y++)
	for(int x=0; x<nWidthNew; x++)
	{
		heightflag[y*nWidthNew+x] = 0;	

		cvColor=cvGet2D(sketchimg,y,x);

		if (cvColor.val[0] == 0 && cvColor.val[1]==0 && cvColor.val[2]==0)
			num++;
	}
	sketchpoint = new GeoPoint[num];

	//赋sketch点坐标
	int k =0;
	for( int y = 0; y < nHeightNew; y++)
	for(int x = 0; x < nWidthNew; x++)
	{
		cvColor=cvGet2D(sketchimg,y,x);

		if (cvColor.val[0] == 0 && cvColor.val[1]==0 && cvColor.val[2]==0)
		{
			sketchpoint[k].x = x;
			sketchpoint[k].y = y;
			k++;
		}
	}

	float mindis=0.0,maxdis=0.0;
	float tmpmin=10000;

	//记录所有像素到sketch点的最小距离和新分形维
	SketchPoints = new MapPoint[nWidthNew*nHeightNew];
		
	//寻找sketch上距离当前点距离最近的点
	for(int y=0; y<nHeightNew; y++)
	for(int x=0; x<nWidthNew; x++)
	{
		int pos = y*nWidthNew +x;

		tmpmin=10000;
		float tmpdis;

		for(int k = 0; k < num;k++)
		{			
			tmpdis = sqrt((x - sketchpoint[k].x)*(x - sketchpoint[k].x) + 
						  (y - sketchpoint[k].y)*(y - sketchpoint[k].y));
		
			if(tmpdis<tmpmin)
				tmpmin = tmpdis;
		}

		SketchPoints[pos].x = x;
		SketchPoints[pos].y = y;
		SketchPoints[pos].dist = tmpmin;
	}

	//按距离大小排序，从小到大，变换序号
	insertion_sort(SketchPoints);

	delete []SketchPoints;	

	//高度按距离关系重新映射
	
	numZ = 0;
	minZ = 10000.0;
	maxZ = 0.0;     

	for (int i = 0; i < nWidth*nHeight; i++)
	{
		if (P3D1[i].z > 0)
			numZ++;
		if (P3D1[i].z < minZ)
			minZ = P3D1[i].z ;
		if (P3D1[i].z > maxZ)
			maxZ = P3D1[i].z ;
	}
	//去除高度小于零的点
	sortZ = new double[numZ];
	int n=0;
	for ( int i = 0; i < numZ; i++)
	{
		if (P3D1[i].z > 0)
		{
			sortZ[n] = P3D1[i].z;
			n++;
		}
	}

	delete []P3D1;

	insertion_sort(sortZ);

}
void CFluidsimView::insertion_sort(struct MapPoint *Point)//从小到大
{
	int i, j;
	int len = nWidthNew*nHeightNew;
	MapPoint temp;

	for (i = 1; i < len; i++) 
	{
		temp = Point[i]; 

		for (j = i - 1; j >= 0 && Point[j].dist > temp.dist; j--) 
			Point[j + 1] = Point[j];
		
		Point[j + 1] = temp;
			
	}

}
void CFluidsimView::insertion_sort(double *Z)
{
	int i, j;
	double temp;

	for (i = 1; i < numZ; i++) 
	{
		temp = Z[i]; 

		for (j = i - 1; j >= 0 && Z[j] < temp; j--) 
			Z[j + 1] = Z[j];
		
		Z[j + 1] = temp;
			
	}
}

void CFluidsimView::OnInitscene()
{
	float xfactor = nWidthNew*1.0/nWidth;
	float yfactor = nHeightNew*1.0/nHeight;

	D3DNew = new GeoPoint [nWidthNew*nHeightNew];  //新的高度场

	for(int i = 0 ; i < nWidthNew; i ++)
	{
		for(int j = 0 ; j < nHeightNew ; j ++)
		{
			int x1=0,y1=0;
			x1 = i/xfactor;
		    y1 = j/yfactor;
			if( (x1 >= 0) && (x1 < nWidth ) && (y1 >= 0) && (y1 < nHeight))
			{
				D3DNew[j*nWidthNew+i].x = i;
				D3DNew[j*nWidthNew+i].y = j;
			}
		}
	}

	///////////////////////////////
		//高度重新映射
	CvScalar cvColor; 
	float maxD = SketchPoints[nWidthNew*nHeightNew-1].dist;
	D = new GLfloat[nWidthNew*nHeightNew];

	for(int i = 0; i <nWidthNew; i++)
	for(int j = 0; j <nHeightNew ; j ++)
	{
		int pos = j*nWidthNew +i;
		double tmp = pos*1.0/nWidthNew/nHeightNew;
		int posZ = tmp*numZ;                //newPoint[pos].dist/diffD ;pos*1.0/nWidthNew/nHeightNew;

		int tmpx = SketchPoints[pos].x;
		int tmpy = SketchPoints[pos].y;

		if(SketchPoints[pos].dist < 1)
			SketchPoints[pos].dist = 1;
		D[tmpy*nWidthNew+tmpx] = log(maxD/SketchPoints[pos].dist);

		cvColor=cvGet2D(sketchimg,tmpy,tmpx);

		if (cvColor.val[0] == 0 && cvColor.val[1]==0 && cvColor.val[2]==0)
		{
			gHillHeightMap[tmpx][tmpy] = sortZ[posZ];
			heightflag[tmpy*nWidthNew+tmpx] = 1;
		}

	}

	gHillHeightMap[0][0] = minZ;
	gHillHeightMap[0][nHeightNew-1] = minZ;
	gHillHeightMap[nWidthNew-1][0] = minZ;
	gHillHeightMap[nWidthNew-1][nHeightNew-1] = minZ;
	generateFractalHill(0,0,nWidthNew-1,nHeightNew-1,1);
}
void CFluidsimView::generateFractalHill(GLint x1,GLint y1,GLint x2,GLint y2,GLint depth)
{
	float tmpH = 1.0;
	if(x2-x1<=1&&y2-y1<=1)
	   return;
	else
	{
		if (heightflag[((y1+y2)/2)*nWidthNew+x1] == 0)//左
		{
			float watch1 =(gHillHeightMap[x1][y1]+gHillHeightMap[x1][y2])/2;
			float watch2 = pow(0.5f,depth*tmpH)*funNum(0,1);
			float Scale = D[((y1+y2)/2)*nWidthNew+x1];

			//do
			//{
				gHillHeightMap[x1][(y1+y2)/2]=(gHillHeightMap[x1][y1]+gHillHeightMap[x1][y2])/2+pow(0.5f,depth*tmpH)+abs(funNum(0,1))*Scale;
			//}while(gHillHeightMap[x1][(y1+y2)/2] > maxZ);
			heightflag[((y1+y2)/2)*nWidthNew+x1] = 1;
		}

		if (heightflag[((y1+y2)/2)*nWidthNew+x2] == 0)//右
		{
			float watch1 =(gHillHeightMap[x2][y1]+gHillHeightMap[x2][y2])/2;
			float watch2 = pow(0.5f,depth*tmpH)*funNum(0,1);
			float Scale = D[((y1+y2)/2)*nWidthNew+x2];

			//do
			//{
				gHillHeightMap[x2][(y1+y2)/2]=(gHillHeightMap[x2][y1]+gHillHeightMap[x2][y2])/2+pow(0.5f,depth*tmpH)+abs(funNum(0,1))*Scale;
			//}while(gHillHeightMap[x2][(y1+y2)/2] > maxZ);
			heightflag[((y1+y2)/2)*nWidthNew+x2] = 1;

		}

		if (heightflag[y1*nWidthNew+((x1+x2)/2)] == 0)//上
		{
			float Scale = D[y1*nWidthNew+((x1+x2)/2)];
			//do
			//{
				gHillHeightMap[(x1+x2)/2][y1]=(gHillHeightMap[x1][y1]+gHillHeightMap[x2][y1])/2+pow(0.5f,depth*tmpH)+abs(funNum(0,1))*Scale;
			//}while(gHillHeightMap[(x1+x2)/2][y1] > maxZ);
			heightflag[y1*nWidthNew+((x1+x2)/2)] = 1;
		}

		if (heightflag[y2*nWidthNew+((x1+x2)/2)] == 0)//下
		{
			float Scale = D[y2*nWidthNew+((x1+x2)/2)];
			float watch = pow(0.5f,depth*tmpH);
			//do
			//{
				gHillHeightMap[(x1+x2)/2][y2]=(gHillHeightMap[x1][y2]+gHillHeightMap[x2][y2])/2+pow(0.5f,depth*tmpH)+abs(funNum(0,1))*Scale;
			//}while(gHillHeightMap[(x1+x2)/2][y2] > maxZ);
			heightflag[y2*nWidthNew+((x1+x2)/2)] = 1;
		}

		if (heightflag[((y1+y2)/2)*nWidthNew+((x1+x2)/2)] == 0)//中
		{
			float Scale = D[((y1+y2)/2)*nWidthNew+((x1+x2)/2)];
			//do
			//{
				gHillHeightMap[(x1+x2)/2][(y1+y2)/2]=(gHillHeightMap[x1][y1]+gHillHeightMap[x1][y2]+gHillHeightMap[x2][y1]+gHillHeightMap[x2][y2])/4+pow(0.5f,depth*tmpH)+abs(funNum(0,1))*Scale;
			//}while(gHillHeightMap[(x1+x2)/2][(y1+y2)/2] > maxZ);
			heightflag[((y1+y2)/2)*nWidthNew+((x1+x2)/2)] = 1;
		}

		depth++;

		generateFractalHill(x1,y1,(x1+x2)/2,(y1+y2)/2,depth);
		generateFractalHill((x1+x2)/2,y1,x2,(y1+y2)/2,depth);
		generateFractalHill(x1,(y1+y2)/2,(x1+x2)/2,y2,depth);
		generateFractalHill((x1+x2)/2,(y1+y2)/2,x2,y2,depth);
		
	}
}
GLfloat CFluidsimView::funNum(GLfloat miu,GLfloat sigma)
{
	GLfloat r1,r2;
	GLfloat u,v,w;
	GLfloat x,y;
	
	do{
		r1=rand()/(GLfloat)32767;
		r2=rand()/(GLfloat)32767;

		u=2*r1-1;
		v=2*r2-1;

		w=u*u+v*v;
	
	}while(w>1);

	x=u*sqrt(((-log(w))/w));
    y=v*sqrt(((-log(w))/w));

	return miu+sigma*x;
}


int CFluidsimView::GetDemMaxHeight(int W, int H, unsigned short *D)
{
	int maxheight=0;//读取图像最高值
	for (int i=0;i<W;i++)
    {
		for(int j = 0; j < H; j++)
		{
			if (D[i*W+j]>maxheight)
			{
				maxheight=D[i*W+j];
			}
		}
	}
	return maxheight;
}


int CFluidsimView::GetDemMinHeight(int W, int H, unsigned short *D, int Max)
{
	int minheight=Max;//读取图像最低值
	for (int i=0;i<W;i++)
    {
		for(int j = 0; j < H; j++)
		{
			if (D[i*W+j]<minheight)
			{
				minheight=D[i*W+j];
			}
		}
	}
	return minheight;
}

int CFluidsimView::GetDemMaxHeight(int W, int H, short *D)//重载
{
	int maxheight=0;//读取图像最高值
	for (int i=0;i<W;i++)
    {
		for(int j = 0; j < H; j++)
		{
			if (D[i*W+j]>maxheight)
			{
				maxheight=D[i*W+j];
			}
		}
	}
	return maxheight;
}



int CFluidsimView::GetDemMinHeight(int W, int H, short *D, int Max)//重载
{
	int minheight=Max;//读取图像最低值
	for (int i=0;i<W;i++)
    {
		for(int j = 0; j < H; j++)
		{
			if (D[i*W+j]<minheight)
			{
				minheight=D[i*W+j];
			}
		}
	}
	return minheight;
}

void CFluidsimView::OnInitScene()
{
	
	for(int i= 0; i < nWidth; ++i)//nWidth/
	{	
		for(int j = 0; j < nHeight; ++j)//nHeight/
		{
			P3D1[i*nWidth+j].x = i;
			P3D1[i*nWidth+j].y = j;
		}
	}
	if(withTexture)
		showflag = 1;
	else
	{
		for(int i = 0; i < nWidth; ++i)//nWidth/
		{	
			for(int j = 0; j < nHeight; ++j)//nHeight/
			{
				/*
				P3D1[i*nWidth+j].R = P3D1[i*nWidth+j].z * 255;
				P3D1[i*nWidth+j].G = P3D1[i*nWidth+j].z * 255;
				P3D1[i*nWidth+j].B = P3D1[i*nWidth+j].z * 255;
				*/
				if (P3D1[i*nWidth+j].z < 0.14)
				{
					P3D1[i*nWidth+j].R = P3D1[i*nWidth+j].G = P3D1[i*nWidth+j].B = 36;

				}
				else if(P3D1[i*nWidth+j].z < 0.28)
					P3D1[i*nWidth+j].R = P3D1[i*nWidth+j].G = P3D1[i*nWidth+j].B = 72;
				else if(P3D1[i*nWidth+j].z < 0.42)
					P3D1[i*nWidth+j].R = P3D1[i*nWidth+j].G = P3D1[i*nWidth+j].B = 108;
				else if(P3D1[i*nWidth+j].z < 0.56)
					P3D1[i*nWidth+j].R = P3D1[i*nWidth+j].G = P3D1[i*nWidth+j].B = 140;
				else if(P3D1[i*nWidth+j].z < 0.70)
					P3D1[i*nWidth+j].R = P3D1[i*nWidth+j].G = P3D1[i*nWidth+j].B = 172;
				else if(P3D1[i*nWidth+j].z < 0.84)
					P3D1[i*nWidth+j].R = P3D1[i*nWidth+j].G = P3D1[i*nWidth+j].B = 204;
				else
					P3D1[i*nWidth+j].R = P3D1[i*nWidth+j].G = P3D1[i*nWidth+j].B = 255;

			}
		}
		showflag = 2;
	}
	DrawScene();
}



void CFluidsimView::OnReadTexture()
{
	CFileDialog dlg(TRUE, _T("*.bmp"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp; *.jpg) |*.bmp;*.jpg|All Files (*.*)|*.*||",NULL);
	char title[]= {"打开纹理数据"};
	dlg.m_ofn.lpstrTitle= title;
	
	if (dlg.DoModal() != IDOK) 
		return;

	char textureName[100];
	strcpy(textureName,dlg.GetPathName());
	int len = strlen(textureName);
	
	IplImage* textureImg = cvLoadImage(textureName, 1);
	int tWidth = textureImg->width;
	int tHeight = textureImg->height;

	CvScalar s;
	for(int j = 0; j < tHeight; ++j)//nWidth/
	{	
		for(int i = 0; i < tWidth; ++i)//nHeight/
		{
			s=cvGet2D(textureImg,j ,i);
			P3D1[j*tWidth+i].R = s.val[2];
			P3D1[j*tWidth+i].G = s.val[1];
			P3D1[j*tWidth+i].B = s.val[0];
		}
	}
	cvNamedWindow( "Texture image", 1); 
	cvShowImage( "Texture image", textureImg);

	cvReleaseImage(&textureImg);

	withTexture = true;
}

void CFluidsimView::CalPatchAverageHeight(GeoPoint *Point)
{

	//patch_size = ((nWidth < nHeight) ? nWidth : nHeight) / patch_width;    //计算有多少个patch

	
	
	
	
	for(int i = 0;i < patch_size; i++)
	{
		for(int j = 0; j < patch_size; j++)
		{
			float Sum_H = 0;
			for(int m = 0; m < patch_width; m++)
				for(int n = 0; n <patch_height; n++)
				{
					int row = patch_width*i + m;
					int column = patch_height*j + n;
					int pos = nWidth*row + column;
					Sum_H += Point[pos].H;                              //计算每个patch内高程值的和		
				}
			Patch[i*patch_size+j].average_height = Sum_H / (patch_width*patch_height);  //计算每个patch内高程值的平均值
		}
	}
}

void CFluidsimView::CalPatchVariance(GeoPoint *Point)
{
	//patch_size = ((nWidth < nHeight) ? nWidth : nHeight) / patch_width;    //计算有多少个patch
	for(int i = 0;i < patch_size; i++)
	{
		for(int j = 0; j < patch_size; j++)
		{
			float Sum_V = 0;
			for(int m = 0; m < patch_width; m++)
				for(int n = 0; n <patch_height; n++)
				{
					int row = patch_width*i + m;       ///可能写错了
					int column = patch_height*j + n;
					int pos = nWidth*row + column;
					Sum_V += (Point[pos].H - Patch[i*patch_size+j].average_height)*(Point[pos].H - Patch[i*patch_size+j].average_height);	
				}
			Patch[i*patch_size+j].patch_Variance = Sum_V / (patch_size*patch_size);    //计算每个patch的方差
			int xxxxxxx = 0;
		}
	}

}

void CFluidsimView::CalPatchRelief(GeoPoint *Point)
{
	//patch_size = ((nWidth < nHeight) ? nWidth : nHeight) / patch_width;    //计算有多少个patch
	for(int i = 0;i < patch_size; i++)
	{
		for(int j = 0; j < patch_size; j++)
		{
			int max_H = 0;
			int min_H = 32767;
			for(int m = 0; m < patch_width; m++)
				for(int n = 0; n <patch_height; n++)
				{
					int row = patch_width*i + m;
					int column = patch_height*j + n;
					int pos = nWidth*row + column;
					if( max_H <= Point[pos].H )	
						max_H = Point[pos].H;
					if( min_H >= Point[pos].H )
						min_H = Point[pos].H;
				}
			Patch[i*patch_size+j].patch_relief = max_H - min_H;
		}
	}
}

void CFluidsimView::OnCalPatchAverageHeight()
{
	CalPatchAverageHeight(P3D1);
	AfxMessageBox(_T("计算完毕"));
}




void CFluidsimView::OnCalPatchVariance()
{
	CalPatchVariance(P3D1);
	AfxMessageBox(_T("计算完毕"));
}




void CFluidsimView::OnCalPatchRelief()
{
	CalPatchRelief(P3D1);
	AfxMessageBox(_T("计算完毕"));
}


void CFluidsimView::KMeans(patch_mountain *samples,int sample_count,int &cluster_count,float threshold)  //KMeans( float* samples,int sample_count,int &cluster_count,int *labels,float threshold)
{
    
    cluster_count = 0;
    //对所有的样本初始化为没有类别，类别 = -1
	for( int i =0; i< sample_count; i++)
		samples[i].label = -1;

	for(int i = 0; i < sample_count; i++ )
	{	 
	  bool flagcurr = false; 
	  if(samples[i].label == -1) //指针指向第i个样本
	  {
		  for( int j = 0; j < sample_count; j++ )
		  {
			  if(samples[i].label == -1 && j!=i)   //指针指向第j个样本
			  {
				  //第一维和第二维是x，y， dims是维数；
				  float dx = (samples[i].X-samples[j].X) * (samples[i].X-samples[j].X);
				  float dy = (samples[i].Y-samples[j].Y) * (samples[i].Y-samples[j].Y);

				  //float t2 = (samples[i].patch_Variance-samples[j].patch_Variance) * (samples[i].patch_Variance-samples[j].patch_Variance);
				  //float t3 = (samples[i].patch_relief-samples[j].patch_relief) * (samples[i].patch_relief-samples[j].patch_relief);
				  float t = sqrt(dx + dy);
				  if( t < threshold)
				  {
					flagcurr = true;

					if( samples[i].label == -1 )
					{
						samples[i].label = cluster_count;
						samples[j].label = samples[i].label;
						cluster_count++;
					}
					else
						samples[j].label = samples[i].label;
				  }
			  }

		  }
		  if(flagcurr == false) //在循环之前，i的类别是-1，并且循环之后没有j与i相似，则将i单独列成一个类
		  {
			  samples[i].label = cluster_count;
			  cluster_count++;
		  }
	  } //if
	}  //for
    cluster_count++;
	


	/*例子
	int cluster_count = 0;
		int *labels = new int [count];
	
		KMeans( alfa,count,cluster_count,labels,threshold);
	
		int *N1 = new int[cluster_count];

		for ( int i = 0 ; i < cluster_count;i++)
			N1[i] = 0;

		for( int i = 0 ;i < count; i++)
		{
			int tmp = labels[i];
			N1[tmp]++;
		}
	
		float max = 0.0;

		max = 0.0;
		int sum =0;
		int n_R =0;
		t =0;
		for( int k1 = 0 ; k1<cluster_count ;k1++)
		{
			if (N1[k1] >max)
			{
				max = N1[k1];
				t= k1;
			}
		}
		for(int i = 0 ;i < count ;i++)
		{
			if (labels[i] == t)
			{
				sum += alfa[i];
				n_R++;
			}
		}
	   alfa_R = sum/n_R ; 

	   delete []labels;
	   delete []N1;
	   */

}



void CFluidsimView::OnKmeansCluster()
{
	
	KmeansCluster(Patch);
	
	
	
	
	
	/*
	//聚类
	int cluster_count = 0;
	//int *labels = new int [patch_size*patch_size];
	float threshold = 100.0;
	
	KMeans( Patch,patch_size*patch_size,cluster_count,threshold);//KMeans( Patch,patch_size,cluster_count,labels,threshold);
	*/
}


void CFluidsimView::OnWriteData2Txt()
{
	WritePatchData2Txt("patch_variance.txt",patch_size,Patch,0);
	WritePatchData2Txt("patch_average_height.txt",patch_size,Patch,1);
	WritePatchData2Txt("patch_relief.txt",patch_size,Patch,2);
	
}


void CFluidsimView::WritePatchData2Txt(char *filename, int patch_size, patch *p, int flag)
{

	ofstream outFile(filename, ios_base::out);  //按新建或覆盖方式写入  
    // 写入数据  
     if(flag == 0)//flag = 0,写入patch的高程方差
	{
		for (int r = 0; r < patch_size; r++)         //row
		{
			for (int c = 0; c < patch_size; c++)  //column
			{  
				float tmp = p[r*patch_size+c].patch_Variance;    //读取数据，at<type> - type 是矩阵元素的具体数据格式  
				outFile << tmp << "\t" ;   //每列数据用 tab 隔开  
			}
			outFile << endl;  //换行 
		}		
     }
	 else if(flag == 1) //flag=1,写入patch的高程均值
	 {
		 for (int r = 0; r < patch_size; r++)         //row
		{
			for (int c = 0; c < patch_size; c++)  //column
			{  
				float tmp = p[r*patch_size+c].average_height;    //读取数据，at<type> - type 是矩阵元素的具体数据格式  
				outFile << tmp << "\t" ;   //每列数据用 tab 隔开  
			}
			outFile << endl;  //换行 
		}		
	 }
	  else if(flag == 2) //flag=2,写入patch的地形起伏度
	 {
		 for (int r = 0; r < patch_size; r++)         //row
		{
			for (int c = 0; c < patch_size; c++)  //column
			{  
				float tmp = p[r*patch_size+c].patch_relief;    //读取数据，at<type> - type 是矩阵元素的具体数据格式  
				outFile << tmp << "\t" ;   //每列数据用 tab 隔开  
			}
			outFile << endl;  //换行 
		}		
	 }
     
	 outFile.close();
	 AfxMessageBox(_T("保存完毕"));
}


void CFluidsimView::KmeansCluster(patch *P)
{
	//初始化
	for(int i = 0; i < patch_size*patch_size; ++i)
	{
		P[i].label = 0;
		//P[i].label_water = -1;
	}
	
	for(int i = 0; i < patch_size*patch_size; ++i)
	{
		/*
		float threshold_V = 4e+5;//区别山  0.2e+9
		float threshold_relief =200;  //0.2e+4
		float threshold_height =1700;  //0.1e+4
		*/
//1_0_4
float threshold_V = 0.09;
float threshold_relief =50; 
float threshold_height =1500;

		if( P[i].patch_Variance > threshold_V)
		{
			P[i].label = 1;
		}
		else if( P[i].patch_relief < threshold_relief && P[i].average_height < threshold_height)
		{
			P[i].label = 2;
		}
	}

}

void CFluidsimView::OnSavePatch()
{
	SavePatch(Patch);
}

void CFluidsimView::SavePatch(patch *P)
{
	for(int i = 0; i < patch_size; ++i)
	{
		for(int j = 0; j < patch_size; ++j)
		{	
			if( P[i*patch_size + j].label == 1)
			{
				IplImage *patch_pic= cvCreateImage(cvSize(patch_width,patch_height),IPL_DEPTH_8U,1);
				CvScalar s;

				int x = i*patch_width;     //坐标(x,y);
				int y = j*patch_height;

				
				string pic_name = "mountain/patch_";
				for(int m = 0; m < patch_height; ++m)//nWidth/行
				{	
					for(int n = 0; n < patch_width; ++n)//nHeight/列
					{
						int X = x+m;
						int Y = y+n;
						s=cvGet2D(patch_pic,m,n);
						s.val[0] = int (P3D1[X*nWidth+Y].z * 255);
						cvSet2D(patch_pic,n,m,s);
					}
				}
				stringstream tmp; 
				tmp<< (i*nWidth+j); 
				pic_name += tmp.str();
				pic_name += ".jpg";
				char *pp =(char*)pic_name.data();
				cvSaveImage(pp, patch_pic);
				cvReleaseImage(&patch_pic);

				}

		}
	}

	AfxMessageBox(_T("保存完毕"));
}


void CFluidsimView::On32821()
{
	for(int i= 0; i < patch_size; ++i)//nWidth/
	{	
		for(int j = 0; j < patch_size; ++j)//nHeight/
		{
			if( Patch[i*patch_size + j].label == 1)   //1:mountain
			{
				int x = i*patch_width;
				int y = j*patch_height;
				for(int k = 0; k<patch_width; ++k)
					for(int l = 0; l <patch_height;++l)
					{
						int pos = (x+k)*nWidth+(y+l);
						P3D1[pos].R = 155;
						P3D1[pos].G = 48;
						P3D1[pos].B = 255;
					}
			}
			if( Patch[i*patch_size + j].label == 2)   //2:water
			{
				int x = i*patch_width;
				int y = j*patch_height;
				for(int k = 0; k<patch_width; ++k)
					for(int l = 0; l <patch_height;++l)
					{
						int pos = (x+k)*nWidth+(y+l);
						P3D1[pos].R = 0;
						P3D1[pos].G = 191;
						P3D1[pos].B = 255;
					}
			}
		}
	}
	showflag = 1;

	DrawScene();

}




void CFluidsimView::OnRandomTree()
{
	//定义训练数据与标签矩阵
    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES, 1, CV_32FC1);

    //定义测试数据矩阵与标签
    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
    Mat testing_classifications = Mat(NUMBER_OF_TESTING_SAMPLES, 1, CV_32FC1);

	Mat var_type = Mat(ATTRIBUTES_PER_SAMPLE + 1, 1, CV_8U );
    var_type.setTo(Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

	//this is a classification problem (i.e. predict a discrete number of class
    //outputs) so reset the last (+1) output var_type element to CV_VAR_CATEGORICAL
	var_type.at<uchar>(ATTRIBUTES_PER_SAMPLE, 0) = CV_VAR_CATEGORICAL;

	double result; // value returned from a prediction
	char* training_path = "training_data.csv";

    //加载训练数据集和测试数据集
    if (read_data_from_csv(training_path, training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES) &&
            read_data_from_csv(training_path, testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES))
    {
      /********************************步骤1：定义初始化Random Trees的参数******************************/
        float priors[] = {1,1,0.5};  // weights of each classification for classes
        CvRTParams params = CvRTParams(25, // max depth
                                       5, // min sample count
                                       0, // regression accuracy: N/A here
                                       false, // compute surrogate split, no missing data
                                       15, // max number of categories (use sub-optimal algorithm for larger numbers)
                                       priors, // the array of priors
                                       false,  // calculate variable importance
                                       4,       // number of variables randomly selected at node and used to find the best split(s).
                                       10,	 // max number of trees in the forest
                                       0.01f,				// forrest accuracy
                                       CV_TERMCRIT_ITER |	CV_TERMCRIT_EPS // termination cirteria
                                      );

        /****************************步骤2：训练 Random Decision Forest(RDF)分类器*********************/
        printf( "\nUsing training database: %s\n\n", "training_data.csv");   //Skin_NonSkin.csv
        CvRTrees* rtree = new CvRTrees;
        rtree->train(training_data, CV_ROW_SAMPLE, training_classifications,
                     Mat(), Mat(), var_type, Mat(), params);

        // perform classifier testing and report results
        //Mat test_sample;

        Mat test_sample(1, 3, CV_32FC1, 1.0f);;

        int correct_class = 0;
        int wrong_class = 0;
        int false_positives [NUMBER_OF_CLASSES] = {0,0};

        printf( "\nUsing testing database: %s\n\n", "training_data.csv");


		char* pic_path = "1.jpg";
		Mat img = imread(pic_path, CV_LOAD_IMAGE_UNCHANGED);
		Mat img_gray(img.rows, img.cols,CV_8UC1, Scalar(0)); //创建一个图像 ( 3个通道, 8 bit位深, 高500, 宽1000, (0, 0, 100) 分别分配给 Blue, Green and Red. )


		size_t number_of = patch_size * patch_size;
	
		for (int p = 0; p < patch_size; p++)
		{
			//uchar* data = img_gray.ptr<uchar>(p);
			for (int q = 0; q < patch_size; q++)
			{

				test_sample.at <float>(0,0) = Patch[p*patch_size+q].average_height;
				test_sample.at <float>(0,1) = Patch[p*patch_size+q].patch_Variance;
				test_sample.at <float>(0,2) = Patch[p*patch_size+q].patch_relief;
				/********************************步骤3：预测*********************************************/
				result = rtree->predict(test_sample, Mat());
				
				if(result == 1.f)
				{
					Patch[p*patch_size+q].label = 1;
				}
				else if(result == 2.f)
				{
					Patch[p*patch_size+q].label = 2;
				}
				
			}
		}
}

	for(int i = 0;i < patch_size; i++)
	{
		for(int j = 0; j < patch_size; j++)
		{
			point tmp;
			if(Patch[i*patch_size+j].label == 1)
			{	
				tmp.X = i; tmp.Y = j;
				high_freqPatch.push_back(tmp);
			}
			else if(Patch[i*patch_size+j].label == 2)
			{
				tmp.X = i; tmp.Y = j;
				low_freqPatch.push_back(tmp);
			}
			else
			{
				tmp.X = i; tmp.Y = j;
				middle_freqPatch.push_back(tmp);
			}
		}
	}
}

int CFluidsimView::read_data_from_csv(const char* filename, Mat data, Mat classes, int n_samples)
{
    float tmp;
    // if we can't read the input file then return 0
    FILE* f = fopen( filename, "r" );
    if( !f )
    {
        printf("ERROR: cannot read csv file %s\n",  filename);
        return 0; // all not OK
    }

    // for each sample in the file
    for(int line = 0; line < n_samples; ++line)
    {
        // for each attribute on the line in the file
        for(int attribute = 0; attribute < (ATTRIBUTES_PER_SAMPLE + 1); attribute++)
        {
            if (attribute < 3)
            {
                // first 3 elements (0-2) in each line are the attributes
                fscanf(f, "%f,", &tmp);
                data.at<float>(line, attribute) = tmp;
				//double xxx =  data.at<float>(line, attribute);、··
				//int fff = 1;

                // printf("%f,", data.at<float>(line, attribute));
            }
            else if (attribute == 3)
            {
                // attribute 3 is the class label {1 ... 2}
                fscanf(f, "%f,", &tmp);
                classes.at<float>(line, 0) = tmp;
                // printf("%f\n", classes.at<float>(line, 0));
            }
        }
    }
    fclose(f);
    return 1; // all OK
}


void CFluidsimView::OnWriteData2CSV()
{
	WriteData2CSV("training_data.csv",Patch);
}

void CFluidsimView::WriteData2CSV(char *filename, patch *p)
{
	
	ofstream outFile(filename, ios_base::app);             //按新建或覆盖方式写入
	for (int r = 0; r < patch_size*patch_size; r++)        //row
	{  
		if(p[r].label  == 1)
		{
			int tmp1 = p[r].average_height;    //读取数据，at<type> - type 是矩阵元素的具体数据格式
			int tmp2 = p[r].patch_Variance;
			int tmp3 = p[r].patch_relief;
			int tmp4 = 1;
			outFile << tmp1 << "," <<tmp2 << "," << tmp3 << "," << tmp4;          //每列数据用 tab 隔开  
		}
		else if(p[r].label == 2)
		{
			int tmp1 = p[r].average_height;    //读取数据，at<type> - type 是矩阵元素的具体数据格式
			int tmp2 = p[r].patch_Variance;
			int tmp3 = p[r].patch_relief;
			int tmp4 = 2;
			outFile << tmp1 << "," <<tmp2 << "," << tmp3 << "," << tmp4;          //每列数据用 tab 隔开  
		}
		else
		{
			int tmp1 = p[r].average_height;    //读取数据，at<type> - type 是矩阵元素的具体数据格式
			int tmp2 = p[r].patch_Variance;
			int tmp3 = p[r].patch_relief;
			int tmp4 = 3;
			outFile << tmp1 << "," <<tmp2 << "," << tmp3 << "," << tmp4;          //每列数据用 tab 隔开  
		}
		outFile << endl;  //换行
	}
			 
	outFile.close();


}

void CFluidsimView::OnReadUserSketch()
{
	//载入sketch草图
	CString path;
	CFileDialog dlg(TRUE, _T("*.png"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp; *.jpg; *.png) |*.bmp;*.jpg;*.png|All Files (*.*)|*.*||",NULL);
	char title[]= {"Open Sketch Image"};
	dlg.m_ofn.lpstrTitle= title;
	
	if (dlg.DoModal() == IDOK) {
		
		path = dlg.GetPathName();		
		sketchimg = cvLoadImage(path,1);
		
		cvNamedWindow( "Sketch Image", 1 ); 
		cvShowImage( "Sketch Image", sketchimg ); 
	}

	CvSize imgtmpsize;
	imgtmpsize = cvGetSize(sketchimg);

	int nWidthSketch = imgtmpsize.width;
	int nHeightSketch = imgtmpsize.height;

	//计数 初始化heightflag
	int num = 0;
	CvScalar cvColor; 	
	heightflag = new int[nWidthSketch*nHeightSketch];
	for(int y=0; y<nHeightSketch; y++)
	for(int x=0; x<nWidthSketch; x++)
	{
		heightflag[y*nWidthSketch+x] = 0;	

		cvColor=cvGet2D(sketchimg,y,x);

		if (cvColor.val[0] == 0 && cvColor.val[1]==0 && cvColor.val[2]==0)
			num++;
	}
	sketchpoint = new GeoPoint[num];

	//赋sketch点坐标
	int k =0;
	for( int y = 0; y < nHeightSketch; y++)
	for(int x = 0; x < nWidthSketch; x++)
	{
		cvColor=cvGet2D(sketchimg,y,x);

		if (cvColor.val[0] == 0 && cvColor.val[1]==0 && cvColor.val[2]==0)
		{
			sketchpoint[k].x = x;
			sketchpoint[k].y = y;
			k++;
		}
	}
}


void CFluidsimView:: OnTifSavePatch()
{
	TifSavePatch(Patch);
}

void CFluidsimView::TifSavePatch(patch *P)
{
	int num = 0;
	for(int i = 0; i < patch_size; ++i)
	{
		for(int j = 0; j < patch_size; ++j)
		{	
			if( P[i*patch_size + j].label == 1)
			{
				num++;
				int x = i*patch_size;     //坐标(x,y);
				int y = j*patch_size;
				string tif_name = "mountain/patch_";
				short patch_data[patch_width*patch_height];
				for(int k = 0;k < patch_width;++k)
				{
					for(int l = 0;l < patch_height;++l)
					{
						int X = x+k;
						int Y = y+l;
						patch_data[k*patch_width+l] = P3D1[X*1000+Y].H;
					}
				}
				tif_name += Int_to_String(num);
				tif_name += ".tif";
				char* pp = (char *)tif_name.data();
				TifSaveGenerateNewDem(pp,patch_width,patch_height,GDAL_example.DATAType,GDAL_example.projection_info,patch_data);
			}
		}
	}
	AfxMessageBox(_T("保存完毕"));
}


void CFluidsimView::OnDrawSkeleton()
{
	CString path;
	CFileDialog dlg(TRUE, _T("*.png"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp; *.jpg; *.png) |*.bmp;*.jpg;*.png|All Files (*.*)|*.*||",NULL);
	char title[]= {"Open Source Image"};
	dlg.m_ofn.lpstrTitle= title;
	Mat sourceimg;

	if (dlg.DoModal() == IDOK) {
		
		path = dlg.GetPathName();
		string ss = CT2CA(path.GetBuffer(0));
		sourceimg= imread(ss);
		imshow("Source Image",sourceimg);

	}

	Mat skel = skeletonization(sourceimg);
	
	imshow("Skeleton Image",skel);
	imwrite("skeleton.png",skel);
}


void CFluidsimView::GenerateNewDem()
{
	GDALAllRegister();//注册类型,读取写入任何类型影像必须加入此句
	GDALDriver *poDriver;
	const char *pszFormat = "GTiff";
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);


	int BandNum = 1;
	GDALDataType dataType = GDAL_example.DATAType;
	int NewWidth = 1000;
	int NewHeight = 1000;

	if(poDriver == NULL) 
	{ 
		return;
	}

	GDALDataset *OutDs;
	char **papszOptions = NULL;
	char **papszMetadata; //这里的参数全是指tif格式的参数,如果是其他格式,请把这里所有注释掉,或者参照文档,自行设定 //设置压缩类型,envi只认得packbits压缩.
	//papszOptions = CSLSetNameValue( papszOptions, "COMPRESS", "PACKBITS" ); //设置压缩比,可以不用,有些时候压缩反而更大,因为是无损的,除非图片很大
	//papszOptions = CSLSetNameValue( papszOptions, "ZLEVEL", "9" ); //设置bsq或者BIP存储 bsq:BAND,bip:PIXEL
	papszOptions = CSLSetNameValue(papszOptions, "INTERLEAVE", "BAND");
	OutDs = poDriver->Create("mountain/sythesis11111.tif",
                         NewWidth,
                         NewHeight,
                         BandNum,
                         dataType,  
                         papszOptions);

	double Geo[6];
	Geo[0] = GDAL_example.geos[0];
	Geo[1] = GDAL_example.geos[1];
	Geo[2] = GDAL_example.geos[2];
	Geo[3] = GDAL_example.geos[3];
	Geo[4] = GDAL_example.geos[4];
	Geo[5] = GDAL_example.geos[5];

	char *pszProjection = GDAL_example.projection_info;
	char *pszPrettyWkt = NULL; //设置为第一幅图的投影,如果与原图不同,请跳过这个if部分,到下个部分直接设置投影 //设置投影,如果需要特殊投影,请找到wkt串,自行建立投影后传入 
	//OGRSpatialReferenceH hSRS;
	//hSRS = OSRNewSpatialReference(NULL);

	//if(OSRImportFromWkt(hSRS, &pszProjection) == CE_None) 
	//{ 
	//	OSRExportToPrettyWkt(hSRS, &pszPrettyWkt, FALSE); 
	//	OutDs->SetProjection(pszPrettyWkt);
	//}

	//关键是这个data的数据
	short *data = new short[NewWidth*NewHeight];
	//初始化新场景的每个高程点为0
	for(int i = 0;i<NewWidth;++i)
	{
		for(int j = 0;j<NewHeight;++j)
		{
			//int pos = i*1000+j;
			data[i*NewWidth+j] = 0;
		}
	}
	NewPatch = (patch *)malloc(patch_size*patch_size*sizeof(struct patch));


	//centerPatchPos   先假设插入的patch坐标为（10，8）（patch坐标意义下）
	int x0 = patch_width*10;
	int y0 = patch_height*8;
	int x0Old = centerPatchPos[2].X;
	int y0Old = centerPatchPos[2].Y;
	for(int i = 0; i < patch_width; ++i)
	{
		for(int j = 0; j < patch_height; ++j)
		{
			int xNew = x0 + i;
			int yNew = y0 + j;
			

			int xxOld = patch_width*x0Old + i;
			int yyOld = patch_height*y0Old + j;
			
			data[NewWidth*xNew + yNew] = P3D1[nWidth*xxOld + yyOld].H;//把最显著的那块Patch先插入新场景中

		}
	}
	NewPatch[patch_size*10+8].IsHandled = 1;


	for(int i = 0; i < mouintainPatch_cluster[2].size(); ++i)
	{
		if(i==2) continue;
		int xOld = mouintainPatch_cluster[2][i].X;
		int yOld = mouintainPatch_cluster[2][i].Y;

		int xdis = xOld - x0Old;
		int ydis = yOld - y0Old;

		int xx = xdis + 10;
		int yy = ydis + 8;

		for(int k = 0; k < patch_width; ++k)
		{
			for(int j = 0; j < patch_height; ++j)
			{
				int xNew = patch_width*xx + k;
				int yNew = patch_height*yy + j;

				int xxOld = patch_width*xOld + k;
				int yyOld = patch_height*yOld + j;
			
				data[NewWidth*xNew + yNew] = P3D1[nWidth*xxOld + yyOld].H;

			}
		}

		NewPatch[patch_size*xx+yy].IsHandled = 1;
	}

	float sum_middle_freqPatch = 0;
	float max_middle = 0;
	float min_middle = 1000000;
	for(int i = 0; i < middle_freqPatch.size(); ++i)
	{
		int tmpX = middle_freqPatch[i].X;
		int tmpY = middle_freqPatch[i].Y;

		patch tmp = Patch[patch_size*tmpX + tmpY];

		sum_middle_freqPatch += tmp.average_height;

		if(max_middle < tmp.average_height)
			max_middle = tmp.average_height;
		if(min_middle > tmp.average_height)
			min_middle = tmp.average_height;
	}



	float aveH_middle_freqPatch = sum_middle_freqPatch / middle_freqPatch.size();

	static int nDx[]= {-1,0,1,0}; //;   {1, 1, 0,-1,-1,-1, 0, 1}
	static int nDy[]= {0,1,0,-1}; //;   {0, 1, 1, 1,0 ,-1,-1,-1}
	for(int i = 0; i < mouintainPatch_cluster[2].size(); ++i)
	{
		int nCurrX = mouintainPatch_cluster[2][i].X - x0Old + 10;
		int nCurrY = mouintainPatch_cluster[2][i].Y - y0Old + 8;
		//4邻域
		for(int k = 0; k<4; ++k)
		{
			int xx = nCurrX+nDx[k];
			int yy = nCurrY+nDy[k];
			if(NewPatch[patch_size*xx + yy].IsHandled == 1)
				continue;

			float r = rand() / double(RAND_MAX);
			float h1 = (max_middle - min_middle);
			float h2 = min_middle - aveH_middle_freqPatch;

			float add = (h1 + h2)*r;
			NewPatch[patch_size*xx + yy].average_height = aveH_middle_freqPatch + add;
			for(int m = 0; m < patch_width; ++m)
			{
				for(int l = 0; l < patch_height; ++l)
				{
					int xNew = patch_width*xx + m;
					int yNew = patch_height*yy + l;		
					data[NewWidth*xNew + yNew] = NewPatch[patch_size*xx + yy].average_height;
				}
			} 

		}

		module::Perlin myModule;
		double value = myModule.GetValue (1.25, 0.75, 0.50);

	}


	//求新场景中这块Patch中高度最高的点的坐标
	int NewMaxHeight = 0;
	int NewMinHeight;
    point NewCenterPos;

	for(int i = 0; i < patch_width; ++i)
	{
		for(int j = 0; j < patch_height; ++j)
		{
			int xNew = x0 + i;
			int yNew = y0 + j;
			if(NewMaxHeight < data[NewWidth*xNew + yNew])
			{
				NewMaxHeight = data[NewWidth*xNew + yNew];
				NewCenterPos.X = xNew;
				NewCenterPos.Y = yNew;
			}
		}
	}










	/*
	for(int i = 0;i<NewWidth;++i)
	{
		for(int j = 0;j<NewHight;++j)
		{
			//int pos = i*1000+j;
			data[i*NewWidth+j] = P3D1[i*1000+j].H;//用原来的DEM的数据
		}
	}
	*/
	//设置坐标 
	//OutDs->SetGeoTransform(Geo);
	//CPLFree(pszPrettyWkt);


	//写入数据到outds中
	GDALRasterBand *outBand = OutDs->GetRasterBand(1);     
	outBand->RasterIO(GF_Write,
                      0,
                      0,
					  NewWidth,
                      NewHeight,
                      data,
                      NewWidth,
                      NewHeight,
                      dataType,
                      0,
                      0);
	GDALClose(OutDs);
}

/*原来的DEM生成函数
void CFluidsimView::GenerateNewDem()
{
	GDALAllRegister();//注册类型,读取写入任何类型影像必须加入此句
	GDALDriver *poDriver;
	const char *pszFormat = "GTiff";
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);


	int BandNum = 1;
	GDALDataType dataType = GDAL_example.DATAType;
	int NewWidth = 1000;
	int NewHight = 1000;

	if(poDriver == NULL) 
	{ 
		return;
	}

	GDALDataset *OutDs;
	char **papszOptions = NULL;
	char **papszMetadata; //这里的参数全是指tif格式的参数,如果是其他格式,请把这里所有注释掉,或者参照文档,自行设定 //设置压缩类型,envi只认得packbits压缩.
	//papszOptions = CSLSetNameValue( papszOptions, "COMPRESS", "PACKBITS" ); //设置压缩比,可以不用,有些时候压缩反而更大,因为是无损的,除非图片很大
	//papszOptions = CSLSetNameValue( papszOptions, "ZLEVEL", "9" ); //设置bsq或者BIP存储 bsq:BAND,bip:PIXEL
	papszOptions = CSLSetNameValue(papszOptions, "INTERLEAVE", "BAND");
	OutDs = poDriver->Create("mountain/sythesis11111.tif",
                         NewWidth,
                         NewHight,
                         BandNum,
                         dataType,  
                         papszOptions);

	double Geo[6];
	Geo[0] = GDAL_example.geos[0];
	Geo[1] = GDAL_example.geos[1];
	Geo[2] = GDAL_example.geos[2];
	Geo[3] = GDAL_example.geos[3];
	Geo[4] = GDAL_example.geos[4];
	Geo[5] = GDAL_example.geos[5];

	char *pszProjection = GDAL_example.projection_info;
	char *pszPrettyWkt = NULL; //设置为第一幅图的投影,如果与原图不同,请跳过这个if部分,到下个部分直接设置投影 //设置投影,如果需要特殊投影,请找到wkt串,自行建立投影后传入 
	//OGRSpatialReferenceH hSRS;
	//hSRS = OSRNewSpatialReference(NULL);

	//if(OSRImportFromWkt(hSRS, &pszProjection) == CE_None) 
	//{ 
	//	OSRExportToPrettyWkt(hSRS, &pszPrettyWkt, FALSE); 
	//	OutDs->SetProjection(pszPrettyWkt);
	//}


	short *data = new short[NewWidth*NewHight];
	//int i = 0 ,j = 0;
	for(int i = 0;i<NewWidth;++i)
	{
		for(int j = 0;j<NewHight;++j)
		{
			//int pos = i*1000+j;
			data[i*NewWidth+j] = P3D1[i*1000+j].H;
		}
	}
	//设置坐标 
	//OutDs->SetGeoTransform(Geo);
	//CPLFree(pszPrettyWkt);


	//写入数据到outds中
	GDALRasterBand *outBand = OutDs->GetRasterBand(1);
	outBand->RasterIO(GF_Write,
                      0,
                      0,
					  NewWidth,
                      NewHight,
                      data,
                      NewWidth,
                      NewHight,
                      dataType,
                      0,
                      0);
	GDALClose(OutDs);
}

*/

void CFluidsimView::OnNewDemSythesis()
{
	GenerateNewDem();
	AfxMessageBox(_T("合成完毕"));

}

void CFluidsimView::TifSaveGenerateNewDem(char *filename,int WidthAll,int HightAll, GDALDataType dataType, char *pszProjection ,short *PatchData)
{
	GDALAllRegister();//注册类型,读取写入任何类型影像必须加入此句
	GDALDriver *poDriver;
	const char *pszFormat = "GTiff";
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);

	int BandNum = 1;

	if(poDriver == NULL) 
	{ 
		return;
	}
	GDALDataset *OutDs;
	char **papszOptions = NULL;
	char **papszMetadata;

	papszOptions = CSLSetNameValue(papszOptions, "INTERLEAVE", "BAND");
	OutDs = poDriver->Create(filename,
                         WidthAll,
                         HightAll,
                         BandNum,
                         dataType,  
                         papszOptions);
	//short *data = new short[WidthAll*HightAll];
	/*
	for(int i = 0;i<WidthAll;++i)
	{
		for(int j = 0;j<HightAll;++j)
		{
			data[i*WidthAll+j] = PatchData[i*WidthAll+j].H;
		}
	}
	*/
	char *pszPrettyWkt = NULL;
	//写入数据到outds中
	GDALRasterBand *outBand = OutDs->GetRasterBand(1);
	outBand->RasterIO(GF_Write,
                      0,
                      0,
                      WidthAll,
                      HightAll,
                      PatchData,
                      WidthAll,
                      HightAll,
                      dataType,
                      0,
                      0);
	GDALClose(OutDs);
	//cv::kmeans();
}


void CFluidsimView::OnMountainPatchCluster()
{
	//计数
	int count = 0;
	for(int i = 0;i < patch_size; i++)
		for(int j = 0; j < patch_size; j++)
			if(Patch[i*patch_size+j].label== 1)
				count++;

	//记录标签为mountain的patch的位置
	P_moun = (patch_mountain *)malloc(count*sizeof(struct patch_mountain));//动态声明结构体数组
	int kkk = 0;
	for(int i = 0; i < patch_size; i++)
	{
		for(int j = 0; j < patch_size; j++)
		{
			if(Patch[i*patch_size+j].label == 1)
			{
				P_moun[kkk].X = Patch[i*patch_size+j].patch_X;  //Patch[i*patch_size+j].patch_X;
				P_moun[kkk].Y = Patch[i*patch_size+j].patch_Y;  //Patch[i*patch_size+j].patch_Y;
				P_moun[kkk].label = 0;
				++kkk;
			}
		}
	}
	int cluster_count;
	//KMeans(P_moun,count,cluster_count,80000.0f);

	char *filename  = "patch_mountain.txt";
	ofstream outFile(filename, ios_base::out);  //按新建或覆盖方式写入  
    // 写入数据  

	for (int r = 0; r < count; ++r)         //row
	{
		int tmp1 = P_moun[r].X;    //读取数据，at<type> - type 是矩阵元素的具体数据格式  
		int tmp2 = P_moun[r].Y;
		outFile << tmp1 << "," << tmp2 ;   //每列数据用 tab 隔开  
	
		outFile << endl;  //换行 
	}		
	outFile.close();



	//按4邻域进行聚类
	std::vector<std::vector<point>> patches;
	for(int i = 0; i < 20; ++i)
		patches.push_back(std::vector<point>());

	int sign=1;
	regionflag = new int [patch_size*patch_size];
	for(int i = 0; i<patch_size; ++i)
		for(int j = 0; j<patch_size; ++j)
			regionflag[i*patch_size+j] = 0;

	for(int i=0; i < patch_size; ++i)
		for(int j=0; j < patch_size; ++j)
		{
			if (Patch[i*patch_size+j].label == 1 && regionflag[i*patch_size+j] == 0)
			{
				RegionGrow(i,j,sign,patches[sign-1]);
				sign++;
			}		
		}
	sign--;

	mouintainPatch_cluster = patches;

	//随机产生颜色
	CvRNG rng = CvRNG(-1);
	for(int i = 0; i < sign; ++i)
    {
		int r = cvRandInt(&rng) % 255;
		int g = cvRandInt(&rng) % 255;
		int b = cvRandInt(&rng) % 255;
		for(int j = 0; j < patches[i].size(); ++j)
		{
			int xxx = patches[i][j].X;
			int yyy = patches[i][j].Y;
			int x = xxx*patch_width;
			int y = yyy*patch_height;
			for(int k = 0; k<patch_width; ++k)
				for(int l = 0; l <patch_height;++l)
				{
					int pos = (x+k)*nWidth+(y+l);
					P3D1[pos].R = r;
					P3D1[pos].G = g;
					P3D1[pos].B = b;
				}
		}	    	
	} 	
}


void CFluidsimView::OnCalAllFeatures()
{
	CalPatchAverageHeight(P3D1);
	CalPatchVariance(P3D1);
	CalPatchRelief(P3D1);
}


void CFluidsimView::RegionGrow(int nSeedX,int nSeedY,int sign, std::vector<point> &pointer)
{
	static int nDx[]= {-1,0,1,0}; //;   {1, 1, 0,-1,-1,-1, 0, 1}
	static int nDy[]= {0,1,0,-1}; //;   {0, 1, 1, 1,0 ,-1,-1,-1}
	
	/*用 patch_size 代替
	int nHeight,nWidth;
	CvSize imgtmpsize;
	imgtmpsize = cvGetSize( img2);
	nHeight = imgtmpsize.height;
	nWidth = imgtmpsize.width;
	*/

	// 定义堆栈，存储坐标
	int *pnGrowQueX ;
	int *pnGrowQueY ;
	
	point v;   //Vector v(nSeedX, nSeedY);
	v.X = nSeedX;
	v.Y = nSeedY;
	pointer.push_back(v);

	
	// 分配空间
	pnGrowQueX = new int [patch_size*patch_size];
	pnGrowQueY = new int [patch_size*patch_size];

	
	// 定义堆栈的起点和终点
	// 当nStart=nEnd, 表示堆栈中只有一个点
	int nStart ;
	int nEnd   ;
	//初始化
	nStart = 0 ;
	nEnd   = 0 ;

	// 把种子点的坐标压入栈
	pnGrowQueX[nEnd] = nSeedX;
	pnGrowQueY[nEnd] = nSeedY;

	regionflag[nSeedX*patch_size+nSeedY] = sign;

	// 当前正在处理的Patch
	int nCurrX ;
	int nCurrY ;

	// 循环控制变量
	int k ;

	// Patch所在图象中的横纵坐标,用来对当前Patch的4邻域进行遍历
	int xx;
	int yy;

	while (nStart<=nEnd)
	{
		// 当前种子点的坐标


		nCurrX = pnGrowQueX[nStart];
		nCurrY = pnGrowQueY[nStart];		

		// 对当前点的4邻域进行遍历
		for (k=0; k < 4; k++)	
		{	
			//4邻域象素的坐标
			xx = nCurrX+nDx[k];
			yy = nCurrY+nDy[k];

			if ( xx < patch_size && xx >= 0  
				 && yy < patch_size && yy >= 0 
				 && Patch[xx*patch_size+yy].label == 1 
				 && regionflag[xx*patch_size+yy] == 0) // && layerflag[yy*nWidth+xx] ==layerflag[nSeedY*nWidth+nSeedX]
			{
	
				nEnd++;// 堆栈的尾部指针后移一位
					
				pnGrowQueX[nEnd] = xx;	// patch(xx，yy) 压入栈
				pnGrowQueY[nEnd] = yy;

				regionflag[xx*patch_size+yy] =  sign;
				//v(xx,yy);
				v.X = xx;
				v.Y = yy;
				pointer.push_back(v);			
			}
		}
		nStart++;
	}
	// 释放内存
	delete []pnGrowQueX;
	delete []pnGrowQueY;
    pnGrowQueX = NULL ;
	pnGrowQueY = NULL ;
}

void CFluidsimView::OnCalMountainPatchFeatures()
{
	//mouintainPatch_cluster;
	std::vector<point> centerPoint;      //每一组patch内平均高度和方差最为显著的那个patch中的高程最高的那个点
	//std::vector<point> centerPatchPos;   //每一组patch内平均高度和方差最为显著的那个patch的patch坐标，20*20patch尺度下
	
	for(int i = 0; i < mouintainPatch_cluster.size();++i)
	{
		std::vector<point> tmp = mouintainPatch_cluster[i];
		if(tmp.size() == 0)
			break;

		point v;
		v.X = tmp[0].X;
		v.Y = tmp[0].Y;

		float tmp_aveHeight = 0;   //patch内平均高程
		float tmp_pVariance = 0;   //patch的方差



		for(int j = 0; j < tmp.size();++j)
		{
			
			int x = tmp[j].X;
			int y = tmp[j].Y;
			float aa = Patch[x*patch_size+y].average_height;   //patch内平均高程
			float bb = Patch[x*patch_size+y].patch_Variance;   //patch的方差
			if( (0.5*tmp_aveHeight + 0.5*tmp_pVariance) < (0.5*aa + 0.5*bb) )
			{
				tmp_aveHeight = aa;
				tmp_pVariance = bb;
				v.X = x;
				v.Y = y;
			}
		}
		centerPatchPos.push_back(v);
		//找该Patch内最高的那个点 x y是目前找出的平均高度和方差最为显著的patch的坐标
		float max_height = 0;
		int max_point_x;
		int max_point_y;
		for(int m = 0; m < patch_width; m++)
		{
				for(int n = 0; n <patch_height; n++)
				{
					int row = patch_width*v.X + m;
					int column = patch_height*v.Y + n;
					int pos = nWidth*row + column;
					if (max_height < P3D1[pos].H)
					{
						max_height = P3D1[pos].H;
						max_point_x = row;
						max_point_y = column;
					}
				}
		}

		v.X = max_point_x;     //v.X*patch_width + 50
		v.Y = max_point_y;    //v.Y*patch_height + 50
		centerPoint.push_back(v);
	}

	


	//计算每个集合的点到该集合中心点的距离
	std::vector<std::vector<float>> cluster_distance;
	for(int i = 0; i<mouintainPatch_cluster.size(); ++i)
		cluster_distance.push_back(std::vector<float>());

	for(int i = 0; i < mouintainPatch_cluster.size();++i)
	{
		std::vector<point> tmp = mouintainPatch_cluster[i];
		
		if(tmp.size() == 0)
			break;
		point center_point_tmp = centerPoint[i];

		for(int j = 0; j < tmp.size();++j)
		{
			
			int x = tmp[j].X*patch_width + 25;
			int y = tmp[j].Y*patch_height + 25;
			float X_distance = (x-center_point_tmp.X)*(x-center_point_tmp.X);   //
			float Y_distance = (y-center_point_tmp.Y)*(y-center_point_tmp.Y);   //   
			float distance = sqrt(X_distance + Y_distance);
			cluster_distance[i].push_back(distance);          //1和3位置处各有9个距离
		}
	}


	float juli_1[29];
	float juli_2[34];

	float fangcha_1[29];
	float fangcha_2[9];

	for(int i = 0; i < 29;++i)
		juli_1[i] = cluster_distance[2][i];

	for(int i = 0; i < 34;++i)
		juli_2[i] = cluster_distance[5][i];

	for(int i = 0; i <29; ++i)
	{
		int tmp_x = mouintainPatch_cluster[2][i].X;
		int tmp_y = mouintainPatch_cluster[2][i].Y;
		float aa = Patch[tmp_x*patch_size + tmp_y].patch_Variance;
		fangcha_1[i] = aa;
	}

	for(int i = 0; i <34; ++i)
	{
		int tmp_x = mouintainPatch_cluster[5][i].X;
		int tmp_y = mouintainPatch_cluster[5][i].Y;
		float bb = Patch[tmp_x*patch_size + tmp_y].patch_Variance;
		fangcha_2[i] = bb;
	}
	///////////////////////////////////////////////////////////////////////////////////
	float ave_height_1[29];
	for(int i = 0; i < 29; ++i)
	{
		int tmp_x = mouintainPatch_cluster[2][i].X;
		int tmp_y = mouintainPatch_cluster[2][i].Y;
		float aa = Patch[tmp_x*patch_size + tmp_y].average_height;
		ave_height_1[i] = aa;
	}

	float ave_height_2[34];
	for(int i = 0; i < 34; ++i)
	{
		int tmp_x = mouintainPatch_cluster[5][i].X;
		int tmp_y = mouintainPatch_cluster[5][i].Y;
		float aa = Patch[tmp_x*patch_size + tmp_y].average_height;
		ave_height_2[i] = aa;
	}
	//////////////////////////////////////////////////////////////////////////////

	//float trainingData_1[9] = {3629798.0,5224846.0,4864548.0,7773395.0,477130.63,5091017.0,5331024.5,2742911.8,311826.50};
	float trainingData_1[29];
	for(int k = 0; k < 29; ++k)
		trainingData_1[k] = juli_1[k];
	//for(int k = 0; k < 9; ++k)
		//trainingData_1[k + 9] = fangcha_2[k];

	float responses_1[29];// = {3629798,5224846,4864548,7773395,477130,5091017,5331024,2742911,311826};
;
	for(int k = 0; k < 29; ++k)
		responses_1[k] = fangcha_1[k];
	//for(int k = 0; k < 9; ++k)
		//responses_1[k + 9] = juli_2[k];

	int responses_1_int[29];
	for(int k = 0; k < 29;++k)
		responses_1_int[k] = responses_1[k];

	for(int k = 0; k < 29;++k)
		responses_1[k] = responses_1_int[k];

	float *trainingData_2 = fangcha_2;
	float *responses_2 = juli_2;


	float trainingData_test[29][2]={{12339.530, 2318.5393}, {13607.689, 1943.4744}, {3781.9065, 2586.7231}, {6870.8091, 2253.2712}, {686.74377, 2739.8020},  
                                {16306.232,1933.1764}, {5777.2671, 2338.5576}, {1664.4569,2338.6904},{588.57202, 2436.8345}, {15876.461, 2222.9141}, {5792.9067, 2674.6421},  
                                {7949.7935,2478.5176}, {1626.4124, 2363.6292}, {15948.693, 2351.1145}, {9803.2861, 2549.2429}, {19332.822, 2400.4512},  
                                {7941.9150,2210.8435}, {5683.3105, 2242.2771}, {31013.320, 2042.0352}, {17742.313, 2290.2275}, {20165.893, 2163.7275},  
                                {35727.395,2063.6299}, {19329.584, 2046.5128}, {9629.5901, 2086.3335}, {15451.046, 2135.8523}, {25006.967, 2017.4596},  
                                {36184.969,1948.3864}, {57125.000, 1867.2928}, {39090.117, 1814.3832} };

	
    CvMat trainingDataCvMat_test = cvMat( 29, 2, CV_32FC1, trainingData_test );  
 
    CvMat responsesCvMat_test = cvMat( 29, 1, CV_32FC1, responses_1 ); 
	
      
    CvRTParams params= CvRTParams(10, 2, 0, false,16, 0, true, 0, 100, 0, CV_TERMCRIT_ITER  );
    CvRTrees rtrees;
    rtrees.train(&trainingDataCvMat_test, CV_ROW_SAMPLE, &responsesCvMat_test, NULL, NULL, NULL, NULL,params);


	float sampleData[2]={14444.530, 2329.0};  
    Mat sampleMat(2, 1, CV_32FC1, sampleData);

    float r = rtrees.predict(sampleMat);

	int tmp = 0;


}


void CFluidsimView::On32833()
{
	//山patch聚类以后，保存数目较多的那几类patch
	//mouintainPatch_cluster;
	std::vector<point> tmp = mouintainPatch_cluster[2];
	for(int i = 0; i < tmp.size(); ++i)
	{
		point v;
		v.X = tmp[i].X;
		v.Y = tmp[i].Y;

		float max_height = 0;
		for(int m = 0; m < patch_width; m++)
		{
				for(int n = 0; n <patch_height; n++)
				{
					int row = patch_width*v.X + m;
					int column = patch_height*v.Y + n;
					int pos = nWidth*row + column;
					if (max_height < P3D1[pos].H)
					{
						max_height = P3D1[pos].H;
					}
				}
		}

		float min_height = max_height;
		for(int m = 0; m < patch_width; m++)
		{
				for(int n = 0; n <patch_height; n++)
				{
					int row = patch_width*v.X + m;
					int column = patch_height*v.Y + n;
					int pos = nWidth*row + column;
					if (min_height > P3D1[pos].H)
					{
						max_height = P3D1[pos].H;
					}
				}
		}


		Mat gray_image(patch_width,patch_height,CV_8UC1);
		for(int m = 0; m < patch_width; m++)
		{
				for(int n = 0; n <patch_height; n++)
				{
					int row = patch_width*v.X + m;
					int column = patch_height*v.Y + n;
					int pos = nWidth*row + column;
					gray_image.at<uchar>(m,n) = int ((float(P3D1[pos].H-min_height)) / (float(max_height-min_height)) * 255);
				}
		}
		string tif_name = "mountain/patchTest_";
		tif_name += Int_to_String(tmp[i].X);
		tif_name += Int_to_String(tmp[i].Y);
	    tif_name += ".bmp";
		imwrite( tif_name, gray_image);
	}

	/*
	int num = 0;
	for(int i = 0; i < patch_size; ++i)
	{
		
		for(int j = 0; j < patch_size; ++j)
		{	
			
			if( P[i*patch_size + j].label == 1)
			{
				num++;
				int x = i*patch_size;     //坐标(x,y);
				int y = j*patch_size;
				string tif_name = "mountain/patch_";
				short patch_data[patch_width*patch_height];
				for(int k = 0;k < patch_width;++k)
				{
					for(int l = 0;l < patch_height;++l)
					{
						int X = x+k;
						int Y = y+l;
						patch_data[k*patch_width+l] = P3D1[X*1000+Y].H;
					}
				}
				tif_name += Int_to_String(num);
				tif_name += ".tif";
				char* pp = (char *)tif_name.data();
				TifSaveGenerateNewDem(pp,patch_width,patch_height,GDAL_example.DATAType,GDAL_example.projection_info,patch_data);
			}

		}
	}

	*/
}


void CFluidsimView::On32834()
{
	//想用分水岭来提取骨架
	Mat image=imread("patchTest_59.bmp");    //载入RGB彩色图像    //  srtm5702.jpg
    imshow("Source Image",image);  
  
    //灰度化，滤波，Canny边缘检测  
    Mat imageGray;  
    cvtColor(image,imageGray,CV_RGB2GRAY);//灰度转换  
    GaussianBlur(imageGray,imageGray,Size(5,5),2);   //高斯滤波  
    imshow("Gray Image",imageGray);   
    Canny(imageGray,imageGray,80,150);    
    imshow("Canny Image",imageGray);

	//查找轮廓  
    vector<vector<Point>> contours;    
    vector<Vec4i> hierarchy;    
    findContours(imageGray,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE,Point());    
    Mat imageContours=Mat::zeros(image.size(),CV_8UC1);  //轮廓     
    Mat marks(image.size(),CV_32S);   //Opencv分水岭第二个矩阵参数  
    marks=Scalar::all(0);

	int index = 0;  
    int compCount = 0;  
    for( ; index >= 0; index = hierarchy[index][0], compCount++ )   
    {  
        //对marks进行标记，对不同区域的轮廓进行编号，相当于设置注水点，有多少轮廓，就有多少注水点  
        drawContours(marks, contours, index, Scalar::all(compCount+1), 1, 8, hierarchy);  
        drawContours(imageContours,contours,index,Scalar(255),1,8,hierarchy);    
    }  

	//我们来看一下传入的矩阵marks里是什么东西  
    Mat marksShows;  
    convertScaleAbs(marks,marksShows);  
    imshow("marksShow",marksShows);  
    imshow("轮廓",imageContours);  
    watershed(image,marks); 

	

}


 void CFluidsimView::OnSaveMountainCluster()
 {
	 char *filename = "mountainCluster_data_new.txt";    //char *filename = "mountainCluster_data.csv";
	 int num = mouintainPatch_cluster.size();
	 for(int i = 0; i < num; ++i)
	 {
		 if(mouintainPatch_cluster[i].size() >= 10)
		 {
			std::vector<point> tmp = mouintainPatch_cluster[i];

			point v;
			v.X = tmp[0].X;
			v.Y = tmp[0].Y;

			float tmp_aveHeight = 0;   //patch内平均高程
			float tmp_pVariance = 0;   //patch的方差
			for(int j = 0; j < tmp.size();++j)
			{
				int x = tmp[j].X;
				int y = tmp[j].Y;
				float aa = Patch[x*patch_size+y].average_height;   //patch内平均高程
				float bb = Patch[x*patch_size+y].patch_Variance;   //patch的方差
				if( (0.5*tmp_aveHeight + 0.5*tmp_pVariance) < (0.5*aa + 0.5*bb) )
				{
					tmp_aveHeight = aa;
					tmp_pVariance = bb;
					v.X = x;
					v.Y = y;
				}
			}
			//找该Patch内最高的那个点 x y是目前找出的平均高度和方差最为显著的patch的坐标
			float max_height = 0;
			int maxH_point_x;
			int maxH_point_y;
			for(int m = 0; m < patch_width; m++)
			{
					for(int n = 0; n <patch_height; n++)
					{
						int row = patch_width*v.X + m;
						int column = patch_height*v.Y + n;
						int pos = nWidth*row + column;
						if (max_height < P3D1[pos].H)
						{
							max_height = P3D1[pos].H;
							maxH_point_x = row;
							maxH_point_y = column;
						}
					}
			}
			v.X = maxH_point_x;     //现在v中存有该patch类中起伏最大的哪个patch中高程最高的那个点
			v.Y = maxH_point_y;     
			
			//计算每个集合的点到该集合中心点的距离
			std::vector<float> Cluster_distance;
			//for(int i = 0; i < tmp.size();++i)
			//{
				for(int j = 0; j < tmp.size();++j)
				{		
					int x = tmp[j].X*patch_width + patch_width/2;
					int y = tmp[j].Y*patch_height + patch_height/2;
					float X_distance = (x-v.X)*(x-v.X);   //
					float Y_distance = (y-v.Y)*(y-v.Y);   //
					float distance = sqrt(X_distance + Y_distance);
					Cluster_distance.push_back(distance);
				}
			//}

			ofstream outFile(filename, ios_base::app);                      //按新建或覆盖方式写入
			for (int r = 0; r <tmp.size(); ++r)        //row
			{  
				int tmpx = tmp[r].X;
				int tmpy = tmp[r].Y;
				int tmp1 = Patch[tmpx*patch_size + tmpy].average_height;    //读取数据，at<type> - type 是矩阵元素的具体数据格式
				int tmp2 = Patch[tmpx*patch_size + tmpy].patch_Variance;    
				int tmp3 = Cluster_distance[r];

				if(r == tmp.size()-1)
				{
					outFile << tmp1 << " " <<tmp2 << " " << tmp3;                          //outFile << tmp1 << "," <<tmp2;
				}
				else
				{
					outFile << tmp1 << " " <<tmp2 << " " << tmp3 << endl;   //每列数据用 tab 隔开       outFile << tmp1 << "," <<tmp2 << endl;
				}
			}
			outFile << endl;
		    outFile.close();
		}
	 }
 }


void CFluidsimView::OnSaveMountainClusterPatch()
{
	int num = mouintainPatch_cluster.size();
	int nnnnnnnnnnnnn = 0;                    //0,65,90,161,246,298,456
	int save_Cluster = 0;
	 for(int i = 0; i < num; ++i)
	 {
		 if(mouintainPatch_cluster[i].size() >= 10)
		 {
			save_Cluster = save_Cluster + 1;
			int size = mouintainPatch_cluster[i].size();
			vector<point> tmp = mouintainPatch_cluster[i];
			for(int j = 0; j < size; ++j)
			{
				int x = tmp[j].X*patch_width;     //坐标(x,y);
				int y = tmp[j].Y*patch_height;
				string tif_name = "mountain/";   //string tif_name = "mountain/patch_";
				short patch_data[patch_width*patch_height];
				for(int k = 0;k < patch_width;++k)
				{
					for(int l = 0;l < patch_height;++l)
					{
						int X = x+k;
						int Y = y+l;
						patch_data[k*patch_width+l] = P3D1[X*1000+Y].H;
					}
				}
				/*
				tif_name += Int_to_String(save_Cluster);
				tif_name += "_";
				tif_name += Int_to_String(j);
				*/
				tif_name += Int_to_String(nnnnnnnnnnnnn++);
				tif_name += ".tif";
				
				char* pp = (char *)tif_name.data();
				TifSaveGenerateNewDem(pp,patch_width,patch_height,GDAL_example.DATAType,GDAL_example.projection_info,patch_data);
				
			 }
		   }
	 }
	
	
	AfxMessageBox(_T("保存完毕"));
}











































 // Perlin Noise Generator
/////////////////////////////////////////////////////////////
float persistence = 0.45f;
int Number_Of_Octaves = 3;


//一个噪声发生器
float Noise1(int x, int y)
{
  x = x % 25;
  y = y % 25;
  int n = x + y * 57;
  n = (n<<13) ^ n;
  return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f); 
}

//一个光滑噪声发生器
float SmoothNoise_1(int x, int y)
{
    float corners = ( Noise1(x-1, y-1)+Noise1(x+1, y-1)+Noise1(x-1, y+1)+Noise1(x+1, y+1) ) / 16.0f;
    float sides   = ( Noise1(x-1, y)  +Noise1(x+1, y)  +Noise1(x, y-1)  +Noise1(x, y+1) ) /  8.0f;
    float center  =  Noise1(x, y) / 4.0f;
    return corners + sides + center;
}

//使用cosin插值函数
float Cosine_Interpolate(float a, float b, float x)
{
    double ft = x * 3.1415927;
    double f = (1 - cos(ft)) * 0.5f;

    return  a*(1-f) + b*f;

}

//插值噪声发生器
float InterpolatedNoise_1(float x, float y)
{

      int integer_X    = int(x);
      float fractional_X = x - integer_X;

      int integer_Y    = int(y);
      float fractional_Y = y - integer_Y;

      float v1 = SmoothNoise_1(integer_X,     integer_Y);
      float v2 = SmoothNoise_1(integer_X + 1, integer_Y);
      float v3 = SmoothNoise_1(integer_X,     integer_Y + 1);
      float v4 = SmoothNoise_1(integer_X + 1, integer_Y + 1);

      float i1 = Cosine_Interpolate(v1 , v2 , fractional_X);
      float i2 = Cosine_Interpolate(v3 , v4 , fractional_X);

      return Cosine_Interpolate(i1 , i2 , fractional_Y);
}


 //最终的PERLIN NOISE
 float PerlinNoise_2D(float x, float y)
 {
      float total = 0.0f;
      float p = persistence;
      int n = Number_Of_Octaves - 1;

      for(int i=0;i<=n;i++)
      {
          float frequency = pow((float)2,i);
          float amplitude = pow(p,i);

          total = total + InterpolatedNoise_1(x * frequency, y * frequency) * amplitude;
      }

      return total;
 } 


int my_cvStdErrReport( int status, const char* func_name, const char* err_msg,
                          const char* file_name, int line, void* userdata )
{
   CString outString;
   //状态由数字转化为语言提示  
   const char *status_char=cvErrorStr(status);//这里可以自己定义转换函数。
   if(status_char)
   outString.Format("出错原因：%s  ",status_char);
  

 //把其他信息添加到CString里面


   //MFC下面弹出提示框
   AfxMessageBox(outString);

  //最好添加错误处理，否则只是弹出提示，程序还是出问题。
   return 1;//返回值跟后续Opencv处理有关。还需要补充
}

 void CFluidsimView::OnFindContours()
 {
	//debug时可以通过下面这个更进一步看到错误
	//设置错误捕捉模式
    cvSetErrMode(CV_ErrModeParent);                                                                                                                                                                                                                                                                                                                                  
    //设置错误时的回调函数
    cvRedirectError(my_cvStdErrReport);



	CString path;
	CFileDialog dlg(TRUE, _T("*.png"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp; *.jpg; *.png) |*.bmp;*.jpg;*.png|All Files (*.*)|*.*||",NULL);
	char title[]= {"Open Source Image"};
	dlg.m_ofn.lpstrTitle= title;
	//IplImage *sourceimg;
	Mat sourceimg;

	if (dlg.DoModal() == IDOK) {
		
		path = dlg.GetPathName();
		//string ss = CT2CA(path.GetBuffer(0));
		char *sss;
		//strcpy(sss,path);
		sss = path.GetBuffer();
		sourceimg = cvLoadImage(sss, CV_LOAD_IMAGE_GRAYSCALE);
		namedWindow("source image",1);
		imshow("source image",sourceimg);
		//cvNamedWindow( "source image", 0);
		//cvShowImage( "source image", sourceimg);
	}

	Mat image;  
    GaussianBlur(sourceimg,image,Size(3,3),0);  
    Canny(image,image,100,250);  
    vector<vector<Point>> contours;  
    vector<Vec4i> hierarchy;  
    findContours(image,contours,hierarchy,RETR_EXTERNAL,CV_CHAIN_APPROX_NONE,Point());  
    Mat imageContours=Mat::zeros(image.size(),CV_8UC1);  
    Mat Contours=Mat::zeros(image.size(),CV_8UC1);  //绘制  
    for(int i=0;i<contours.size();i++)  
    {  
        //contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数  
        for(int j=0;j<contours[i].size();j++)   
        {  
            //绘制出contours向量内所有的像素点  
            Point P=Point(contours[i][j].x,contours[i][j].y);  
            Contours.at<uchar>(P)=255;  
        }  
  
        //输出hierarchy向量内容  
        char ch[256];  
        sprintf(ch,"%d",i);  
        string str=ch;  
        cout<<"向量hierarchy的第" <<str<<" 个元素内容为："<<endl<<hierarchy[i]<<endl<<endl;  
  
        //绘制轮廓  
        drawContours(imageContours,contours,i,Scalar(255),1,8,hierarchy);  
    } 
	namedWindow("Contours Image",1);
    imshow("Contours Image",imageContours); //轮廓  

	Point2f xx = Point2f(500,500);
	double tmp = pointPolygonTest(contours[0],xx,0);

	vector<point> Need2Placed;
	for(int i = 0; i < patch_size; ++i)       //for(int i = 0; i < patch_size; ++i)
	{
		for(int j = 0; j < patch_size; ++j)   //for(int j = 0; j < patch_size; ++j)
		{
			int flag = 0;
			Point2f pos = Point2f(i*patch_width,j*patch_height);//左上角点
			double isIn0 = pointPolygonTest(contours[0],pos,0);

			pos = Point2f(i*patch_width,j*patch_height + 4);//右上角点
			double isIn1 = pointPolygonTest(contours[0],pos,0);

			pos = Point2f(i*patch_width + 4,j*patch_height);//左下角点
			double isIn2 = pointPolygonTest(contours[0],pos,0);

			pos = Point2f(i*patch_width + 4,j*patch_height + 4);//右下角点
			double isIn3 = pointPolygonTest(contours[0],pos,0);

			//统计每一个patch 4个角点是否在用户所画的边界内
			if(isIn0 == 1.0) 
				flag++;
			if(isIn1 == 1.0)
				flag++;
			if(isIn2 == 1.0)
				flag++;
			if(isIn3 == 1.0)
				flag++;

			if(flag > 2)
			{
				point v;
				v.X = i;
				v.Y = j;
				Need2Placed.push_back(v);
			}
		}
	}
	
	//opencv里面和gdal的坐标不太一样，需要做个镜像翻转
	for (int i = 0; i < Need2Placed.size(); ++i)
	{
		int tmp = Need2Placed[i].X;
		Need2Placed[i].X = Need2Placed[i].Y;
		Need2Placed[i].Y = tmp;
	}

	
	point p;
	double avg_px=0,avg_py=0;
	for(int i = 0 ;i < Need2Placed.size(); i++)
	{ 
		avg_px += Need2Placed[i].X;
		avg_py += Need2Placed[i].Y;
	} 
	p.X=avg_px/Need2Placed.size();                          //取到了中心块
	p.Y=avg_py/Need2Placed.size();        

	int x1 = Need2Placed.size();
	int x2 = avg_px;
	int x3 = avg_py;

	
	p.X = p.X*patch_width + 2;                                      //先从中心块中手动指定一点作为新场景中山区的最高点
	p.Y = p.Y*patch_height + 3;

	vector<double> NewHighFreDistance;   //计算用户所画轮廓内覆盖的patch到新场景最高点的距离
	for(int i = 0; i < Need2Placed.size();++i)
	{		
		point tmp = Need2Placed[i];
		int x = tmp.X*patch_width + patch_width/2;
		int y = tmp.Y*patch_height + patch_height/2;
		double X_distance = (x-p.X)*(x-p.X);   //
		double Y_distance = (y-p.Y)*(y-p.Y);   //
		double distance = sqrt(X_distance + Y_distance);
		NewHighFreDistance.push_back(distance);
	}

	ofstream outFile11("NewHighFreDistance.txt",ios_base::out);  //按新建或覆盖方式写入  
	// 写入数据  
	for (int r = 0; r < 1257; r++)         //row
	{
		float tmp = NewHighFreDistance[r];    //读取数据，at<type> - type 是矩阵元素的具体数据格式 
		outFile11 << tmp << endl;
	}		
	outFile11.close();

	
	const char *csvname = "mountainCluster_data_20171020.txt";
	FILE* f = fopen( csvname, "r" );
    if( !f )
    {
        AfxMessageBox("未能打开数据文件!");
    }

	
	double trainingData[4748];                     //距离
	float responsesData[4748];                    //高度
	float responsesData_V[4748];                  //方差
	for(int line = 0; line < 4748; ++line)
	{
        float tmp1, tmp2;
		double tmp3;
		fscanf(f, "%f%f%f,", &tmp1, &tmp2, &tmp3);   //高度，方差，距离
		trainingData[line] = tmp3;               //距离
		responsesData[line] = tmp1;              //高度
		responsesData_V[line] = tmp2;            //方差
    }
    fclose(f);
	
	/*
	int num = 0;
	for(int j = 0; j < 519; ++j)
	{
		if(responsesData[num] > responsesData[j])
			num = j;
	}
	*/


	//建树预测
	double trainingData_int[4748];
	for(int i = 0; i < 4748; ++i)
	{
		trainingData_int[i] = trainingData[i];
	}


	CvMat trainingDataCvMat_test = cvMat( 4748, 1, CV_32FC1, trainingData_int);
	
 
    
	int responsesData_int[4748];
	for(int i = 0; i < 4748; ++i)
	{
		responsesData_int[i] = responsesData[i]*100;
	}
	CvMat responsesCvMat_test = cvMat( 4748, 1, CV_32SC1, responsesData_int ); 


    CvRTParams params= CvRTParams(10, 2, 0, false,16, 0, true, 0, 100, 0, CV_TERMCRIT_ITER );
    CvRTrees rtrees;
    rtrees.train(&trainingDataCvMat_test, CV_ROW_SAMPLE, &responsesCvMat_test, NULL, NULL, NULL, NULL,params);

	vector<float> NewHighFreAvgHeight;
	
	for(int i = 0; i < NewHighFreDistance.size(); ++i)   //for(int i = 0; i < NewHighFreDistance.size(); ++i)
	{
		double sampleData = NewHighFreDistance[i];//
		Mat sampleMat(1, 1, CV_32FC1, sampleData);     
		float rr = rtrees.predict(sampleMat);
		NewHighFreAvgHeight.push_back(rr);
	}

	for(int i = 0; i < NewHighFreAvgHeight.size(); ++i)
	{
		NewHighFreAvgHeight[i] = NewHighFreAvgHeight[i]/100;
	}
	//NewHighFreDistance.clear();

	/*
	ofstream outFile12("11111111111.txt",ios_base::out);  //按新建或覆盖方式写入  
	// 写入数据  
	for (int r = 0; r < 4748; r++)         //row
	{
		float tmp = NewHighFreAvgHeight[r];    //读取数据，at<type> - type 是矩阵元素的具体数据格式 
		outFile12 << tmp << endl;
	}		
	outFile12.close();
	*/
	
	//加一点随机扰动


	
	//找到跟预测出的值最接近的mountain文件夹下的patch进行新场景的填充
	vector<int> SeclectedPatch;
	for(int i = 0; i < NewHighFreAvgHeight.size(); ++i)
	{
		float tmpp = abs(NewHighFreAvgHeight[i] - responsesData[0]);
		int flag = 0;
		for(int j = 0; j < 4748; ++j)
		{
			float tmppp = abs(NewHighFreAvgHeight[i] - responsesData[j]);
			
			if(tmpp > tmppp)
			{
				tmpp = tmppp;
				flag = j;
			}
		}
		SeclectedPatch.push_back(flag);
	}

	
	ofstream outFile15("222222222.txt",ios_base::out);//按新建或覆盖方式写入  
	// 写入数据  
	for (int r = 0; r < NewHighFreAvgHeight.size(); r++)         //row
	{
		outFile15 << NewHighFreAvgHeight[r] << "\t" << NewHighFreDistance[r] <<endl;
	}		
	outFile15.close();
	



	//将选择出的每一块patch的高度赋到新场景中
	GDALAllRegister();//注册类型,读取写入任何类型影像必须加入此句
	GDALDriver *poDriver;
	const char *pszFormat = "GTiff";
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);


	int BandNum = 1;
	GDALDataType dataType = GDAL_example.DATAType;
	int NewWidth = 1000;
	int NewHeight = 1000;

	if(poDriver == NULL) 
	{ 
		return;
	}

	GDALDataset *OutDs;
	char **papszOptions = NULL;
	char **papszMetadata; //这里的参数全是指tif格式的参数,如果是其他格式,请把这里所有注释掉,或者参照文档,自行设定 //设置压缩类型,envi只认得packbits压缩.
	papszOptions = CSLSetNameValue(papszOptions, "INTERLEAVE", "BAND");
	OutDs = poDriver->Create("mountain/sythesis20171025.tif",
                         NewWidth,
                         NewHeight,
                         BandNum,
                         dataType,  
                         papszOptions);

	double Geo[6];
	Geo[0] = GDAL_example.geos[0];
	Geo[1] = GDAL_example.geos[1];
	Geo[2] = GDAL_example.geos[2];
	Geo[3] = GDAL_example.geos[3];
	Geo[5] = GDAL_example.geos[5];
	Geo[4] = GDAL_example.geos[4];

	char *pszProjection = GDAL_example.projection_info;
	char *pszPrettyWkt = NULL; //设置为第一幅图的投影,如果与原图不同,请跳过这个if部分,到下个部分直接设置投影 //设置投影,如果需要特殊投影,请找到wkt串,自行建立投影后传入 


	//关键是这个data的数据
	short *data = new short[NewWidth*NewHeight];
	//初始化新场景的每个高程点为0
	for(int i = 0;i<NewWidth;++i)
	{
		for(int j = 0;j<NewHeight;++j)
		{
			data[i*NewWidth+j] = 0;
		}
	}
	//NewPatch = (patch *)malloc(patch_size*patch_size*sizeof(struct patch));

	int *ishandled = new int[patch_size*patch_size];
	for(int i = 0; i < patch_size; ++i)
		for(int j = 0; j < patch_size; ++j)
			ishandled[patch_size*i + j] = 0;

	
	
	for(int i = 0; i < Need2Placed.size(); ++i)
	{
		string patch_name = "mountain/";
		
		patch_name += Int_to_String(SeclectedPatch[i]);
		patch_name += ".tif";
		char* patch_name_new = (char *)patch_name.data();
		int IsSucceeded = ChooseSelectedPatchIntoNewScene(patch_name_new,5,5,data,Need2Placed[i],i);
		int a = Need2Placed[i].X;
		int b = Need2Placed[i].Y;
		ishandled[patch_size*a+b] = 1;
	}

	/*

	//做平滑 行1
	for(int k = 0; k >=-5 ;--k)
	{
		for(int i = 0; i < nWidth; ++i)
		{
			for(int j = patch_height + k; j <=(nWidth+k - patch_height); j=j+patch_height)
			{
				if(i == 0) //上边界
				{
					int a1 = data[nWidth*i + (j-1)];
					int a2 = data[nWidth*i + j];
					int a3 = data[nWidth*i + (j+1)];
					int a4 = data[nWidth*(i+1) + (j-1)];
					int a5 = data[nWidth*(i+1) + j];
					int a6 = data[nWidth*(i+1) + (j+1)];
					data[nWidth*i + j] = (a1 + a2 + a3 + a4 + a5 + a6) / 6;
				}
				else if(i == 1000-1) //下边界
				{
					int a1 = data[nWidth*i + (j-1)];
					int a2 = data[nWidth*i + j];
					int a3 = data[nWidth*i + (j+1)];
					int a4 = data[nWidth*(i-1) + (j-1)];
					int a5 = data[nWidth*(i-1) + j];
					int a6 = data[nWidth*(i-1) + (j+1)];
					data[nWidth*i + j] = (a1 + a2 + a3 + a4 + a5 + a6) / 6;
				}
				else
				{
					int a1 = data[nWidth*(i-1) + (j-1)];
					int a2 = data[nWidth*(i-1)  + j];
					int a3 = data[nWidth*(i-1)  + (j+1)];
					int a4 = data[nWidth*i + (j-1)];
					int a5 = data[nWidth*i + j];
					int a6 = data[nWidth*i + (j+1)];
					int a7 = data[nWidth*(i+1) + (j-1)];
					int a8 = data[nWidth*(i+1) + j];
					int a9 = data[nWidth*(i+1) + (j+1)];
					data[nWidth*i + j] = (a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9) / 9;
				}
			}
		}
	}
	for(int k = 0; k <=5 ;++k)
	{
		for(int i = 0; i < nWidth; ++i)
		{
			for(int j = patch_height + k; j <=(nHeight + k - patch_height); j=j + patch_height)
			{
				if(i == 0) //上边界
				{
					int a1 = data[nWidth*i + (j-1)];
					int a2 = data[nWidth*i + j];
					int a3 = data[nWidth*i + (j+1)];
					int a4 = data[nWidth*(i+1) + (j-1)];
					int a5 = data[nWidth*(i+1) + j];
					int a6 = data[nWidth*(i+1) + (j+1)];
					data[nWidth*i + j] = (a1 + a2 + a3 + a4 + a5 + a6) / 6;
				}
				else if(i == nWidth-1) //下边界
				{
					int a1 = data[nWidth*i + (j-1)];
					int a2 = data[nWidth*i + j];
					int a3 = data[nWidth*i + (j+1)];
					int a4 = data[nWidth*(i-1) + (j-1)];
					int a5 = data[nWidth*(i-1) + j];
					int a6 = data[nWidth*(i-1) + (j+1)];
					data[nWidth*i + j] = (a1 + a2 + a3 + a4 + a5 + a6) / 6;
				}
				else
				{
					int a1 = data[nWidth*(i-1) + (j-1)];
					int a2 = data[nWidth*(i-1)  + j];
					int a3 = data[nWidth*(i-1)  + (j+1)];
					int a4 = data[nWidth*i + (j-1)];
					int a5 = data[nWidth*i + j];
					int a6 = data[nWidth*i + (j+1)];
					int a7 = data[nWidth*(i+1) + (j-1)];
					int a8 = data[nWidth*(i+1) + j];
					int a9 = data[nWidth*(i+1) + (j+1)];
					data[nWidth*i + j] = (a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9) / 9;
				}
			}
		}
	}
	
	//做平滑 列1
	for(int k = 0; k >=-5; --k)
	{
		for(int i = patch_width+k; i <=(nWidth + k - patch_width); i=i+patch_width)                 
		{
			for(int j = 0; j < nHeight; ++j)
			{
				if(j == 0) //左边界
				{
					int a1 = data[nWidth*(i-1) + j];
					int a2 = data[nWidth*i + j];
					int a3 = data[nWidth*(i+1) + j];
					int a4 = data[nWidth*(i-1) + (j+1)];
					int a5 = data[nWidth*i + (j+1)];
					int a6 = data[nWidth*(i+1) + (j+1)];
					data[1000*i + j] = (a1 + a2 + a3 + a4 + a5 + a6) / 6;
				}
				else if(i == nWidth-1) //右边界
				{
					int a1 = data[nWidth*(i-1) + j];
					int a2 = data[nWidth*i + j];
					int a3 = data[nWidth*(i+1) + j];
					int a4 = data[nWidth*(i-1) + (j-1)];
					int a5 = data[nWidth*i + (j-1)];
					int a6 = data[nWidth*(i+1) + (j-1)];
					data[nWidth*i + j] = (a1 + a2 + a3 + a4 + a5 + a6) / 6;
				}
				else
				{
					int a1 = data[nWidth*(i-1) + (j-1)];
					int a2 = data[nWidth*i  + (j-1)];
					int a3 = data[nWidth*(i+1)  + (j-1)];
					int a4 = data[nWidth*(i-1) + j];
					int a5 = data[nWidth*i + j];
					int a6 = data[nWidth*(i+1) + j];
					int a7 = data[nWidth*(i-1) + (j+1)];
					int a8 = data[nWidth*i + (j+1)];
					int a9 = data[nWidth*(i+1) + (j+1)];
					data[nWidth*i + j] = (a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9) / 9;
				}
			}
		}
	}
	for(int k = 0; k <=5; ++k)
	{
		for(int i = patch_width + k; i <=(nWidth+k - patch_width); i=i + patch_width)                 
		{
			for(int j = 0; j < nHeight; ++j)
			{
				if(j == 0) //左边界
				{
					int a1 = data[nWidth*(i-1) + j];
					int a2 = data[nWidth*i + j];
					int a3 = data[nWidth*(i+1) + j];
					int a4 = data[nWidth*(i-1) + (j+1)];
					int a5 = data[nWidth*i + (j+1)];
					int a6 = data[nWidth*(i+1) + (j+1)];
					data[nWidth*i + j] = (a1 + a2 + a3 + a4 + a5 + a6) / 6;
				}
				else if(i == 1000-1) //右边界
				{
					int a1 = data[nWidth*(i-1) + j];
					int a2 = data[nWidth*i + j];
					int a3 = data[nWidth*(i+1) + j];
					int a4 = data[nWidth*(i-1) + (j-1)];
					int a5 = data[nWidth*i + (j-1)];
					int a6 = data[nWidth*(i+1) + (j-1)];
					data[nWidth*i + j] = (a1 + a2 + a3 + a4 + a5 + a6) / 6;
				}
				else
				{
					int a1 = data[nWidth*(i-1) + (j-1)];
					int a2 = data[nWidth*i  + (j-1)];
					int a3 = data[nWidth*(i+1)  + (j-1)];
					int a4 = data[nWidth*(i-1) + j];
					int a5 = data[nWidth*i + j];
					int a6 = data[nWidth*(i+1) + j];
					int a7 = data[nWidth*(i-1) + (j+1)];
					int a8 = data[nWidth*i + (j+1)];
					int a9 = data[nWidth*(i+1) + (j+1)];
					data[nWidth*i + j] = (a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9) / 9;
				}
			}
		}
	}
	*/


	/*
	vector<point> HouXuanDian;
	for(int i = 0; i < Need2Placed.size(); ++i)
	{
		if(Need2Placed[i].Y == 2 || Need2Placed[i].Y ==12)
			HouXuanDian.push_back(Need2Placed[i]);
	}
	
	

	for(int i = 0; i < patch_size; ++i)
		for(int j = 0; j < patch_size; ++j)
		{
			if(ishandled[patch_size*i+j] == 0)
			{
				srand((unsigned)time(NULL));
				int randnum = (rand() % (HouXuanDian.size()));

				int ooldx0 = HouXuanDian[randnum].X*50;
				int ooldy0 = HouXuanDian[randnum].Y*50;
				int nnewx0 = i*50;
				int nnewy0 = j*50;
				for(int m = 0; m < 50; ++m)
					for(int n = 0; n < 50; ++n)
					{
						data[1000*(nnewx0+m) + (nnewy0+n)] = data[1000*(ooldx0+m) + (ooldy0+n)];
					}
			}

		}
	
	 delete[] ishandled;
	 */


	//写入数据到outds中
	GDALRasterBand *outBand = OutDs->GetRasterBand(1);
	outBand->RasterIO(GF_Write,
                      0,
                      0,
					  NewWidth,
                      NewHeight,
                      data,
                      NewWidth,
                      NewHeight,
                      dataType,
                      0,
                      0);
	GDALClose(OutDs);


	int xxxxxx = 0;

	//int *isHighFrequence = new int[patch_size*patch_size];







	/*
	for(int i = 0; i < patch_size; ++i)
	{
		for(int j = 0; j < patch_size; ++j)
		{
			int n = 0;
			if(

		}

	}
	*/

	//  (patch *)malloc(patch_size*patch_size*sizeof(struct patch));       //动态声明结构体数组




	


	/*
    IplImage* img_temp = cvCreateImage(cvGetSize(sourceimg), 8, 1); 
	IplImage* img_temp2 = cvCreateImage(cvGetSize(sourceimg), 8, 1);
    cvThreshold(sourceimg, img_temp, 50, 255, CV_THRESH_BINARY);

	CvMemStorage* mem_storage = cvCreateMemStorage(0);      
    CvSeq *first_contour = NULL, *c = NULL;

	cvFindContours(img_temp, mem_storage, &first_contour, sizeof(CvContour), CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);//#1 需更改区域    
	//first_contour = cvApproxPoly( first_contour, sizeof(CvContour), mem_storage, CV_POLY_APPROX_DP, 3, 1 );
    
	cvZero(img_temp);      
    cvDrawContours(img_temp, first_contour, cvScalar(255), cvScalar(255), 0,1);                      //#2 需更改区域 
	cvNamedWindow( "contour_test", 0); 
    cvShowImage("contour_test", img_temp); 

	cvCanny(sourceimg,img_temp2,100,255,3);
	cvNamedWindow( "Canny", 0); 
    cvShowImage("Canny", img_temp2);

	

	vector<CvPoint> a;
	//cvCvtSeqToArray(first_contour,);
	int count = first_contour->total;
	for(int i  = 0; i < count; ++i)
	{
		CvPoint t = *CV_GET_SEQ_ELEM(CvPoint,first_contour,i);
		Point2f p = Point2f(t.x,t.y);
		a.push_back(p);
	} 

	double tmp = pointPolygonTest(a,xx,0); 
	*/

	

	//Mat skel = skeletonization(sourceimg);
	
	//imshow("Skeleton Image",skel);
	//imwrite("skeleton.png",skel);
 }


 bool CFluidsimView::ChooseSelectedPatchIntoNewScene(char *filename, int width_patch, int height_patch, short *Data, point position,int rrr)
 {
	int len = strlen(filename);

	GDALAllRegister();
	const char *cstr = filename; 
	GDALDataset *poDataset_patch = (GDALDataset *)GDALOpen(cstr,GA_ReadOnly);
	GDALDataType dataType = poDataset_patch->GetRasterBand(1)->GetRasterDataType();
	

	GDAL_example.DATAType = dataType;
	GDAL_example.projection_info = (char*)(poDataset_patch->GetProjectionRef());
	poDataset_patch->GetGeoTransform(GDAL_example.geos);
	
	
	if( poDataset_patch == NULL )
	{
		AfxMessageBox("Failed To Open DEM File!");  //打开文件失败
	}

	//int i,j,bandNum;
	//nWidth = poDataset->GetRasterXSize();  //影像宽度
	//nHeight = poDataset->GetRasterYSize(); //影像高度
	//bandNum = poDataset->GetRasterCount(); //影像波段数量

	switch (dataType)
	{
		case GDT_UInt16: 
			{
				unsigned short *data_patch = new unsigned short[width_patch*height_patch];
				GDALRasterBand *band = poDataset_patch->GetRasterBand(1);//获取第一波段，波段从1开始
				band->RasterIO( GF_Read, 0, 0, width_patch, height_patch, 
                      data_patch, width_patch, height_patch, dataType, 
                      0, 0 );
				GDALClose(poDataset_patch);

				int x0 = patch_width*position.X;
				int y0 = patch_height*position.Y;

				for(int i = 0; i < patch_width; ++i)
				{
					for(int j = 0; j < patch_height; ++j)
					{
						int xNew = x0 + i;
						int yNew = y0 + j;
						Data[1000*xNew + yNew] = data_patch[patch_width*i + j];//把最显著的那块Patch先插入新场景中
					}
				}

				break;
			}
		case GDT_Int16: 
			{
				short *data_patch = new short[width_patch*height_patch];
				GDALRasterBand *band = poDataset_patch->GetRasterBand(1);//获取第一波段，波段从1开始
				band->RasterIO( GF_Read, 0, 0, width_patch, height_patch, 
                      data_patch, width_patch, height_patch, dataType, 
                      0, 0 );
				GDALClose(poDataset_patch);

				int x0 = patch_width*position.X;
				int y0 = patch_height*position.Y;

				for(int i = 0; i < patch_width; ++i)
				{
					for(int j = 0; j < patch_height; ++j)
					{
						int xNew = x0 + i;
						int yNew = y0 + j;
						Data[200*xNew + yNew] = data_patch[patch_width*i + j];//把最显著的那块Patch先插入新场景中
						int xxxxxxxxxxxxxxxxxxx = 0;
					}
				}

				break;
			}
	}
	
	
	ofstream outFile13("222222222.txt",ios_base::app);//按新建或覆盖方式写入  
	// 写入数据  
	//读取数据，at<type> - type 是矩阵元素的具体数据格式 
	outFile13 << rrr << endl;		
	outFile13.close();

	return 1;
 }

 int dir[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
 void CFluidsimView::dfs(int x,int y)
 {
	patch & p = Patch[x*patch_size+y]; // current patch
	for(int i = 0 ;i < 4; ++i)
	{
		int nx = x + dir[i][0];
		int ny = y + dir[i][1];
		if(nx>=0 && nx<patch_size && ny>=0 && ny<patch_size)
		{
			patch & np = Patch[nx*patch_size+ny];  //next patch
			if(np.flag ==-1  && np.label==1)
			{
				np.flag = p.flag;
				dfs(nx,ny);
			}
		}		
	}
 }
 void CFluidsimView::OnNewCluster()
 {
	 int idx = 0;
	 for(int i = 0 ;i < patch_size*patch_size; ++i)
	 {
		if(Patch[i].flag == -1 && Patch[i].label==1)
		{
			Patch[i].flag = idx;
			idx++;
			dfs(Patch[i].patch_X,Patch[i].patch_Y);
		}	
	 }

	 /*
	ofstream outFile("NewCluster.txt", ios_base::out);  //按新建或覆盖方式写入  
	 // 写入数据  
	for (int r = 0; r < patch_size; ++r)                 //row
	{
		for (int c = 0; c < patch_size; ++c)             //column
		{  
			int tmp = Patch[r*patch_size+c].flag;        //读取数据，at<type> - type 是矩阵元素的具体数据格式  
			outFile << tmp << "\t" ;                     //每列数据用 tab 隔开  
		}
		outFile << endl;  //换行 
	}
	 outFile.close();
	 AfxMessageBox(_T("保存完毕"));*/

	 int maxNum = 0;
	 for(int i = 0; i<patch_size*patch_size; ++i)
	 {
		if(Patch[i].flag > maxNum)
			maxNum = Patch[i].flag;
	 }
	 maxNum++;
	 int x = maxNum;

	 int Num[348]; //(int *)malloc(x*x*sizeof(int)); //Patch = (patch *)malloc(patch_size*patch_size*sizeof(struct patch));       //动态声明结构体数组
	 for(int i = 0; i<348; ++i)
		 Num[i] = 0;
	 for(int i = 0; i<patch_size*patch_size; ++i)
	 {
		 if(Patch[i].flag > 1)
		 {
			 Num[Patch[i].flag]++;
		 }
	 }

	 vector<vector<point>> cluster;
	 for(int i = 0; i < 2; ++i)
		 cluster.push_back(std::vector<point>());

	 for(int i = 0; i < patch_size; ++i)
	 {
		 for(int j = 0; j < patch_size; ++j)
		 {
			 if(Patch[i*patch_size+j].flag == 163)
			 {
				 point tmp;
				 tmp.X = i; 
				 tmp.Y = j;
				 cluster[0].push_back(tmp);
			 }

		 }	 
	 }
	 /*
	 for(int i = 0; i < patch_size; ++i)
	 {
		 for(int j = 0; j < patch_size; ++j)
		 {
			 if(Patch[i*patch_size+j].flag == 42)
			 {
				 point tmp;
				 tmp.X = i; 
				 tmp.Y = j;
				 cluster[1].push_back(tmp);
			 }

		 }	 
	 }*/
	 mouintainPatch_cluster = cluster;

	 char *filename = "mountainCluster_data_20171020.txt";    //char *filename = "mountainCluster_data.csv";
	 int num = cluster.size();
	 //for(int i = 0; i < num; ++i)
	 //{
		
			 std::vector<point> tmp = cluster[0];

			 point v;
			 v.X = tmp[0].X;
			 v.Y = tmp[0].Y;

			 float tmp_aveHeight = 0;   //patch内平均高程
			 float tmp_pVariance = 0;   //patch的方差
			 for(int j = 0; j < tmp.size();++j)
			 {
				 int x = tmp[j].X;
				 int y = tmp[j].Y;
				 float aa = Patch[x*patch_size+y].average_height;   //patch内平均高程
				 float bb = Patch[x*patch_size+y].patch_Variance;   //patch的方差
				 if( (0.6*tmp_aveHeight + 0.4*tmp_pVariance) < (0.6*aa + 0.4*bb) )
				 {
					 tmp_aveHeight = aa;
					 tmp_pVariance = bb;
					 v.X = x;
					 v.Y = y;
				 }
			 }
			 //找该Patch内最高的那个点，(x和y是目前找出的平均高度和方差最为显著的patch的坐标)
			 float max_height = 0;
			 int maxH_point_x;
			 int maxH_point_y;
			 for(int m = 0; m < patch_width; m++)
			 {
				 for(int n = 0; n <patch_height; n++)
				 {
					 int row = patch_width*v.X + m;
					 int column = patch_height*v.Y + n;
					 int pos = nWidth*row + column;
					 if (max_height < P3D1[pos].H)
					 {
						 max_height = P3D1[pos].H;
						 maxH_point_x = row;
						 maxH_point_y = column;
					 }
				 }
			 }
			 v.X = maxH_point_x;     //现在v中存有该patch类中起伏最大的哪个patch中高程最高的那个点
			 v.Y = maxH_point_y;     

			 //计算每个集合的点到该集合中心点的距离
			 std::vector<float> Cluster_distance;
			 for(int j = 0; j < tmp.size();++j)
			 {		
				 int x = tmp[j].X*patch_width + patch_width/2;
				 int y = tmp[j].Y*patch_height + patch_height/2;
				 float X_distance = (x-v.X)*(x-v.X);   //
				 float Y_distance = (y-v.Y)*(y-v.Y);   //
				 float distance = sqrt(X_distance + Y_distance);
				 Cluster_distance.push_back(distance);
			 }
			 
			 
			 ofstream outFile(filename, ios_base::app);                      //按新建或覆盖方式写入
			 for (int r = 0; r <tmp.size(); ++r)        //row
			 {  
				 int tmpx = tmp[r].X;
				 int tmpy = tmp[r].Y;
				 float tmp1 = Patch[tmpx*patch_size + tmpy].average_height;    //读取数据，at<type> - type 是矩阵元素的具体数据格式
				 float tmp2 = Patch[tmpx*patch_size + tmpy].patch_Variance;    
				 float tmp3 = Cluster_distance[r];

				 if(r == tmp.size()-1)
				 {
					 outFile << tmp1 << " " <<tmp2 << " " << tmp3;                          //outFile << tmp1 << "," <<tmp2;
				 }
				 else
				 {
					 outFile << tmp1 << " " <<tmp2 << " " << tmp3 << endl;   //每列数据用 tab 隔开       outFile << tmp1 << "," <<tmp2 << endl;
				 }
			 }
			 outFile << endl;
			 outFile.close();
			 
	//}
	 
	 
	 int xxx = 0;

 }


 void CFluidsimView::Onsize100()
 {
	int w = 200;
	int h = 200;

	point centerP;
	centerP.X = 13;
	centerP.Y = 5;

	centerP.X = centerP.X*patch_width + 2;                                      //先从中心块中手动指定一点作为新场景中山区的最高点
	centerP.Y = centerP.Y*patch_height + 3;


	int patch_size_n = w/patch_width;

	vector<point> Need2Placed;
	for(int i = 0; i < patch_size_n; ++i)
	{
		for(int j = 0; j < patch_size_n; ++j)
		{
			point v;
			v.X = i;
			v.Y = j;

			Need2Placed.push_back(v);

		}
	}


	vector<double> NewHighFreDistance;   //计算用户所画轮廓内覆盖的patch到新场景最高点的距离

	for(int i = 0; i < Need2Placed.size();++i)
	{		
		point tmp = Need2Placed[i];
		int x = tmp.X*patch_width + patch_width/2;
		int y = tmp.Y*patch_height + patch_height/2;
		double X_distance = (x-centerP.X)*(x-centerP.X);   //
		double Y_distance = (y-centerP.Y)*(y-centerP.Y);   //
		double distance = sqrt(X_distance + Y_distance);
		NewHighFreDistance.push_back(distance);
	}

	/*
	ofstream outFile11("NewHighFreDistance.txt",ios_base::out);  //按新建或覆盖方式写入  
	// 写入数据  
	for (int r = 0; r < 1257; r++)         //row
	{
		float tmp = NewHighFreDistance[r];    //读取数据，at<type> - type 是矩阵元素的具体数据格式 
		outFile11 << tmp << endl;
	}		
	outFile11.close();*/

	
	const char *csvname = "mountainCluster_data_20171020.txt";
	FILE* f = fopen( csvname, "r" );
    if( !f )
    {
        AfxMessageBox("未能打开数据文件!");
    }

	
	double trainingData[4748];                     //距离
	float responsesData[4748];                    //高度
	float responsesData_V[4748];                  //方差
	for(int line = 0; line < 4748; ++line)
	{
        float tmp1, tmp2;
		double tmp3;
		fscanf(f, "%f%f%f,", &tmp1, &tmp2, &tmp3);   //高度，方差，距离
		trainingData[line] = tmp3;               //距离
		responsesData[line] = tmp1;              //高度
		responsesData_V[line] = tmp2;            //方差
    }
    fclose(f);
	
	/*
	int num = 0;
	for(int j = 0; j < 519; ++j)
	{
		if(responsesData[num] > responsesData[j])
			num = j;
	}
	*/


	//建树预测
	double trainingData_int[4748];
	for(int i = 0; i < 4748; ++i)
	{
		trainingData_int[i] = trainingData[i];
	}


	CvMat trainingDataCvMat_test = cvMat( 4748, 1, CV_32FC1, trainingData_int);
	
 
    
	int responsesData_int[4748];
	for(int i = 0; i < 4748; ++i)
	{
		responsesData_int[i] = responsesData[i]*100;
	}
	CvMat responsesCvMat_test = cvMat( 4748, 1, CV_32SC1, responsesData_int ); 


    CvRTParams params= CvRTParams(10, 2, 0, false,16, 0, true, 0, 100, 0, CV_TERMCRIT_ITER );
    CvRTrees rtrees;
    rtrees.train(&trainingDataCvMat_test, CV_ROW_SAMPLE, &responsesCvMat_test, NULL, NULL, NULL, NULL,params);

	vector<float> NewHighFreAvgHeight;
	
	for(int i = 0; i < NewHighFreDistance.size(); ++i)   //for(int i = 0; i < NewHighFreDistance.size(); ++i)
	{
		double sampleData = NewHighFreDistance[i];//
		Mat sampleMat(1, 1, CV_32FC1, sampleData);     
		float rr = rtrees.predict(sampleMat);
		NewHighFreAvgHeight.push_back(rr);
	}

	for(int i = 0; i < NewHighFreAvgHeight.size(); ++i)
	{
		NewHighFreAvgHeight[i] = NewHighFreAvgHeight[i]/100;
	}
	//NewHighFreDistance.clear();

	/*
	ofstream outFile12("11111111111.txt",ios_base::out);  //按新建或覆盖方式写入  
	// 写入数据  
	for (int r = 0; r < 4748; r++)         //row
	{
		float tmp = NewHighFreAvgHeight[r];    //读取数据，at<type> - type 是矩阵元素的具体数据格式 
		outFile12 << tmp << endl;
	}		
	outFile12.close();
	*/
	
	//加一点随机扰动


	
	//找到跟预测出的值最接近的mountain文件夹下的patch进行新场景的填充
	vector<int> SeclectedPatch;
	for(int i = 0; i < NewHighFreAvgHeight.size(); ++i)
	{
		float tmpp = abs(NewHighFreAvgHeight[i] - responsesData[0]);
		int flag = 0;
		for(int j = 0; j < 4748; ++j)
		{
			float tmppp = abs(NewHighFreAvgHeight[i] - responsesData[j]);
			
			if(tmpp > tmppp)
			{
				tmpp = tmppp;
				flag = j;
			}
		}
		SeclectedPatch.push_back(flag);
	}

	/*
	ofstream outFile15("222222222.txt",ios_base::out);//按新建或覆盖方式写入  
	// 写入数据  
	for (int r = 0; r < NewHighFreAvgHeight.size(); r++)         //row
	{
		outFile15 << NewHighFreAvgHeight[r] << "\t" << NewHighFreDistance[r] <<endl;
	}		
	outFile15.close();
	*/



	//将选择出的每一块patch的高度赋到新场景中
	GDALAllRegister();//注册类型,读取写入任何类型影像必须加入此句
	GDALDriver *poDriver;
	const char *pszFormat = "GTiff";
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);


	int BandNum = 1;
	GDALDataType dataType = GDAL_example.DATAType;
	int NewWidth = 200;
	int NewHeight = 200;

	if(poDriver == NULL) 
	{ 
		return;
	}

	GDALDataset *OutDs;
	char **papszOptions = NULL;
	char **papszMetadata; //这里的参数全是指tif格式的参数,如果是其他格式,请把这里所有注释掉,或者参照文档,自行设定 //设置压缩类型,envi只认得packbits压缩.
	papszOptions = CSLSetNameValue(papszOptions, "INTERLEAVE", "BAND");
	OutDs = poDriver->Create("mountain/sythesis20171025.tif",
                         NewWidth,
                         NewHeight,
                         BandNum,
                         dataType,  
                         papszOptions);

	double Geo[6];
	Geo[0] = GDAL_example.geos[0];
	Geo[1] = GDAL_example.geos[1];
	Geo[2] = GDAL_example.geos[2];
	Geo[3] = GDAL_example.geos[3];
	Geo[5] = GDAL_example.geos[5];
	Geo[4] = GDAL_example.geos[4];

	char *pszProjection = GDAL_example.projection_info;
	char *pszPrettyWkt = NULL; //设置为第一幅图的投影,如果与原图不同,请跳过这个if部分,到下个部分直接设置投影 //设置投影,如果需要特殊投影,请找到wkt串,自行建立投影后传入 


	//关键是这个data的数据
	short *data = new short[NewWidth*NewHeight];
	//初始化新场景的每个高程点为0
	for(int i = 0;i<NewWidth;++i)
	{
		for(int j = 0;j<NewHeight;++j)
		{
			data[i*NewWidth+j] = 0;
		}
	}
	//NewPatch = (patch *)malloc(patch_size*patch_size*sizeof(struct patch));

	int *ishandled = new int[patch_size*patch_size];
	for(int i = 0; i < patch_size; ++i)
		for(int j = 0; j < patch_size; ++j)
			ishandled[patch_size*i + j] = 0;

	
	
	for(int i = 0; i < Need2Placed.size(); ++i)
	{
		string patch_name = "mountain/";
		
		patch_name += Int_to_String(SeclectedPatch[i]);
		patch_name += ".tif";
		char* patch_name_new = (char *)patch_name.data();
		int IsSucceeded = ChooseSelectedPatchIntoNewScene(patch_name_new,5,5,data,Need2Placed[i],i);
		int a = Need2Placed[i].X;
		int b = Need2Placed[i].Y;
		ishandled[patch_size*a+b] = 1;
	}

	//写入数据到outds中
	GDALRasterBand *outBand = OutDs->GetRasterBand(1);
	outBand->RasterIO(GF_Write,
		0,
		0,
		NewWidth,
		NewHeight,
		data,
		NewWidth,
		NewHeight,
		dataType,
		0,
		0);
	GDALClose(OutDs);
 }


 void CFluidsimView::On32840()
 {
	 CString path;
	 CFileDialog dlg(TRUE, _T("*.png"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp; *.jpg; *.png) |*.bmp;*.jpg;*.png|All Files (*.*)|*.*||",NULL);
	 char title[]= {"Open Source Image"};
	 dlg.m_ofn.lpstrTitle= title;

	 Mat sourceimg;

	 if (dlg.DoModal() == IDOK) {

		 path = dlg.GetPathName();
		 char *sss;
		 //strcpy(sss,path);
		 sss = path.GetBuffer();
		 sourceimg = cvLoadImage(sss, CV_LOAD_IMAGE_GRAYSCALE);
		 //namedWindow("source image",CV_WINDOW_NORMAL);
		 //imshow("source image",sourceimg); 
	 }


	 //直接对高度进行分层
	 int Row = sourceimg.rows;
	 int Column = sourceimg.cols;
	 int nr;
	 int nc;

	 if(sourceimg.isContinuous())
	 {
		nr=1;
		nc=Column*Row*sourceimg.channels();
	 } 
	 imshow("scource image",sourceimg); 

	 /*
	 Mat fenceng = sourceimg;
	 //遍历像素点灰度值  
	 for (int i=0;i<nr;i++)  
	 {  
		 uchar *p=sourceimg.ptr<uchar>(i);    //获取行地址  
		 for (int j=0;j<nc;j++)  
		 {  
			 if(p[j] > 236)
				p[j] = 255;           //修改灰度值
			 else if(p[j] > 204)
				p[i] = 236;
			 else if(p[j] > 172)
				 p[j] = 204;
			 else if(p[j] > 140)
				 p[j] = 172;
			 else if(p[j] > 108)
				 p[j] = 140;
			 else if(p[j] > 72)
				 p[j] = 108;
			 else if(p[j] > 36)
				 p[j] = 72;
			 else
				 p[j] = 36;
		 }  
	 }
	 
	 imshow("fenceng image",fenceng); 
	 */

	 
	 Mat fancha = Mat(Row,Column,sourceimg.depth());
	 for (int i = 0; i < nr; i++)
	 {
		 uchar *p = sourceimg.ptr<uchar>(i);
		 uchar *pp = fancha.ptr<uchar>(i);
		 for (int j = 0; j < nc; ++j)
		 {
			 pp[j] = 255 - p[j];
		 }
	 }
	 imshow("cha image",fancha); 
	 

	 //Mat new1 = Enropty(sourceimg,sourceimg.rows,sourceimg.cols);
	 Mat new2 = Enropty(fancha,fancha.rows,fancha.cols);

	 //static int nDx[]= {-1,0,1,0}; //   {1, 1, 0,-1,-1,-1, 0, 1}
	 //static int nDy[]= {0,1,0,-1}; //   {0, 1, 1, 1,0 ,-1,-1,-1}

	 //imshow("New1 image",new1); 
	 imshow("After Enropty New2 image",new2); 
	 imwrite("EnroptyCut.bmp",new2);

	 //找外部轮廓
	 Mat image;  
	 GaussianBlur(new2,image,Size(3,3),0);  
	 Canny(image,image,100,250);  
	 vector<vector<Point>> contours;  
	 vector<Vec4i> hierarchy;  
	 findContours(image,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE,Point());  
	 Mat imageContours=Mat::zeros(image.size(),CV_8UC1);  
	 Mat Contours=Mat::zeros(image.size(),CV_8UC1);  //绘制  
	 for(int i=0;i<contours.size();i++)  
	 {  
		 //contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数  
		 for(int j=0;j<contours[i].size();j++)   
		 {  
			 //绘制出contours向量内所有的像素点  
			 Point P=Point(contours[i][j].x,contours[i][j].y);  
			 Contours.at<uchar>(P)=255;  
		 }  

		 //输出hierarchy向量内容  
		 char ch[256];  
		 sprintf(ch,"%d",i);  
		 string str=ch;  
		 cout<<"向量hierarchy的第" <<str<<" 个元素内容为："<<endl<<hierarchy[i]<<endl<<endl;  

		 //绘制轮廓  
		 drawContours(imageContours,contours,i,Scalar(255),1,8,hierarchy);  
	 } 
	 namedWindow("Contours Image",1);
	 imshow("Contours Image",imageContours); //轮廓
	 

	 


	 int tmp = 0;
 }

 Mat CFluidsimView::Enropty(Mat s, int Row, int Column)
 {
	 Mat N = Mat(Row,Column,s.depth());
	 int nr;
	 int nc;
	 s.copyTo(N);
	 if(s.isContinuous())
	 {
		 nr=1;
		 nc=Column*Row*s.channels();
	 } 
	 
	 double *Arr = new double[Row*Column];
	 for(int i=1;i<Row-1;++i)
	 {
		 for(int j=1;j<Column-1;++j)
		 {
			 int total = 0;
			 total = N.at<uchar>(i-1,j-1) + N.at<uchar>(i-1,j) + N.at<uchar>(i-1,j+1) + N.at<uchar>(i,j-1) + N.at<uchar>(i,j) + N.at<uchar>(i,j+1) + N.at<uchar>(i+1,j-1) + N.at<uchar>(i+1,j) + N.at<uchar>(i+1,j+1);
			 double p1 = (double)N.at<uchar>(i-1,j-1)/total;
			 double p2 = (double)N.at<uchar>(i-1,j)/total;
			 double p3 = (double)N.at<uchar>(i-1,j+1)/total;
			 double p4 = (double)N.at<uchar>(i,j-1)/total;
			 double p5 = (double)N.at<uchar>(i,j)/total;
			 double p6 = (double)N.at<uchar>(i,j+1)/total;
			 double p7 = (double)N.at<uchar>(i+1,j-1)/total;
			 double p8 = (double)N.at<uchar>(i+1,j)/total;
			 double p9 = (double)N.at<uchar>(i+1,j+1)/total;

			 double result = -p1*log(p1) / log(2.0);
			 result += -p2*log(p2) / log(2.0);
			 result += -p3*log(p3) / log(2.0);
			 result += -p4*log(p4) / log(2.0);
			 result += -p5*log(p5) / log(2.0);
			 result += -p6*log(p6) / log(2.0);
			 result += -p7*log(p7) / log(2.0);
			 result += -p8*log(p8) / log(2.0);
			 result += -p9*log(p9) / log(2.0);
			 Arr[i*Column+j] = result;
		 }
	 }

	 double maxV = 0;

	 for(int i=0;i<Row;++i)
	 {
		 for(int j=0;j<Column;++j)
		 {
			 if(maxV < Arr[i*Column+j])
				 maxV = Arr[i*Column+j];
		 }
	 }

	 double minV = maxV;
	 for(int i=1;i<Row-1;++i)
	 {
		 for(int j=1;j<Column-1;++j)
		 {
			 if(minV > Arr[i*Column+j])
				 minV = Arr[i*Column+j];
		 }
	 }

	 Mat M(Row-1,Column-1,N.depth());
	 for(int i=0;i<Row-1;++i)
	 {
		 for(int j=0;j<Column-1;++j)
		 {
			 //int yyy = int (255*( (Arr[(i+1)*Column + (j+1)]-minV) / (maxV - minV)));
			 M.at<uchar>(i,j) = int (255*( (Arr[(i+1)*Column + (j+1)]-minV) / (maxV - minV)));
			 //int xxx = 0;

			 /*
			 if(M.at<uchar>(i,j)<=252)
				 M.at<uchar>(i,j) = 0;
			 else
				 M.at<uchar>(i,j) = 255;
			 */
				 
		 }
	 }
	 delete []Arr;
	 return M;

 }

 void CFluidsimView::OnHeCheng1()//合成高程图
 {
	 Mat sourceimg = imread("out20171031.bmp", 1);

	 Mat skeletonimg = imread("out201711301111.bmp",1);
	 for(int i = 0; i < skeletonimg.rows; ++i)
	 {
		 for (int j = 0; j < skeletonimg.cols;++j)
		 {
			 if(skeletonimg.at<uchar>(i,j) == 255)
			 {
				 point tmp;
				 tmp.X = i;tmp.Y = j;
				 pos_skeleton.push_back(tmp);
			 }
		 }
	 }

	 int maxSourceV = 0;

	 for(int i=0;i<skeletonimg.rows;++i)
	 {
		 for(int j=0;j<skeletonimg.cols;++j)
		 {	
			 if(maxSourceV < skeletonimg.at<uchar>(i,j))
			 {
				 short tmp = skeletonimg.at<uchar>(i,j);
				 maxSourceV = skeletonimg.at<uchar>(i,j);
			 }
		 }
	 }
	 int minSourceV = maxSourceV;
	 for(int i=0;i<skeletonimg.rows;++i)
	 {
		 for(int j=0;j<skeletonimg.cols;++j)
		 {	
			 if(minSourceV > skeletonimg.at<uchar>(i,j))
			 {
				 minSourceV = skeletonimg.at<uchar>(i,j);
			 }
		 }
	 }

	 //short *data00 = new short[NewWidth*NewHeight];


	 int maxH = 3500;
	 int minH = 1200;


	 //将选择出的每一块patch的高度赋到新场景中
	 GDALAllRegister();//注册类型,读取写入任何类型影像必须加入此句
	 GDALDriver *poDriver;
	 const char *pszFormat = "GTiff";
	 poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);


	 int BandNum = 1;
	 GDALDataType dataType = GDAL_example.DATAType;
	 int NewWidth = skeletonimg.cols;    //sourceimg.cols;
	 int NewHeight = skeletonimg.rows;   //sourceimg.rows;

	 Mat sk_flip;
	 flip(skeletonimg,sk_flip,1);




	 if(poDriver == NULL) 
	 { 
		 return;
	 }

	 GDALDataset *OutDs;
	 char **papszOptions = NULL;
	 char **papszMetadata; //这里的参数全是指tif格式的参数,如果是其他格式,请把这里所有注释掉,或者参照文档,自行设定 //设置压缩类型,envi只认得packbits压缩.
	 papszOptions = CSLSetNameValue(papszOptions, "INTERLEAVE", "BAND");
	 OutDs = poDriver->Create("out20171130.tif",
		 NewWidth,
		 NewHeight,
		 BandNum,
		 dataType,  
		 papszOptions);

	 double Geo[6];
	 Geo[0] = GDAL_example.geos[0];
	 Geo[1] = GDAL_example.geos[1];
	 Geo[2] = GDAL_example.geos[2];
	 Geo[3] = GDAL_example.geos[3];
	 Geo[4] = GDAL_example.geos[4];
	 Geo[5] = GDAL_example.geos[5];

	 char *pszProjection = GDAL_example.projection_info;
	 char *pszPrettyWkt = NULL; //设置为第一幅图的投影,如果与原图不同,请跳过这个if部分,到下个部分直接设置投影 //设置投影,如果需要特殊投影,请找到wkt串,自行建立投影后传入 
	 //关键是这个data的数据
	short *data = new short[NewWidth*NewHeight];
	
	for(int i = 0;i<NewHeight;++i)
	{
		for(int j = 0;j<NewWidth;++j)
		{
			short tmp = skeletonimg.at<uchar>(i,j);
			data[i*NewWidth+j] =  (maxH - minH)*((double)tmp/(255)) + minH;         //(maxH - minH)*(double(sourceimg.at<uchar>(i,j) - minSourceV) / (maxSourceV - minSourceV)) + minH;

		}
	}
	//写入数据到outds中
	GDALRasterBand *outBand = OutDs->GetRasterBand(1);
	outBand->RasterIO(GF_Write,
                      0,
                      0,
					  NewWidth,
                      NewHeight,
                      data,
                      NewWidth,
                      NewHeight,
                      dataType,
                      0,
                      0);
	GDALClose(OutDs);
 }


 Mat CFluidsimView::Enropty_Height()
 {
	 double *Arr = new double[nWidth*nHeight];
	 double *Hcha = new double[nWidth*nHeight];
	 for(int i = 0; i < nWidth; ++i)
		 for (int j = 0; j < nHeight; ++j)
		 {
			 Hcha[i*nWidth+j] = maxheight - P3D1[(i)*nWidth + (j)].H + minheight;
		 }

	 for(int i=1;i<nWidth-1;++i)
	 {
		 for(int j=1;j<nHeight-1;++j)
		 {
			 double total = 0;   //P3D1[i*nWidth+j].H
			
			 //高度熵
			 total = Hcha[(i-1)*nWidth + (j-1)] + Hcha[(i-1)*nWidth + (j)] + Hcha[(i-1)*nWidth + (j+1)] + 
				 Hcha[(i)*nWidth + (j-1)]   + Hcha[(i)*nWidth + (j)]   + Hcha[(i)*nWidth + (j+1)]   +
				 Hcha[(i+1)*nWidth + (j-1)] + Hcha[(i+1)*nWidth + (j)] + Hcha[(i+1)*nWidth + (j+1)];
			 double p1 = (double)Hcha[(i-1)*nWidth + (j-1)]/total;
			 double p2 = (double)Hcha[(i-1)*nWidth + (j)]/total;
			 double p3 = (double)Hcha[(i-1)*nWidth + (j+1)]/total;
			 double p4 = (double)Hcha[(i)*nWidth + (j-1)]/total;
			 double p5 = (double)Hcha[(i)*nWidth + (j)]/total;
			 double p6 = (double)Hcha[(i)*nWidth + (j+1)]/total;
			 double p7 = (double)Hcha[(i+1)*nWidth + (j-1)]/total;
			 double p8 = (double)Hcha[(i+1)*nWidth + (j)]/total;
			 double p9 = (double)Hcha[(i+1)*nWidth + (j+1)]/total;




			 double result = -p1*log(p1) / log(2.0);
			 result += -p2*log(p2) / log(2.0);
			 result += -p3*log(p3) / log(2.0);
			 result += -p4*log(p4) / log(2.0);
			 result += -p5*log(p5) / log(2.0);
			 result += -p6*log(p6) / log(2.0);
			 result += -p7*log(p7) / log(2.0);
			 result += -p8*log(p8) / log(2.0);
			 result += -p9*log(p9) / log(2.0);
			 Arr[i*nWidth+j] = result;
		 }
	 }

	 double maxV = 0;

	 for(int i=0;i<nWidth;++i)
	 {
		 for(int j=0;j<nHeight;++j)
		 {
			 if(maxV < Arr[i*nWidth+j])
				 maxV = Arr[i*nHeight+j];
		 }
	 }

	 double minV = maxV;
	 for(int i=1;i<nWidth-1;++i)
	 {
		 for(int j=1;j<nHeight-1;++j)
		 {
			 if(minV > Arr[i*nWidth+j])
				 minV = Arr[i*nHeight+j];
		 }
	 }

	 Mat M(nWidth-1,nHeight-1,CV_8UC1);
	 for(int i=0;i<nWidth-1;++i)
	 {
		 for(int j=0;j<nHeight-1;++j)
		 {
			 //int yyy = int (255*( (Arr[(i+1)*Column + (j+1)]-minV) / (maxV - minV)));
			 M.at<uchar>(i,j) = int (255*( (Arr[(i+1)*nWidth + (j+1)]-minV) / (maxV - minV)));
			 M.at<uchar>(i,j) = 255 - M.at<uchar>(i,j);
			 //int xxx = 0;

			 /*
			 if(M.at<uchar>(i,j)<=252)
				 M.at<uchar>(i,j) = 0;
			 else
				 M.at<uchar>(i,j) = 255;
			 */
				 
		 }
	 }
	 delete []Arr;
	 return M;
 }

 void CFluidsimView::On32842()
 {
	 double MaxV,MinV;
	 MaxV = maxheight;
	 MinV = minheight;
	 Mat x = Enropty_Height();
	 namedWindow("After Enropty tif image", CV_WINDOW_NORMAL);
	 imwrite("Enropty_height_result.png",x);
	 imshow("After Enropty tif image",x);


	 //直接对高度进行分层 
	 Mat fenceng = Mat(x.rows+1,x.cols+1,x.depth());
	 float xx = (MaxV - MinV)/7;
	 //遍历像素点灰度值
	 for (int i=0;i<nWidth;++i)  
	 {  
		 uchar *p=fenceng.ptr<uchar>(i);    //获取行地址  
		 for (int j=0;j<nHeight;++j)  
		 {  
			 if(P3D1[i*nWidth+j].H > (MinV+xx*6))
				p[j] = 255;                 
			 else if(P3D1[i*nWidth+j].H > (MinV+xx*5))
				p[i] = 216;
			 else if(P3D1[i*nWidth+j].H > (MinV+xx*4))
				 p[j] = 180;
			 else if(P3D1[i*nWidth+j].H > (MinV+xx*3))
				 p[j] = 144;
			 else if(P3D1[i*nWidth+j].H > (MinV+xx*2))
				 p[j] = 108;
			 else if(P3D1[i*nWidth+j].H > (MinV+xx*1))
				 p[j] = 72;
			// else if(P3D1[i*nWidth+j].H > (MinV+xx))
				//p[j] = 72;
			 else
				 p[j] = 36;
		 }  
	 }
	 namedWindow("fenceng image",WINDOW_NORMAL);
	 imwrite("fenceng.png",fenceng);
	 imshow("fenceng image",fenceng); 

	 
	 
 }


 










































 float CFluidsimView::GDALHillshadeAlg(float* afWin, float fDstNoDataValue, void* pData)
 {
	 GDALHillshadeAlgData*psData = (GDALHillshadeAlgData*)pData;
	 double x, y,aspect, xx_plus_yy, cang;

	 // 计算坡度
	 x =((afWin[0] + afWin[3] + afWin[3] + afWin[6]) -
		 (afWin[2]+ afWin[5] + afWin[5] + afWin[8])) / psData->ewres;

	 y =((afWin[6] + afWin[7] + afWin[7] + afWin[8]) -
		 (afWin[0]+ afWin[1] + afWin[1] + afWin[2])) / psData->nsres;

	 xx_plus_yy =x * x + y * y;

	 // 计算坡向
	 aspect =atan2(y, x);

	 // 计算山体阴影值
	 cang =(psData->sin_altRadians -
		 psData->cos_altRadians_mul_z_scale_factor* sqrt(xx_plus_yy) *
		 sin(aspect- psData->azRadians)) /
		 sqrt(1+ psData->square_z_scale_factor * xx_plus_yy);

	 if (cang<= 0.0)
		 cang= 1.0;
	 else
		 cang= 1.0 + (254.0 * cang);

	 return(float)cang;
 }

 void*  CFluidsimView::GDALCreateHillshadeData(double* adfGeoTransform, double z,double scale,double alt,double az)
 {
	 GDALHillshadeAlgData*pData =(GDALHillshadeAlgData*)CPLMalloc(sizeof(GDALHillshadeAlgData));

	 const double degreesToRadians = M_PI / 180.0;
	 pData->nsres= adfGeoTransform[5];
	 pData->ewres= adfGeoTransform[1];
	 pData->sin_altRadians= sin(alt * degreesToRadians);
	 pData->azRadians= az * degreesToRadians;
	 double z_scale_factor = z / (8 * scale);
	 pData->cos_altRadians_mul_z_scale_factor = cos(alt* degreesToRadians) * z_scale_factor;
	 pData->square_z_scale_factor= z_scale_factor * z_scale_factor;
	 return pData;
 }


 void CFluidsimView::On32843()
 {
	 //Mat sketch = imread("skeleton.png",CV_8UC1);
	 IplImage* simg = cvLoadImage("20171017.png", 1);
	 //MiniCross(simg);





 }
