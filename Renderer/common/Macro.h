#ifndef ZMath_h
#define ZMath_h

#define create_func(type) static type * create() { \
auto ret = new type(); \
ret->init(); \
return ret; \
}
enum LightType {SpotLight, DirectionalLight};

#define Setter_and_getter_vec3(varName , funcName) \
public : inline void set##funcName(const Vec3 &val) { \
    this->varName = val; \
} \
public : inline Vec3 get##funcName() const { \
    return this->varName; \
} \
public : inline void set##funcName##X(double val){\
    this->varName.x = val; \
}\
public : inline double get##funcName##X() { \
    return this->varName.x; \
} \
public : inline void set##funcName##Y(double val){\
    this->varName.y = val; \
}\
public : inline double get##funcName##Y() { \
    return this->varName.y; \
} \
public : inline void set##funcName##Z(double val){\
    this->varName.z = val; \
}\
public : inline double get##funcName##Z() { \
    return this->varName.z; \
} \
protected : Vec3 varName;


#endif /* ZMath_h */
