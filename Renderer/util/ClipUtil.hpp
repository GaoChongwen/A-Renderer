#ifndef ClipUtil_hpp
#define ClipUtil_hpp

#include <vector>
#include "Canvas.hpp"
#include "Vertex.hpp"

class ClipUtil{
public:
    static void doClippingInCvv(vector<Triangle> &triList){
        _doClppingInCvvAgainstNearPlane(triList);
        _doClppingInCvvAgainstFarPlane(triList);
        _doClppingInCvvAgainstLeftPlane(triList);
        _doClppingInCvvAgainstRightPlane(triList);
        _doClppingInCvvAgainstTopPlane(triList);
        _doClppingInCvvAgainstBottomPlane(triList);
    }

    static void _doClppingInCvvAgainstNearPlane(vector<Triangle> &triList) ;

    static void _doClppingInCvvAgainstFarPlane(vector<Triangle> &triList) ;

    static void _doClppingInCvvAgainstRightPlane(vector<Triangle> &triList) ;
  
    static void _doClppingInCvvAgainstLeftPlane(vector<Triangle> &triList) ;
 
    static void _doClppingInCvvAgainstTopPlane(vector<Triangle> &triList) ;
 
    static void _doClppingInCvvAgainstBottomPlane(vector<Triangle> &triList) ;
 
    static vector<bool> _getbOutListForNearPlane(Triangle &tri) ;

    static vector<bool> _getbOutListForFarPlane(Triangle &tri) ;
 
    static vector<bool> _getbOutListForRightPlane(Triangle &tri) ;
 
    static vector<bool> _getbOutListForLeftPlane(Triangle &tri) ;

    static vector<bool> _getbOutListForTopPlane(Triangle &tri) ;
 
    static vector<bool> _getbOutListForBottomPlane(Triangle &tri) ;

    static inline Ldouble _getFactorForNearPlane(const VertexOut &vertIn, const VertexOut &vertOut) 
    {
    return (vertIn.pos.z - 0) / (vertIn.pos.z - vertOut.pos.z);
    }

    static inline Ldouble _getFactorForFarPlane(const VertexOut &vertIn, const VertexOut &vertOut)
    {
    return (vertIn.pos.z - vertIn.pos.w) / (vertIn.pos.z - vertOut.pos.z + vertOut.pos.w - vertIn.pos.w);
    }

    static inline Ldouble _getFactorForRightPlane(const VertexOut &vertIn, const VertexOut &vertOut)
    {
    return (vertIn.pos.x - vertIn.pos.w) / (vertIn.pos.x - vertOut.pos.x + vertOut.pos.w - vertIn.pos.w);
    }

    static inline Ldouble _getFactorForLeftPlane(const VertexOut &vertIn, const VertexOut &vertOut)
    {
    return (vertIn.pos.x + vertIn.pos.w) / (vertIn.pos.x - vertOut.pos.x + vertIn.pos.w - vertOut.pos.w);
    }

    static inline Ldouble _getFactorForTopPlane(const VertexOut &vertIn, const VertexOut &vertOut) 
    {
    return (vertIn.pos.y - vertIn.pos.w) / (vertIn.pos.y - vertOut.pos.y + vertOut.pos.w - vertIn.pos.w);
    }

    static inline Ldouble _getFactorForBottomPlane(const VertexOut &vertIn, const VertexOut &vertOut)
    {
        return (vertIn.pos.y + vertIn.pos.w) / (vertIn.pos.y - vertOut.pos.y + vertIn.pos.w - vertOut.pos.w);
    }
};

#endif 