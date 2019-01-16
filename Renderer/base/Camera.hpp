#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "Node.hpp"
#include "Mat4.hpp"

class Camera : public Node {
    
public:
    
    virtual ~Camera();
    
    void init();
    
    static Camera * getInstance();
    
    Mat4 getViewMat() const;
    
    Mat4 getProjectionMat() const;
    
    /**
     The combined camera transformation matrix, including perspective projection.
     
     This is the complete matrix to use in the vertex shader.
     */
    Mat4 getViewProjectionMat() const;
    
    inline void setNearAndFarPlane(double n , double f){
        setNearPlane(n);
        setFarPlane(f);
    }
    
    void initLookAt(Vec3 lookAt);
    
    Mat4 getCurDirectionMat() const;
    
    inline void offsetDirection(double verticalAngle , double horizontalAngle){
        _pitch += verticalAngle;
        _yaw += horizontalAngle;
        _normalizeAngle();
    }
    
    inline double getNearPlane(){
        return _nearPlane;
    }
    
    inline void setNearPlane(double val){
        _nearPlane = val;
    }
    
    inline double getFarPlane(){
        return _farPlane;
    }
    
    inline void setFarPlane(double val){
        _farPlane = val;
    }
    
    inline void setMoveVelo(double val){
        _moveVelo = val;
    }
    
    inline double getMoveVelo(){
        return _moveVelo;
    }
    
    
    /**
     A unit vector representing the direction the camera is facing in world coordinate system
     */
    Vec3 forward();
    
    /**
     A unit vector representing the direction out of the top of the camera in world coordinate system
     */
    Vec3 up();
    
    /**
     A unit vector representing the direction to the right of the camera in world coordinate system
     */
    Vec3 right();
    
    inline void offsetPosition(Vec3 moveVec){
        _position = _position + moveVec;
    }
    
    /**
     The vertical viewing angle of the camera, in degrees.
     
     Determines how "wide" the view of the camera is. Large angles appear to be zoomed out,
     as the camera has a wide view. Small values appear to be zoomed in, as the camera has a
     very narrow view.
     
     The value must be between 0 and 180.
     */
    inline double getFieldOfView(){
        return _fovy;
    }
    
    inline void setFieldOfView(double fov){
        if (fov > _maxFov) {
            _fovy = _maxFov;
        }else if(fov < _minFov){
            _fovy = _minFov;
        }
    }
    
protected:
    
    Camera();
    
    static Camera * s_pCamera;
    
    inline void _normalizeAngle(){
        _yaw = fmodf(_yaw, 360.0f);
        //fmodf can return negative values, but this will make them all positive
        if(_yaw < 0.0f){
            _yaw += 360.0f;
        }
        
        if(_pitch > _maxPitch){
            _pitch = _maxPitch;
        }
        else if(_pitch < -_maxPitch){
            _pitch = -_maxPitch;
        }
    }
    
    Vec3 _lookAt;
    
    Vec3 _headVec;
    
    double _pitch = 0;
    
    double _yaw = 0;
    
    double _maxPitch = 150;
    
    double _nearPlane;
    
    double _farPlane;
    
    double _moveVelo = 100;
    
    double _minFov;
    
    double _fovy;
    
    double _maxFov;
};

#endif /* Camera_hpp */













