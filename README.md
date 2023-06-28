## Calibration-and-Augmented-Reality-cpp
The repository contains the code files to calibrate a camera and then use the calibration to generate virtual objects in a scene. The program detects a target and then places a virtual object in the scene relative to the target that moves and orients itself correctly given motion of the camera or target.


The OpenCV package to calibrate a given camera and use the calibration parameters to augment virtual objects onto a checkerboard pattern. The checkerboard pattern is printed onto a surface and the user is prompted to hit ‘s’ as many number of time that the user wishes to save frames with the checkerboard pattern in it to go into the camera calibration function cv::calibrateCamera() as real-world object points and pixel-plane object points pair with correspondence between them, here the object being the checkerboard. Then the user hits ‘q’ which brings up the cv::imshow() window which show the checkerboard being tracked and drawn with green dots at its inner 5x8 checkercorner along with an axis at the first corner, as the user moves the checkerboard in front of the camera. Another cv::imshow() window shows an assymetrical object being augmented onto the checkerboard. Finally we use Harris corners as a feature being detected being detected on images which could be used as the basis for putting augmented reality into the image.


* Detecting and extracting chessboard corners and selecting calibration images

![image](https://github.com/josejosepht/Calibration-and-Augmented-Reality-cpp/assets/97187460/6feefbb5-7347-4573-9019-941816f7d8b0)


* Final error estimate after calibration

![image](https://github.com/josejosepht/Calibration-and-Augmented-Reality-cpp/assets/97187460/92e5027f-3205-4cda-9860-abbb67bedc0d)


* Writing camera matrix and distortion coefficients to file
![image](https://github.com/josejosepht/Calibration-and-Augmented-Reality-cpp/assets/97187460/07277f0a-23de-43f3-8920-84dc32650919)


* Rotation and translation data printed on console in real-time
![image](https://github.com/josejosepht/Calibration-and-Augmented-Reality-cpp/assets/97187460/92315973-9241-4721-a79f-840082c846dd)

* Project outside corners and 3D Axes
![image](https://github.com/josejosepht/Calibration-and-Augmented-Reality-cpp/assets/97187460/0bbb1967-27a8-4dcc-a907-d434afecbdc8)

* Augmenting an asymmetrical triangular prism object with colored edges
![image](https://github.com/josejosepht/Calibration-and-Augmented-Reality-cpp/assets/97187460/41cdc125-987b-4edd-a66e-786f85d00573)


Video demo:
https://github.com/josejosepht/Calibration-and-Augmented-Reality-cpp/assets/97187460/c8beefc1-4d30-4df8-b3c1-688b3558f46c


* Harris corner detection on checkerboard pattern target
![image](https://github.com/josejosepht/Calibration-and-Augmented-Reality-cpp/assets/97187460/d819c173-c75d-46ee-b8c2-8da6ca0d8faf)


* More augmented objects
![image](https://github.com/josejosepht/Calibration-and-Augmented-Reality-cpp/assets/97187460/39fd54a5-2099-4d84-955a-c05c6e775f8a)


Video demos:
https://github.com/josejosepht/Calibration-and-Augmented-Reality-cpp/assets/97187460/3ffd200e-f8cb-4767-a73e-af43ef0190ca


https://github.com/josejosepht/Calibration-and-Augmented-Reality-cpp/assets/97187460/24251fbb-1fdd-4f03-9a81-92ffa0c27f16

