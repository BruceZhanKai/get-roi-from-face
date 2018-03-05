



#pragma region multi tracking

//===================================================================
//===================================================================
/*OpenCV2.4.11*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
//===================================================================
#include "libTracker.h"
#include "basics.h"
#include <tbb/parallel_for.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
//===================================================================
//===================================================================
//===================================================================
//================ dlib =============================================
//===================================================================
#include <stdio.h>
#include <vector>
#include <iomanip>                                                       //與使用此函數QueryPerformanceCounter有關,若移動至Linux須拿掉
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <stdlib.h>
//===================================================================
#include <math.h>
//=========================s==========================================
//===================================================================
//===================================================================
using namespace std;
using namespace cv;
//===================================================================
int text_point_x;
int text_point_y;
//===================================================================
bool front = true;
bool front1 = true;
int lbp_size = 256;
int rotation_size = 192;
bool lock_scale;
bool length_unknown;
int w_all = 1000;
int h_all = 1000;
int captured_image_size = 1200;
//===================================================================
#define SMALL 0.0000001

/*kinds of color type */
//cv::Scalar show_color_pink = Scalar(136, 0, 255);  //pink

cv::Scalar show_color_White = Scalar(255, 255, 255);
cv::Scalar show_color_green = CV_RGB(0, 255, 100);
cv::Scalar show_color_pink = Scalar(120, 0, 255); //淡藍色  255,255,0
cv::Scalar show_color_blue = Scalar(255, 255, 0); //淡藍色  255,255,0
cv::Scalar show_color_grey = Scalar(50, 50, 50); //暗灰色  255,255,0
cv::Scalar show_color_GWhite = Scalar(220, 220, 220); //暗灰色  255,255,0
const double DESIRED_LEFT_EYE_X = 0.20;//0.16     // Controls how much of the face is visible after preprocessing.//調整欲擷取訓練之臉的大小
const double DESIRED_LEFT_EYE_Y = 0.4;//0.14



//==============================================================================================================================


/*add area from the original image to the real input image*/
void add_image_area(Mat& image, Mat& final_img)
{
	if (image.cols <= 1000 && image.rows <= 1000)
	{
		Mat out1;
		Rect image_rect = Rect((captured_image_size - image.cols) / 2, (captured_image_size - image.rows) / 2, image.cols, image.rows);
		out1 = final_img(image_rect);
		image.copyTo(out1);
	}
	else if (image.cols >= 3000 || image.rows >= 3000)
	{
		float be_small[3] = { 1500, 1, 1 };
		be_small[1] = be_small[0] / float(image.cols);
		be_small[2] = float(image.rows)*be_small[1];
		resize(image, final_img, Size((int)be_small[0], (int)be_small[2]));
	}
	else
	{
		w_all = image.cols;
		h_all = image.rows;
		final_img = image;
	}
	//imshow("final_img",final_img);
	//waitKey(1000);
}


void be_small_img(Mat& image, Mat& final_img){
	if ((image.cols > image.rows) && image.cols >= 2000){
		float be_small[4] = { 2000, 0, 0, 0 };
		be_small[1] = be_small[0] / float(image.cols);
		be_small[2] = float(image.cols)*be_small[1];
		be_small[3] = float(image.rows)*be_small[1];
		resize(image, final_img, Size((int)be_small[2], (int)be_small[3]));
	}
	else if ((image.rows > image.cols) && image.rows >= 2000)
	{
		float be_small[4] = { 2000, 0, 0 };
		be_small[1] = be_small[0] / float(image.rows);
		be_small[2] = float(image.cols)*be_small[1];
		be_small[3] = float(image.rows)*be_small[1];
		resize(image, final_img, Size((int)be_small[2], (int)be_small[3]));
	}
	else
	{
		final_img = image;
	}
}



int main(int argc, char **argv)
{
	cout << "Welcome to use Startup Robot Recognition Engine." << endl;

    FacialTracker facialtracker;


    if(!facialtracker.Initial(argc, argv))
    {
        std::cout << "[main] facialtracker Initial fail\n";
		return false;
    }

	//bool load_success = false;
	//string load_success;
	bool load_success = false;

	char separator1 = ';';
	int data_base_number = 0;

	double eye_dis;
	bool save_image;
	bool save_feature;

	bool D_path;
	string position_path="./";


	string input_path, output_path;
	cout << "input folder name?" << endl;
	cout << "./[inputfolder]/..." << endl;
	cout << ">";
	cin >> input_path;


    output_path = position_path + "Result-" + input_path;

    position_path += input_path;

    char ResultFolder1_char[500];

    strcpy(ResultFolder1_char, output_path.c_str());

    mkdir(ResultFolder1_char, 0777);

    //cout<<"position_path="<<position_path<<endl;
    //cout<<"output_path="<<output_path<<endl;

    //  ./ResultFolder1

    std::vector<string> ClassName = BZ::GetAllNameInFolder(position_path);

    BZ::DoOfstream("",output_path+"/Miss.txt",BZ::SaveMode::OUT,BZ::TextMode::EMPTY);

    for(int i=0; i<ClassName.size(); i++)
    {
        std::string ClassPath = position_path + "/" + ClassName[i];
        //cout<<"ClassPath="<<ClassPath<<endl;
        std::vector<string> ImageName = BZ::GetAllNameInFolder(ClassPath);
        for(int j=0; j<ImageName.size(); j++)
        {
            std::string ImagePath = ClassPath + "/" + ImageName[j];
            //cout<<"ImagePath="<<ImagePath<<endl;
            cv::Mat Image = cv::imread(ImagePath, 1);
            for(int k=0; k<10; k++)
            {
                Information DetectFinalImage = facialtracker.Update(Image);
                if(!DetectFinalImage.FeartureImage.empty())
                {
                    //get roi image
                    //save image in output folder
                    char ResultFolder2_char[500];
                    strcpy(ResultFolder2_char, ClassPath.c_str());
                    mkdir(ResultFolder2_char, 0777);
                    //  ./ResultFolder1/ResultFolder2/  (ResultFolder2=ClassName)
                    std::string OutPath = output_path + "/" + ClassName[i] + "/" + ImageName[j];
                    cv::imwrite(OutPath, DetectFinalImage.FeartureImage);
                    break;
                }
                else if(DetectFinalImage.FeartureImage.empty() && k == 9)
                {
                    //no image exist
                    //save input image path in output folder
                    BZ::DoOfstream(ImagePath,output_path+"/Miss.txt",BZ::SaveMode::APP,BZ::TextMode::TEXTN);
                }
            }
        }
    }

	cout << "complete" << endl;
	system("pause");
	return 0;
}


#pragma endregion 2013


///////////////////////////////////////////////////////////////////////////////


