
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//ProfileMonitor_viewer
//                  created by Kazuo Tanaka
//-------------------------------------------------------------------------------------------------------------------------------------------------------


// Include files to use the PYLON API.
#include <iostream>
#include <fstream>
#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#include <pylon/PylonGUI.h>
#endif
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <time.h>

// Namespace a
using namespace cv;
using namespace Pylon;
using namespace std;
using namespace GenApi;

// definition of const number
string file_path = "/mnt/sakelab/profile-monitor/offline/profile-image/";
static const uint32_t c_countOfImagesToGrab = 100;
const double xrange = 83.3; //画像のx両端間の距離（mm）
const double yrange = 62.5;//画像のy両端間の距離（mm）
const double xwindow = 42; //x方向の切り取る範囲
const double ywindow = 42; //y方向の切り取る範囲
const double xcenter = 1.26; //画像のx中心位置(mm)
const double ycenter = 5.30; //画像のy中心位置(mm)
const int xPixels = 2592; //resolution of a camera(x)
const int yPixels = 1944;//resolution of a camera(y)
const double xwindow_target = 28.4; //width of the ROI(x)
const double ywindow_target = 28.4;//height of the ROI(y)
const int projection_hight = 150; //height of the projection windows
const int ywindows_margin = 280;
const double resize_factor = 0.5; //resize factor of the image
const double threshold_value = 25.5;
const int ROIlist_rate = 0.5; //time interval to take ROI
const int ROIlist_length = 1000;  //max rows of ROI list
const int max_gain = 23; // max gain of camera
const int max_exposure = 100;// max exposure time (10^x)
const double Rb_ROI_to_numbers = 2.94e3;
const int Fr_ROI_to_numbers = 0.006;
const double factor_vol = 2.8;
const string mpod_ip = "192.168.253.73";
//definition of position on image
int xlowPixels = (int)(xPixels / 2 * (1 - xwindow / xrange + 2 * xcenter / xrange));
int xupPixels = (int)(xPixels / 2 * (1 + xwindow / xrange + 2 * xcenter / xrange));
int ylowPixels = (int)(yPixels / 2 * (1 - ywindow / yrange + 2 * ycenter / yrange));
int yupPixels = (int)(yPixels / 2 * (1 + ywindow / yrange + 2 * ycenter / yrange));
int xwidthPixels = (int)(xPixels*xwindow / xrange);
int ywidthPixels = (int)(yPixels*ywindow / yrange);
int xlowPixels_target = (int)(xPixels / 2 * (1 - xwindow_target / xrange + 2 * xcenter / xrange));
int xupPixels_target = (int)(xPixels / 2 * (1 + xwindow_target / xrange + 2 * xcenter / xrange));
int ylowPixels_target = (int)(yPixels / 2 * (1 - ywindow_target / yrange + 2 * ycenter / yrange));
int yupPixels_target = (int)(yPixels / 2 * (1 + ywindow_target / yrange + 2 * ycenter / yrange));

int xlowPixels_target_cm = (xlowPixels_target - xlowPixels) * resize_factor;
int xupPixels_target_cm = (xupPixels_target - xlowPixels) * resize_factor;
int ylowPixels_target_cm = (ylowPixels_target - ylowPixels) * resize_factor;
int yupPixels_target_cm = (yupPixels_target - ylowPixels) * resize_factor;

int xwidthPixels_target = (int)(xPixels*xwindow_target / xrange);
int ywidthPixels_target = (int)(yPixels*ywindow_target / yrange);
int key;      //waitkey

int slider_flag = 0;
int gain_slider = 0;
int exposure_slider = 1;
int RbFr_slider = 1;
double max_vertical;
double max_horizontal;
char filename[128];
char foldername[128];
time_t time_start, time_end1, time_end2;
float targetROI_sum;
vector< vector<float> > ROIlist( 2, vector<float>(0) );

char  vol_306[1024];
float ROI_to_numbers ;
string ion="Fr";

//declartion of local time
time_t t;

//to solve a issue about to_string() func
template < typename T > std::string to_string( const T& n )
{
  std::ostringstream stm ;
  stm << n ;
  return stm.str() ;
}

//struct for mouse parameter
struct MouseParams
{
  Mat mouseparams_img;
  Mat mouseparams_cmimg;
  Point mouseparams_pt;
  vector< vector<float> > ROIlist;
};


//-------------------------------------------------------------------------------------------------------------------------------------------------------
//MouseCall----------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void MouseCall( int event, int x, int y, int flags, void* param) {
  MouseParams* mp = (MouseParams*)param;
  cv::Mat img =  mp->mouseparams_img;
  cv::Mat cm_img =  mp->mouseparams_cmimg;
