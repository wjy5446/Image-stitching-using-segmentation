# Image Stitching using Multiple Homographies Estimated by Segmented Regions for Different Parallaxes

> This is a framework that combines multiple images using multiple homographies.
>
> For reduce misalignment, we didved the images into background and foreground. Then, we apply background and foreground homographies to the images 

**Paper address** [link](https://ieeexplore.ieee.org/abstract/document/8123591/) "Lee, D., Yoon, J., & Lim, S. (2017, September). Image Stitching Using Multiple Homographies Estimated by Segmented Regions for Different Parallaxes. In *Vision, Image and Signal Processing (ICVISP), 2017 International Conference on* (pp. 71-75). IEEE."



![alt text](https://github.com/wjy5446/Image-stitching-using-segmentation/tree/master/image/abstract0.png)



## 1. Abstract

  We propose a novel image stitching method using multiple homographies. The method can stitch images having different parallaxes, such as an image that contains distant buildings and close trees. Such images might not be stitched with fine quality by single global homography. Therefore, we select a background homography by analyzing the inliers of the homographies estimated by RANSAC (random sample consensus) for leave-one-out segmented regions, and the left segmented region of the background homography is designated as an object region. Then, the object homography is estimated by the object region. Using these multiple homographies, image stitching and multi-band blending are performed. With this method, images having different parallaxes are stitched with higher visual quality than with other methods using single homography and multiple homographies.



## 2. Overall Method

![head](https://github.com/wjy5446/Image-stitching-using-segmentation/tree/master/image/method.png)



## 3. Foreground candidate detection

I divide the picture into background and foreground using inlier rate.



![head](https://github.com/wjy5446/Image-stitching-using-segmentation/tree/master/image/inlier.png)

- I use the inlier rate like the eqaution in the above figure.



![head](https://github.com/wjy5446/Image-stitching-using-segmentation/tree/master/image/case_inlier.png)

- In case of foreground, the inlier rate is high.
- In case of background, the inlier rate is low.



![head](https://github.com/wjy5446/Image-stitching-using-segmentation/tree/master/image/experiment_inlier.png)

- in the above figure, you can see that when the inlier rate is low, the foreground is selected.



## 4. Foreground validation

![head](https://github.com/wjy5446/Image-stitching-using-segmentation/blob/master/image/foreground%20validation.png)



- Selecting the foreground by inlier rate is often failed. So, I valid the selected foreground using the validation function.



- Finally, we estimate the background and foreground homography.



## 5. Warping and Blending

![head](https://github.com/wjy5446/Image-stitching-using-segmentation/blob/master/image/image_processing.png)

- Before the warping and blending, we preprocess the images for removing the noise and fill the holes.



![head](https://github.com/wjy5446/Image-stitching-using-segmentation/blob/master/image/Blending.png)

- Warping and blending are consisted two stage. First, the background is stitched using background homography. Then, the foreground is stitched using foreground homography.
- Blending algorithm is Multi-band blending. 



## 6. Result

- Compare the other method

![head](https://github.com/wjy5446/Image-stitching-using-segmentation/blob/master/image/result1.png)

- It is good performance when the image contain the prallaxes.



![head](https://github.com/wjy5446/Image-stitching-using-segmentation/blob/master/image/result2.png)

- But, the proposed method take a long time.



##### MFC

![head](https://github.com/wjy5446/Image-stitching-using-segmentation/blob/master/image/screenshot.png)



## Code

> It is based on OpenCV 2.4.1.1,  MFC

```sh
- Important code

ImageProcessing.h / ImageProcessing.cpp : including the basic imageProcessing function
imageStitch_jy.h / imageStitch_jy.cpp : including the main image stitching code
```

