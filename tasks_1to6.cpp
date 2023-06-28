//Jose Joseph Thandapral
//CS5330
//Project 4 : Calibration and Augmented Reality 
//Tasks 1 to 6  

#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <fstream>
using namespace cv;
using namespace std;

int main()
{
    int imgId = 1;
    Mat frame, grayImg, imgCopy;
    char key;
    vector<Point2f> corner_set;
    vector<Point2f> corner_set1;
    vector<vector<Point2f>> corner_list;
    vector<Point3f> point_set;
    vector<vector<Point3f>> point_list;
    Size patternSize = Size(9, 6);
    std::vector<cv::Mat> calibrationImages;
    cv::VideoCapture* capdev;
    cv::Mat cameraMatrix, distortionCoefficients;
    std::vector<cv::Mat> rvecs, tvecs;
    double error;
    //Assigning cameraMatrix to be of type CV_64FC1
    cameraMatrix = Mat(3,3, CV_64FC1);

    //Initialize point set
    for (int y = 0; y > -6; y--) {
        for (int x = 0; x < 9; x++) {
            point_set.push_back(Point3f(x, y, 0));

        }
    }

    // open the video device
    capdev = new cv::VideoCapture(1);

    //// Replace the IP address and port number with that of your phone's camera
    //std::string ip_address = "10.0.0.141";
    //std::string port_number = "8080";
    //// Create a VideoCapture object to connect to the camera stream
    //VideoCapture capture("http://" + ip_address + ":" + port_number + "/video");

    if (!capdev->isOpened()) {

        std::cout << "Error opening camera stream" << std::endl;
        return -1;
    }

    while (true)
    { 
        Mat cpy;
        *capdev >> frame;
        if (frame.empty()) {
            printf("Image is empty\n");
            break;
        }
        imshow("Frame", frame);
        cpy = frame.clone();
        //get the input from the keyboard
        key = waitKey(30);
        //Convert the current frame to grayscale
        cvtColor(frame, grayImg, COLOR_BGR2GRAY);

//                          TASK 1: Detect and Extract Chessboard Corners

        //check for a chessboard pattern in current frame image
        bool flag = findChessboardCorners(grayImg, patternSize, corner_set);
        if (flag) {
            imgCopy = frame.clone();
            //find the pixel location of corners on chessboard patter in frame
            cornerSubPix(grayImg, corner_set, Size(11, 11), Size(-1, -1),
                TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.1));
        }
        //draw the chessboard corners and display it
        drawChessboardCorners(frame, patternSize, Mat(corner_set), flag);
        imshow("Video", frame);
        key = waitKey(10);
        if (key == 's')
        {

//                          TASK 2: Select Calibration Images

            std::string imgName = "calibImg" + std::to_string(imgId) + ".jpg";
            //imwrite(imgName, imgCopy);
            imwrite(imgName, cpy);
            calibrationImages.push_back(cv::imread(imgName));
            cout << "\nSaved calibImg" << std::to_string(imgId) << ".jpg\n";
            imshow("saved calib image", cv::imread(imgName));
            ++imgId;
        }
        if (key == 'q' || key == 27)
            break;
    }

    // Loop over the calibration images and find the corners
    for (int i = 0; i < calibrationImages.size(); i++) {
        // Convert the image to grayscale
        imshow("current calib img", calibrationImages[i]);
        cv::cvtColor(calibrationImages[i], grayImg, cv::COLOR_BGR2GRAY);

        // Find the corners
        std::vector<cv::Point2f> corners;
        bool found = cv::findChessboardCorners(grayImg, patternSize, corners);
        //cout << "found or not " << found;

        // If the corners are found, save the corner locations and 3D world points
        if (found) {
            corner_list.push_back(corners);
            point_list.push_back(point_set);
        }

        if(i==0)
        {
            error = cv::calibrateCamera(point_list, corner_list, calibrationImages[0].size(), cameraMatrix, distortionCoefficients, rvecs, tvecs);
            cout << "\nParameters before calibration:\n";
            cout << "camera_matrix: " << cameraMatrix << "\n" << "distortion_coeff: " << distortionCoefficients;
            cout << "\nerror: " << error << "\n";
        }

        // Draw the corners on the image and save it
        cv::drawChessboardCorners(calibrationImages[i], patternSize, corners, found);
        std::string filename = "calibImg" + std::to_string(i + 1) + ".jpg";
        cv::imwrite(filename, calibrationImages[i]);
    }