//file save on Lbuttondown
  if (event == cv::EVENT_LBUTTONDOWN) {
    vector< vector<float> > ROIlist =  mp->ROIlist;
    char  datafile[10240];
    char  voltage_list[1024];
    cout << "now saving...\n";





    //create folder
    t = time(NULL);
    strftime(foldername, sizeof(foldername), "%y%m%d", localtime(&t));
    string folderCreateCommand = "mkdir " + file_path + to_string(foldername);
    system(folderCreateCommand.c_str());

    //save image file
    strftime(filename, sizeof(filename), "PM%y%m%d%H%M%S", localtime(&t));
    imwrite(file_path + to_string(foldername) + "/" + to_string(filename) + ".bmp", img);
    imwrite(file_path + to_string(foldername) + "/" + to_string(filename) + ".png", cm_img);
    cout << "Image file is saved as " << file_path  + to_string(foldername) + "/" + to_string(filename) + ".bmp" << "\n";


    //save data file
    FILE* fp;

    if ((fp = popen(("snmpwalk -v 2c -M /home/cyric/mibs -m +WIENER-CRATE-MIB -c public "+mpod_ip+to_string(" outputVoltage| egrep 'u[0-9]+.*[0-9]*\\.[0-9]' -o | tr \"\\n\" \", \" | sed -e \"s/Opaque: Float: //g\"")).c_str(), "r")) != NULL) {
      while (fgets(voltage_list, sizeof(voltage_list), fp) != NULL) {//read voltage list
      }
    }

    pclose(fp);
    strcat( datafile, "ion mode :\t " );    strcat( datafile, ion.c_str() );
    strcat( datafile, "\n MCP front :\t " );    strcat( datafile, vol_306 );
    strcat( datafile, "gain :\t " );    strcat( datafile, to_string(gain_slider).c_str() );
    strcat( datafile, "\n exposure time :\t " );   strcat( datafile, to_string(exposure_slider*100000).c_str() );
    strcat( datafile, " us \nvoltage list: \t " );
    strcat( datafile, voltage_list );
    ofstream datafilename((file_path + to_string(foldername) + "/" + to_string(filename) + ".txt").c_str());
    datafilename << datafile;
    datafilename.close();

    ofstream ROIlistfilename("ROI.txt");
    for (int i = 0; i < ROIlist[0].size(); i++) {
      ROIlistfilename << ROIlist[0][i];
      ROIlistfilename << "\t";
      ROIlistfilename << ROIlist[1][i];
      ROIlistfilename << "\n";
    }

    ROIlistfilename.close();


    //submit elog
    string elog_command = "cat " + file_path + to_string(foldername) + "/" + to_string(filename) + to_string(".txt | elog -h localhost -p 8080 -l offline -a Author=\"tanaka\" -a ion=\"")+ion+to_string("\" -f ") + file_path  + to_string(foldername) + "/" + to_string(filename) + ".png -f " + "ROI.txt" ;
    system(elog_command.c_str());

    cout << "data file is saved.";
    cout << datafile;
    cout < "\n";
  }


//kill app on Rbuttondown
  else if (event == cv::EVENT_RBUTTONDOWN) {
    time(&time_start);
    time(&time_end1);
    time(&time_end2);
    system ("rm ROI.txt");
    ofstream ROIlistfilename("ROI.txt");
    ROIlistfilename << 0;
    ROIlistfilename << "\t";
    ROIlistfilename << 0;
    ROIlistfilename << "\n";

  }

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
//trackbar function---------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------

void
on_tracker (int val)
{

  slider_flag = 1;

}


