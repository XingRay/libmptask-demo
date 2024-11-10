#include <iostream>
#include <chrono>

#include "mediapipe/tasks/libmptask/vision/face_landmarker/face_landmarker.h"
#include "mediapipe/tasks/libmptask/vision/face_landmarker/face_landmarker_result.h"
#include "mediapipe/tasks/libmptask/vision/core/common.h"

#include <opencv2/opencv.hpp>

void printResult(const libmptask::FaceLandmarkerResult &result);

int testImage();

int testVideo();

int testCamera();

// Define the paths to the model and image
const std::string model_path = "D:/project/makeup/engine/mediapipe/task/face_landmarker.task";
const std::string image_path = "D:/project/makeup/img/face_image.png";

int main(int argc, char **argv) {
//    return testImage();
    return testVideo();
//    return testCamera();
}

int64_t getCurrentTimestampMs() {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return duration.count();
}

int testImage() {
    libmptask::FaceLandmarkerOptions options = libmptask::FaceLandmarkerOptions();
    options.base_options.model_asset_path = model_path.c_str();
    options.running_mode = libmptask::RunningMode::IMAGE;
    options.output_face_blendshapes = true;
    options.output_facial_transformation_matrixes = true;

    libmptask::FaceLandmarker *faceLandmarker = libmptask::FaceLandmarker::create(options, nullptr);
    libmptask::FaceLandmarkerResult result = faceLandmarker->detect(image_path);
    printResult(result);

    faceLandmarker->close();
    delete faceLandmarker;
    faceLandmarker = nullptr;

    std::cout << "Detection completed successfully." << std::endl;

    return 0;
}

void printResult(const libmptask::FaceLandmarkerResult &result) {
    std::cout << "===================  printResult  =========================" << std::endl;
    // Output face landmarks
    std::cout << "face_landmarks_count: " << result.face_landmarks.size() << std::endl;
    for (const auto &landmarks: result.face_landmarks) {
        std::cout << "Detected landmarks: " << std::endl;
        for (const auto &landmark: landmarks.landmarks) {
            std::cout << "( " << landmark.x << ", " << landmark.y << ", " << landmark.z << " )" << std::endl;
        }
    }

    // Output face blendshapes if available
    if (result.face_blendshapes.has_value()) {
        std::cout << "face_blendshapes_count: " << result.face_blendshapes->size() << std::endl;
        for (const auto &blendshape: *result.face_blendshapes) {
            std::cout << "Detected face blendshape: " << std::endl;
            std::cout << "head_index:" << blendshape.head_index << std::endl;
            if (blendshape.head_name.has_value()) {
                std::cout << "head_name:" << *blendshape.head_name << std::endl;
            } else {
                std::cout << "head_name has no value" << std::endl;
            }

            for (const auto &category: blendshape.categories) {
                std::cout << "index: " << category.index << ", score: " << category.score;
                std::cout << ", category_name: ";
                if (category.category_name.has_value()) {
                    std::cout << *category.category_name;
                } else {
                    std::cout << "N/A";
                }

                std::cout << "display_name: ";
                if (category.display_name.has_value()) {
                    std::cout << *category.display_name;
                } else {
                    std::cout << "N/A";
                }

                std::cout << std::endl;
            }
        }
    } else {
        std::cout << "No face blendshapes detected." << std::endl;
    }

    // Output facial transformation matrices if available
    if (result.facial_transformation_matrixes.has_value()) {
        std::cout << "facial_transformation_matrixes_count: " << result.facial_transformation_matrixes->size() << std::endl;
        for (const auto &matrix: *result.facial_transformation_matrixes) {
            std::cout << "Matrix (" << matrix.rows() << "x" << matrix.cols() << "):" << std::endl;
            for (int row = 0; row < matrix.rows(); ++row) {
                for (int col = 0; col < matrix.cols(); ++col) {
                    std::cout << matrix(row, col) << " ";
                }
                std::cout << std::endl;
            }
        }
    } else {
        std::cout << "No facial transformation matrices detected." << std::endl;
    }
}

// 识别并标记人脸关键点的函数
void markFaceLandmarks(cv::Mat &frame, const libmptask::FaceLandmarkerResult &landmarks) {
    for (const auto &face_landmarks: landmarks.face_landmarks) {
        for (const auto &landmark: face_landmarks.landmarks) {
            int x = static_cast<int>(landmark.x * frame.cols);
            int y = static_cast<int>(landmark.y * frame.rows);
            cv::circle(frame, cv::Point(x, y), 2, cv::Scalar(0, 255, 0), -1);
        }
    }
}

int testVideo() {
    const std::string video_path = "D:\\tmp\\001.mp4";
    cv::VideoCapture cap(video_path);
    if (!cap.isOpened()) {
        std::cerr << "Error opening video file: " << video_path << std::endl;
        return -1;
    }
    std::cout << "opening video file success, video_path:" << video_path << std::endl;

    libmptask::FaceLandmarkerOptions options = libmptask::FaceLandmarkerOptions();
    options.base_options.model_asset_path = model_path.c_str();
//    options.running_mode = libmptask::RunningMode::VIDEO;
    options.running_mode = libmptask::RunningMode::IMAGE;
    options.output_face_blendshapes = true;
    options.output_facial_transformation_matrixes = true;

    libmptask::FaceLandmarker *faceLandmarker = libmptask::FaceLandmarker::create(options, nullptr);
    if (faceLandmarker == nullptr) {
        std::cerr << "faceLandmarker create fialed" << std::endl;
        return -1;
    }

    cv::Mat frame;
    double fps = cap.get(cv::CAP_PROP_FPS);  // 获取视频帧率
    int delay = static_cast<int>(1000 / fps);  // 计算帧间隔（单位：ms）

    while (cap.read(frame)) {
        std::cout << "read frame" << std::endl;
        // 转换帧格式（假设您的引擎需要 RGB 格式）
        cv::Mat rgb_frame;
        cv::cvtColor(frame, rgb_frame, cv::COLOR_BGR2RGB);

        // 检测关键点
//        libmptask::FaceLandmarkerResult result = faceLandmarker->detectForVideo(rgb_frame.data, rgb_frame.cols, rgb_frame.rows, 3, getCurrentTimestampMs());
        libmptask::FaceLandmarkerResult result = faceLandmarker->detect(rgb_frame.data, rgb_frame.cols, rgb_frame.rows, 3);

        // 在帧上标记关键点
        markFaceLandmarks(frame, result);

        // 显示结果
        cv::imshow("Face Landmarks", frame);

        // 等待帧间隔时间，如果按键退出则跳出循环
        if (cv::waitKey(delay) >= 0) {
            break;
        }

        std::cout << "read next frame" << std::endl;
    }

    // release detection
    faceLandmarker->close();
    delete faceLandmarker;
    faceLandmarker = nullptr;

    // release cv
//    cap.release();
//    cv::destroyAllWindows();

    return 0;
}

int testCamera() {
    return 0;
}