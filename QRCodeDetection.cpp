
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>


#include <stdlib.h>
#include <stdio.h>
#include <iostream>

// Include files to use the pylon API.
#include <pylon/PylonIncludes.h>


// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using cout.
using namespace std;


// Namespace for using OpenCV objects.
using namespace cv;


int main(int argc, char* argv[])
{
    // The exit code of the sample application.
    int exitCode = 0;


    Pylon::PylonAutoInitTerm autoInitTerm;

    // Before using any pylon methods, the pylon runtime must be initialized.
    //PylonInitialize();

    try
    {
        // Create an instant camera object with the camera device found first.
        Pylon::CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());

        // Print the model name of the camera.
        std::cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;


        GenApi::INodeMap& nodemap   = camera.GetNodeMap();

        camera.Open();

        GenApi::CIntegerPtr width   = nodemap.GetNode("Width");
        GenApi::CIntegerPtr height  = nodemap.GetNode("Height");

        // The parameter MaxNumBuffer can be used to control the count of buffers
        // allocated for grabbing. The default value of this parameter is 10.
        camera.MaxNumBuffer = 5;

        Pylon::CImageFormatConverter formatConverter;

        formatConverter.OutputPixelFormat = PixelType_BGR8packed;

        CPylonImage pylonImage;
        
        cv::Mat openCvImage;
        
        std::vector<cv::Point> QRCodePoints;

        cv::Mat rectifiedQRCodeImage;

        cv::QRCodeDetector qrDecoder = cv::QRCodeDetector();


        camera.StartGrabbing( GrabStrategy_LatestImageOnly );

        CGrabResultPtr ptrGrabResult;

        // Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // 
        while ( camera.IsGrabbing() )
        {
            // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
            camera.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);

            // Image grabbed successfully?
            if (ptrGrabResult->GrabSucceeded())
            {
              
                formatConverter.Convert(pylonImage,ptrGrabResult);

                openCvImage = cv::Mat( ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *)pylonImage.GetBuffer() );

                std:string data = qrDecoder.detectAndDecode(openCvImage,QRCodePoints,rectifiedQRCodeImage);

                if( data.length() > 0 )
                {
                    std::cout << "QRCode Data:  " << data << endl;


                   for( int i = 0; i < QRCodePoints.size() - 1 ; i++ )
                   {
                     cv::line( openCvImage, QRCodePoints.at(i) ,  QRCodePoints.at( i + 1 ), cv::Scalar(255, 0, 0) , 2, 8, 0 );
                   }

                    cv::line( openCvImage, QRCodePoints.at(0) ,  QRCodePoints.at(3), cv::Scalar(255, 0, 0) , 2, 8, 0 );
       
                }

                namedWindow("OpenCV Display Window", cv::WindowFlags::WINDOW_AUTOSIZE);
                imshow("OpenCV Display Window", openCvImage);

                if (waitKey(1) != -1)
                    break;

            }
            else
            {
                cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
            }


        }

    }

    catch (const GenericException &e)
    {
        // Error handling.
        cerr << "An exception occurred." << endl
        << e.GetDescription() << endl;
        exitCode = 1;
    }


    // Uncomment the following two lines to enable waiting on exit.
    // cerr << endl << "Press Enter to exit." << endl;
    // while( cin.get() != '\n');

    // Releases all pylon resources.
    PylonTerminate();


    cv::destroyWindow( "OpenCV Display Window" );

    return exitCode;
}