//-------------------------------------------------------------------------------------------------------------------------------------------------------
//Main---------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  // The exit code
  int exitCode = 0;

  try
  {
    ROIlist[0].push_back(1);
    ROIlist[1].push_back(1);

    time(&time_start);
    time(&time_end1);
    time(&time_end2);
    // Preparation for Pylon5

    Pylon::PylonAutoInitTerm autoInitTerm;
    CGrabResultPtr ptrGrabResult;
    CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());
    cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;
    camera.Open();
    INodeMap& nodemap = camera.GetNodeMap();
    GenApi::CFloatPtr gain( nodemap.GetNode( "Gain"));
    GenApi::CFloatPtr exposuretime( nodemap.GetNode( "ExposureTime"));
    gain->SetValue(0);  //gain must be greater than 0, less than 23
    exposuretime->SetValue(100000);  //exposure time must be greater than 35 us.
    GenApi::CIntegerPtr width(camera.GetNodeMap().GetNode("Width"));
    GenApi::CIntegerPtr height(camera.GetNodeMap().GetNode("Height"));
    camera.StartGrabbing();
    CPylonImage image;
    CImageFormatConverter fc;
    fc.OutputPixelFormat = PixelType_Mono8;

    // Prepartion for oepnCV
    namedWindow("CM_Image", WINDOW_AUTOSIZE); moveWindow("CM_Image", 0, 0);
    namedWindow("Projection_Horizontal", WINDOW_AUTOSIZE); moveWindow("Projection_Horizontal", 0, ywindows_margin + ywidthPixels * resize_factor);
    namedWindow("Projection_Vertical", WINDOW_AUTOSIZE); moveWindow("Projection_Vertical", xwidthPixels * resize_factor, 0);

    //preparetion for trackbar
    cvCreateTrackbar ("Gain", "CM_Image", &gain_slider, max_gain, on_tracker);
    cvCreateTrackbar ("exposure(*100000 us)", "CM_Image", &exposure_slider, max_exposure, on_tracker);
    cvCreateTrackbar ("0=Rb, 1=Fr", "CM_Image", &RbFr_slider, 1, on_tracker);
    Rect ROI(xlowPixels, ylowPixels, xwidthPixels, ywidthPixels);
    Rect target_ROI(xlowPixels_target, ylowPixels_target, xwidthPixels_target, ywidthPixels_target);

    Mat img(width->GetValue(), height->GetValue(), CV_8UC1, Scalar::all(0));
    Mat resized_img(xwidthPixels * resize_factor, ywidthPixels * resize_factor, CV_8UC1, Scalar::all(0));
    Mat cm_img(width->GetValue(), height->GetValue(), CV_8UC3, Scalar::all(0));
    Mat img_horizontal;
    Mat img_vertical;
    Mat cropped_img(xwidthPixels, ywidthPixels, CV_8UC1, Scalar::all(0));
    Mat img_target(xwidthPixels_target, ywidthPixels_target, CV_8UC1, Scalar::all(0));
    Mat img_target_threshold(xwidthPixels_target, ywidthPixels_target, CV_8UC1, Scalar::all(0));
    Mat projection_horizontal(projection_hight, ywidthPixels * resize_factor, CV_8UC1, Scalar::all(0));
    Mat projection_vertical(xwidthPixels * resize_factor, projection_hight, CV_8UC1, Scalar::all(0));


    MouseParams mp;
    setMouseCallback("CM_Image", &MouseCall, (void *)&mp);
    projection_horizontal = cv::Scalar(255);
    projection_vertical = cv::Scalar(255);

//generate ROI graph
    system ("rm ROI.txt");
    system (("gnome-terminal --geometry=80x20+1000+200 -x  root -l  .x 'ROI_graph.C(" + to_string(RbFr_slider) +to_string(",")+to_string(ROIlist_rate)+ to_string(") '")).c_str());
    ofstream ROIlistfilename("ROI.txt");
    ROIlistfilename << 0;
    ROIlistfilename << "\t";
    ROIlistfilename << 0;
    ROIlistfilename << "\n";

    ROIlistfilename.close();

