#include <iostream>
#include "mediapipe/tasks/libmptask/vision/face_landmarker/face_landmarker.h"
#include "mediapipe/tasks/libmptask/vision/face_landmarker/face_landmarker_result.h"
#include "mediapipe/tasks/libmptask/vision/core/common.h"

void printResult(const libmptask::FaceLandmarkerResult &result);

int main() {
    // Define the paths to the model and image
    const std::string model_path = "D:/project/makeup/engine/mediapipe/task/face_landmarker.task";
    const std::string image_path = "D:/project/makeup/img/face_image.png";

    libmptask::FaceLandmarkerOptions options = libmptask::FaceLandmarkerOptions();
    options.base_options.model_asset_path = model_path.c_str();
    options.running_mode = libmptask::RunningMode::IMAGE;
    options.output_face_blendshapes = true;
    options.output_facial_transformation_matrixes = true;

    libmptask::FaceLandmarker *faceLandmark = libmptask::FaceLandmarker::create(options, nullptr);
    libmptask::FaceLandmarkerResult result = faceLandmark->detect(image_path);
    printResult(result);

    faceLandmark->close();
    delete faceLandmark;
    faceLandmark = nullptr;

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