//                          TASK 3: Calibrate the Camera    

    // Calibrate the camera
    error = cv::calibrateCamera(point_list, corner_list, calibrationImages[0].size(), cameraMatrix, distortionCoefficients, rvecs, tvecs);
    cout << "\nParameters after calibration:\n";
    cout << "camera_matrix: " << cameraMatrix << "\n" << "distortion_coeff: " << distortionCoefficients;
    cout << "\nerror: " << error << "\n";

    cv::destroyAllWindows();
    // Open a file for writing
    std::ofstream file("camera_params.txt");

    // Write the camera matrix to the file
    file << "cameraMatrix:" << std::endl << cameraMatrix << std::endl;

    // Write the distortion coefficients to the file
    file << "distortionCoefficients:" << std::endl << distortionCoefficients << std::endl;

    // Close the file
    file.close();

    while (true)
    {
//                          TASK 4: Calculate Current Position of the Camera  
        *capdev >> frame;
        if (frame.empty()) {
            printf("Image is empty\n");
            break;
        }
        Mat framecpy = frame.clone();
        Mat framecpy1 = frame.clone();
        Mat framecpy2 = frame.clone();
        Mat framecpy3 = frame.clone();
        //Convert the current frame to grayscale
        cvtColor(frame, grayImg, COLOR_BGR2GRAY);
        //check for a chessboard pattern in current frame image
        bool flag = findChessboardCorners(grayImg, patternSize, corner_set1);
        if (flag) {
            //find the pixel location of corners on chessboard patter in frame
            cornerSubPix(grayImg, corner_set1, Size(11, 11), Size(-1, -1),
                TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.1));

            // Get the pose of the chessboard
            Mat rvec, tvec;
            solvePnP(point_set, corner_set1, cameraMatrix, distortionCoefficients, rvec, tvec);

            // Print out the rotation and translation data
            cout << "Rotation vector:" << endl << rvec << endl;
            cout << "Translation vector:" << endl << tvec << endl;

            // The 3D points of the corners of the checkerboard
            std::vector<cv::Point3f> boardPoints; 
            // Define the board points in the order of the corners detected by the algorithm
            boardPoints.push_back(cv::Point3f(0, 0, 0));
            boardPoints.push_back(cv::Point3f(8, 0, 0));
            boardPoints.push_back(cv::Point3f(0, -5, 0));
            boardPoints.push_back(cv::Point3f(8, -5, 0));

//                          TASK 5: Project Outside Corners or 3D Axes

            // Project the 3D points onto the image plane
            std::vector<cv::Point2f> projectedPoints;
            cv::projectPoints(boardPoints, rvec, tvec, cameraMatrix, distortionCoefficients, projectedPoints);

            // Draw the projected points on the image
            for (const auto& p : projectedPoints) {
                cv::circle(frame, p, 5, cv::Scalar(0, 255, 0), -1);
            }

            // Draw a 3D axis on the first corner of the checkerboard pattern detected
            cv::Mat R;
            cv::Rodrigues(rvec, R);
            cv::drawFrameAxes(frame, cameraMatrix, distortionCoefficients, R, tvec, 1);

            // Draw the chessboard and its axes
            //drawChessboardCorners(frame, patternSize, corner_set1, flag);
            imshow("Axes and corners of checkerboard", frame);

//                          TASK 6: Create a Virtual Object

            // The 3D points of the corners of the checkerboard
            std::vector<cv::Point3f> boardPoints1;
            // Define the board points in the order of the corners detected by the algorithm
            //The below boardPoints1 object is for an assymetrical triangular prism with different
            //colors for edges
            boardPoints1.push_back(cv::Point3f(1, 1, 0));
            boardPoints1.push_back(cv::Point3f(7, 1, 0));
            boardPoints1.push_back(cv::Point3f(1, -4, 0));
            boardPoints1.push_back(cv::Point3f(7, -4, 0));
            boardPoints1.push_back(cv::Point3f(3, -2.5, 3));
            boardPoints1.push_back(cv::Point3f(5, -2.5, 3));

            // Project the 3D points onto the image plane
            std::vector<cv::Point2f> projectedPoints1;
            cv::projectPoints(boardPoints1, rvec, tvec, cameraMatrix, distortionCoefficients, projectedPoints1);

            // Draw the projected points on the image
            for (const auto& p : projectedPoints1) {
                cv::circle(framecpy, p, 5, cv::Scalar(0, 255, 0), -1);
            }
            line(framecpy, projectedPoints1[0], projectedPoints1[1], Scalar(255, 0, 0), 4, LINE_8);
            line(framecpy, projectedPoints1[1], projectedPoints1[3], Scalar(0, 255, 0), 4, LINE_8);
            line(framecpy, projectedPoints1[2], projectedPoints1[3], Scalar(255, 0, 0), 4, LINE_8);
            line(framecpy, projectedPoints1[2], projectedPoints1[0], Scalar(0, 255, 0), 4, LINE_8);
            line(framecpy, projectedPoints1[0], projectedPoints1[4], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy, projectedPoints1[2], projectedPoints1[4], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy, projectedPoints1[1], projectedPoints1[5], Scalar(255, 0, 0), 4, LINE_8);
            line(framecpy, projectedPoints1[3], projectedPoints1[5], Scalar(255, 0, 0), 4, LINE_8);
            line(framecpy, projectedPoints1[4], projectedPoints1[5], Scalar(85, 85, 85), 4, LINE_8);
            imshow("assymetrical triangular prism", framecpy);

            //More 3D objects - Extensions: Spiral object, Paper plane, making the target not look like original target
            std::vector<cv::Point3f> boardPoints2;
            boardPoints2.push_back(cv::Point3f(4, 1, 0));
            boardPoints2.push_back(cv::Point3f(7, 1, 0));
            boardPoints2.push_back(cv::Point3f(7, -4, 0));
            boardPoints2.push_back(cv::Point3f(4, -4, 0));

            boardPoints2.push_back(cv::Point3f(4, 1, 3));
            boardPoints2.push_back(cv::Point3f(7, 1, 3));
            boardPoints2.push_back(cv::Point3f(7, -4, 3));
            boardPoints2.push_back(cv::Point3f(4, -4, 3));


            boardPoints2.push_back(cv::Point3f(4, 1, 6));
            boardPoints2.push_back(cv::Point3f(7, 1, 6));
            boardPoints2.push_back(cv::Point3f(7, -4, 6));
            boardPoints2.push_back(cv::Point3f(4, -4, 6));

            // Project the 3D points onto the image plane
            std::vector<cv::Point2f> projectedPoints2;
            cv::projectPoints(boardPoints2, rvec, tvec, cameraMatrix, distortionCoefficients, projectedPoints2);
            line(framecpy1, projectedPoints2[0], projectedPoints2[1], Scalar(255, 0, 0), 4, LINE_8);
            line(framecpy1, projectedPoints2[1], projectedPoints2[2], Scalar(255, 0, 0), 4, LINE_8);
            line(framecpy1, projectedPoints2[2], projectedPoints2[3], Scalar(255, 0, 0), 4, LINE_8);
            line(framecpy1, projectedPoints2[3], projectedPoints2[0], Scalar(255, 0, 0), 4, LINE_8);

            line(framecpy1, projectedPoints2[0], projectedPoints2[5], Scalar(0, 255, 0), 4, LINE_8);
            line(framecpy1, projectedPoints2[1], projectedPoints2[6], Scalar(0, 255, 0), 4, LINE_8);
            line(framecpy1, projectedPoints2[2], projectedPoints2[7], Scalar(0, 255, 0), 4, LINE_8);
            line(framecpy1, projectedPoints2[3], projectedPoints2[4], Scalar(0, 255, 0), 4, LINE_8);

            line(framecpy1, projectedPoints2[4], projectedPoints2[5], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy1, projectedPoints2[5], projectedPoints2[6], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy1, projectedPoints2[6], projectedPoints2[7], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy1, projectedPoints2[7], projectedPoints2[4], Scalar(0, 0, 255), 4, LINE_8);

            line(framecpy1, projectedPoints2[4], projectedPoints2[9], Scalar(0, 255, 0), 4, LINE_8);
            line(framecpy1, projectedPoints2[5], projectedPoints2[10], Scalar(0, 255, 0), 4, LINE_8);
            line(framecpy1, projectedPoints2[6], projectedPoints2[11], Scalar(0, 255, 0), 4, LINE_8);
            line(framecpy1, projectedPoints2[7], projectedPoints2[8], Scalar(0, 255, 0), 4, LINE_8);

            line(framecpy1, projectedPoints2[8], projectedPoints2[9], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy1, projectedPoints2[9], projectedPoints2[10], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy1, projectedPoints2[10], projectedPoints2[11], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy1, projectedPoints2[11], projectedPoints2[8], Scalar(0, 0, 255), 4, LINE_8);
            imshow("assymetrical spiral", framecpy1);

            std::vector<cv::Point3f> boardPoints3;
            boardPoints3.push_back(cv::Point3f(3, -3.5, 1.5));
            boardPoints3.push_back(cv::Point3f(10, -3.5, 1.5));

            boardPoints3.push_back(cv::Point3f(6.5, -5, 4));
            boardPoints3.push_back(cv::Point3f(3, -5, 4));
            boardPoints3.push_back(cv::Point3f(6.5, -2, 4));
            boardPoints3.push_back(cv::Point3f(3, -2, 4));

            boardPoints3.push_back(cv::Point3f(5.5, -7, 4));
            boardPoints3.push_back(cv::Point3f(2, -7, 4));
            boardPoints3.push_back(cv::Point3f(5.5, 0, 4));
            boardPoints3.push_back(cv::Point3f(2, 0, 4));

            std::vector<cv::Point2f> projectedPoints3;
            cv::projectPoints(boardPoints3, rvec, tvec, cameraMatrix, distortionCoefficients, projectedPoints3);
            

            line(framecpy2, projectedPoints3[0], projectedPoints3[1], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy2, projectedPoints3[1], projectedPoints3[2], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy2, projectedPoints3[2], projectedPoints3[3], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy2, projectedPoints3[0], projectedPoints3[3], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy2, projectedPoints3[0], projectedPoints3[5], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy2, projectedPoints3[1], projectedPoints3[4], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy2, projectedPoints3[4], projectedPoints3[5], Scalar(0, 0, 255), 4, LINE_8);

            line(framecpy2, projectedPoints3[2], projectedPoints3[6], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy2, projectedPoints3[6], projectedPoints3[7], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy2, projectedPoints3[3], projectedPoints3[7], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy2, projectedPoints3[4], projectedPoints3[8], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy2, projectedPoints3[8], projectedPoints3[9], Scalar(0, 0, 255), 4, LINE_8);
            line(framecpy2, projectedPoints3[5], projectedPoints3[9], Scalar(0, 0, 255), 4, LINE_8);

            imshow("Paper plane", framecpy2);

            //Augmenting whole checkerboard surface witha dartboard target pattern
            std::vector<cv::Point3f> boardPoints4;
            boardPoints4.push_back(cv::Point3f(4, -2.5, 0));
            std::vector<cv::Point2f> projectedPoints4;
            cv::projectPoints(boardPoints4, rvec, tvec, cameraMatrix, distortionCoefficients, projectedPoints4);
            for (const auto& p : projectedPoints4) {
                cv::circle(framecpy3, p, 200, cv::Scalar(0, 255, 0), -1);
                cv::circle(framecpy3, p, 150, cv::Scalar(255, 0, 0), -1);
                cv::circle(framecpy3, p, 100, cv::Scalar(0, 0, 255), -1);
                cv::circle(framecpy3, p, 50, cv::Scalar(255, 255, 255), -1);
            }
            imshow("Checkerboard changed to other color", framecpy3);

        }

        key = waitKey(30);
        if (key == 'q' || key == 27)
            break;
    }

    while (cv::waitKey(10) != 'q');
    cv::destroyAllWindows();
    return 0;
}