//read voltage of MCP front
    FILE* fp;

    if ((fp = popen(("snmpget -v 2c -M /home/cyric/mibs -m +WIENER-CRATE-MIB -c public "+mpod_ip+to_string(" outputMeasurementSenseVoltage.306 |grep \"[0-9]*\\.[0-9]\" -o")).c_str(), "r")) != NULL) {

      while (fgets(vol_306, sizeof(vol_306), fp) != NULL) {
        cout << "MCP front is " << vol_306 << "\n";
      }
    }
    if (RbFr_slider == 0) {
      ROI_to_numbers = Rb_ROI_to_numbers / pow(factor_vol, (atoi(vol_306) - 1000) / 50) / exposure_slider;
    }
    else {
      ROI_to_numbers = Fr_ROI_to_numbers / pow(factor_vol, (atoi(vol_306) - 1000) / 50) /exposure_slider;
    }

    pclose(fp);

    while (camera.IsGrabbing()) {
      camera.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);

      if (ptrGrabResult->GrabSucceeded()) {

        if (slider_flag == 1) {
              if ((fp = popen(("snmpget -v 2c -M /home/cyric/mibs -m +WIENER-CRATE-MIB -c public "+mpod_ip+to_string(" outputMeasurementSenseVoltage.306 |grep \"[0-9]*\\.[0-9]\" -o")).c_str(), "r")) != NULL) {

      while (fgets(vol_306, sizeof(vol_306), fp) != NULL) {
      }
    }
          if (RbFr_slider == 0) {
            ion = "Rb";
            ROI_to_numbers = Rb_ROI_to_numbers / pow(factor_vol, (atoi(vol_306) - 1000) / 50) / exposure_slider;
          }
          else {
            ion ="Fr";
            ROI_to_numbers = Fr_ROI_to_numbers / pow(factor_vol, (atoi(vol_306) - 1000) / 50) /exposure_slider;
          }

          gain->SetValue(gain_slider);
          exposuretime->SetValue(exposure_slider*100000);
          slider_flag = 0;
        }
        //image conversion
        fc.Convert(image, ptrGrabResult);
        img = cv::Mat(ptrGrabResult->GetHeight(),     ptrGrabResult->GetWidth(), CV_8UC1, (uint8_t*)image.GetBuffer());
        cropped_img = img(ROI);
        img_target = img(target_ROI);
        threshold(img_target, img_target_threshold, threshold_value, 255, THRESH_TOZERO);
        targetROI_sum = sum(img_target_threshold).val[0] * ROI_to_numbers;
        resize(cropped_img, resized_img,  Size(), resize_factor, resize_factor, INTER_LINEAR);

        applyColorMap(resized_img, cm_img, COLORMAP_JET);

        //display text
        putText(cm_img, "MCP front: " + to_string(vol_306) + to_string(" V"), cv::Point(20, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(200, 200, 200), 2, CV_AA);
        putText(cm_img, "Gain: " + to_string(gain_slider), cv::Point(20, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(200, 200, 200), 2, CV_AA);
        putText(cm_img, "exposure time: " + to_string(exposure_slider*100000) + to_string(" us"), cv::Point(20, 60), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(200, 200, 200), 2, CV_AA);
        putText(cm_img, "number of "+ion+ to_string(" ions: ") + to_string(targetROI_sum)+to_string("\t gross: ") +to_string(sum(img_target_threshold).val[0] ), cv::Point(20, 80), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(200, 200, 200), 2, CV_AA);


        //show target Rect

        rectangle(cm_img, cv::Point(xlowPixels_target_cm, ylowPixels_target_cm), cv::Point(xupPixels_target_cm, yupPixels_target_cm), cv::Scalar(68, 174, 85), 2, 8, 0);
        //show color map
        imshow("CM_Image", cm_img);

        //load mouseparams
    mp.mouseparams_img = img;
    mp.mouseparams_cmimg = cm_img;
    mp.ROIlist  = ROIlist;
        //generate projection image
        projection_horizontal = cv::Scalar(255);
        projection_vertical = cv::Scalar(255);
        reduce(resized_img, img_horizontal, 0, CV_REDUCE_AVG, CV_64FC1 );
        reduce(resized_img, img_vertical, 1, CV_REDUCE_AVG, CV_64FC1);
        cv::minMaxLoc(img_horizontal, NULL, &max_horizontal, NULL, NULL);
        cv::minMaxLoc(img_vertical, NULL, &max_vertical, NULL, NULL);

        //display text on projection image
        putText(projection_horizontal, "intensity ave: "+to_string(max_horizontal), cv::Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(120), 2, CV_AA);
        putText(projection_vertical, to_string(max_vertical), cv::Point(100, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(120), 2, CV_AA);

        //draw graphs on projection image
        for (int i = 0; i < img_horizontal.total(); i++) {
          rectangle(projection_horizontal, cv::Point(i, 0.9 * projection_hight * (1 - img_horizontal.at<double>(i) / max_horizontal)), cv::Point(i + 1, projection_hight), cv::Scalar(50), -1, 0);
        }
        for (int i = 0; i < img_vertical.total(); i++) {
          rectangle(projection_vertical, cv::Point(0, i), cv::Point(0.9 * projection_hight * img_vertical.at<double>(i) / max_vertical, i + 1), cv::Scalar(50), -1, 0);
        }

        //show projection image
        imshow("Projection_Horizontal", projection_horizontal);
        imshow("Projection_Vertical", projection_vertical);

        //generate target ROI list
        time(&time_end2);
        if (time_end2 >= time_end1 + ROIlist_rate) {
          time_end1 = time_end2;
          if (ROIlist[0].size() > ROIlist_length) {
            ROIlist[0].erase(ROIlist[0].begin());
            ROIlist[1].erase(ROIlist[1].begin());
          }
          ROIlist[0].push_back(difftime(time_end2, time_start));
          ROIlist[1].push_back(targetROI_sum);
          ofstream ROIlistfilename("ROI.txt", ios_base::app);
          ROIlistfilename << difftime(time_end2, time_start);
          ROIlistfilename << "\t";
          ROIlistfilename << targetROI_sum;
          ROIlistfilename << "\n";

          ROIlistfilename.close();


        }
        mp.ROIlist  = ROIlist;
        int ROIlist_max = *std::max_element(ROIlist[1].begin(), ROIlist[1].end());

        key = waitKey(100);

        if (key == 27) {
          camera.StopGrabbing();
          system ("rm ROI.txt");
          break;
        }
      }
    }

  }

  catch (GenICam::GenericException &e)
  {
    // Error handling.
    cerr << "An exception occurred." << endl
         << e.GetDescription() << endl;
    exitCode = 1;
  }


  return exitCode;
}

