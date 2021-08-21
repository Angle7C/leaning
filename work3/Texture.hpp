//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
// #include <opencv2/opencv.hpp>
#include <optional>
#include <opencv2/opencv.hpp>
#include <Eigen/Eigen>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {   
        cv::Mat temp = cv::imread(name);
        // cv::namedWindow("原图");
	    // cv::imshow("原图", temp);
        cv::cvtColor(temp, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

};
#endif //RASTERIZER_TEXTURE_H