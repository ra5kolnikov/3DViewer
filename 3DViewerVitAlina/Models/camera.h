#ifndef CAMERA_H
#define CAMERA_H

#include "qmatrix4x4.h"
#include "qvectornd.h"
class Camera
{
public:
    Camera()
        : eye_({0.0f, 0.0f, 0.0f})
        , view_direction_({0.f, 0.0f, 0.0f})
        , up_vector_({0.0f, 1.0f, 0.0f})
    {}
    QMatrix4x4 GetViewMatrix()
    {
        view_matrix_.setToIdentity();
        view_matrix_.lookAt(eye_, view_direction_, up_vector_);
        return view_matrix_;
    }
    void MoveForward(double speed)
    {
        eye_ -= QVector3D(0, 0, speed);
        view_direction_ -= QVector3D(0, 0, speed);

        // auto axis = QVector3D(1, 0, 0);
        // eye_ = QQuaternion::fromAxisAndAngle(axis, -speed) * eye_;

        // QVector3D new_pos = eye_ - QVector3D(0, 0, speed);
        // QVector3D center = {};
        // QVector3D distance_to_center = new_pos - center;
        // // Calculate the distance magnitude
        // double distance_magnitude = distance_to_center.length();

        // // Set a minimum distance threshold
        // double min_distance = 1.0; // Adjust as needed

        // // Check if the distance is greater than the minimum threshold
        // if (distance_magnitude > min_distance) {
        //     // Move the camera left
        //     eye_ = new_pos;
        // }
    }
    void MoveBackward(double speed)
    {
        eye_ += QVector3D(0, 0, speed);
        view_direction_ += QVector3D(0, 0, speed);
        // auto axis = QVector3D(1, 0, 0);
        // eye_ = QQuaternion::fromAxisAndAngle(axis, speed) * eye_;
        // QVector3D new_pos = eye_ + QVector3D(0, 0, speed);
        // QVector3D center = {};
        // QVector3D distance_to_center = new_pos - center;
        // // Calculate the distance magnitude
        // double distance_magnitude = distance_to_center.length();

        // // Set a minimum distance threshold
        // double min_distance = 1.0; // Adjust as needed

        // // Check if the distance is greater than the minimum threshold
        // if (distance_magnitude > min_distance) {
        //     // Move the camera left
        //     eye_ = new_pos;
        // }
    }
    void MoveLeft(double speed)
    {
        auto axis = QVector3D(0, 1, 0);
        eye_ = QQuaternion::fromAxisAndAngle(axis, -speed) * eye_;
        // eye_ -= QVector3D(speed, 0, 0);
        // view_direction_ -= QVector3D(speed, 0, 0);
        // QVector3D right = QVector3D::crossProduct(view_direction_, up_vector_);
        // eye_ -= right * speed;
        // QVector3D new_pos = eye_ - QVector3D(speed, 0, 0);
        // // Calculate the distance between the new position and the center of the scene
        // QVector3D center = {};
        // QVector3D distance_to_center = new_pos - center;
        // // Calculate the distance magnitude
        // double distance_magnitude = distance_to_center.length();

        // // Set a minimum distance threshold
        // double min_distance = 1.0; // Adjust as needed

        // // Check if the distance is greater than the minimum threshold
        // if (distance_magnitude > min_distance) {
        //     // Move the camera left
        //     eye_ = new_pos;
        // }
    }
    void MoveRight(double speed)
    {
        auto axis = QVector3D(0, 1, 0);
        eye_ = QQuaternion::fromAxisAndAngle(axis, speed) * eye_;
        // eye_ += QVector3D(speed, 0, 0);
        // view_direction_ += QVector3D(speed, 0, 0);
        // QVector3D right = QVector3D::crossProduct(view_direction_, up_vector_);
        // eye_ += right * speed;
        // QVector3D new_pos = eye_ + QVector3D(speed, 0, 0);
        // QVector3D center = {};
        // QVector3D distance_to_center = new_pos - center;
        // // Calculate the distance magnitude
        // double distance_magnitude = distance_to_center.length();

        // // Set a minimum distance threshold
        // double min_distance = 1.0; // Adjust as needed

        // // Check if the distance is greater than the minimum threshold
        // if (distance_magnitude > min_distance) {
        //     // Move the camera left
        //     eye_ = new_pos;
        // }
    }
    void MoveDown(double speed)
    {
        // QVector3D right = QVector3D::crossProduct(view_direction_, up_vector_);
        // eye_ += right * speed;
        eye_ -= QVector3D(0, speed, 0);
        view_direction_ -= QVector3D(0, speed, 0);
        // QQuaternion orbit_rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 2);
        // eye_ = orbit_rotation * -eye_;
        // QVector3D new_pos = eye_ - QVector3D(0, speed, 0);
        // QVector3D center = {};
        // QVector3D distance_to_center = new_pos - center;
        // // Calculate the distance magnitude
        // double distance_magnitude = distance_to_center.length();

        // // Set a minimum distance threshold
        // double min_distance = 1.0; // Adjust as needed

        // // Check if the distance is greater than the minimum threshold
        // if (distance_magnitude > min_distance) {
        //     // Move the camera left
        //     eye_ = new_pos;
        // }
    }
    void MoveUp(double speed)
    {
        eye_ += QVector3D(0, speed, 0);
        view_direction_ += QVector3D(0, speed, 0);
        // QVector3D right = QVector3D::crossProduct(view_direction_, up_vector_);
        // eye_ += right * speed;
        // QVector3D new_pos = eye_ + QVector3D(0, speed, 0);
        // QVector3D center = {};
        // QVector3D distance_to_center = new_pos - center;
        // // Calculate the distance magnitude
        // double distance_magnitude = distance_to_center.length();

        // // Set a minimum distance threshold
        // double min_distance = 1.0; // Adjust as needed

        // // Check if the distance is greater than the minimum threshold
        // if (distance_magnitude > min_distance) {
        //     // Move the camera left
        //     eye_ = new_pos;
        // }
    }
    void SetCameraPosition(QVector3D eye)
    {
        eye_ = eye;
        distance_ = eye_.z();
    }
    void SetCameraDirection(QVector3D view_direction) { view_direction_ = view_direction; }
    void Move(int direction)
    {
        double step = distance_ / 10;
        switch (direction) {
        case Qt::Key_A:
            MoveLeft(step);
            break;
        case Qt::Key_D:
            MoveRight(step);
            break;
        case Qt::Key_W:
            MoveForward(step);
            break;
        case Qt::Key_S:
            MoveBackward(step);
            break;
        case Qt::Key_2:
            MoveUp(step);
            break;
        case Qt::Key_1:
            MoveDown(step);
            break;
        }
        qDebug() << direction;
    }

private:
    QVector3D rotation_vector;
    QVector3D eye_;
    QVector3D view_direction_;
    QVector3D up_vector_;
    QMatrix4x4 view_matrix_;
    QVector3D old_mouse_pos_;
    double distance_;
};

#endif // CAMERA_H
