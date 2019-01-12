#ifndef Ref_hpp
#define Ref_hpp

#include <stdio.h>

class Ref {
public:
    Ref():
    _refCount(1){
        
    }
    
    void retain() {
        ++_refCount;
    }
    
    void release() {
        --_refCount;
        if (_refCount == 0) {
            delete this;
        }
    }
    
    virtual ~Ref() {
        
    }
    
protected:

    unsigned _refCount = 1;
};

#endif /* Ref_hpp */
