#include "PMGL/camera.hpp"
#include "PMGL/base.hpp"

/*
===================
    PRIVATE
===================
*/
void PMGL::camera::update_view() const{
    if(is_dirty_view){
        view = trasf.get_rigid_inverse_matrix();
        is_dirty_view = false;
    }
}

void PMGL::camera::update_proj() const{
    if(is_dirty_proj){ 
        projection = glm::perspective(glm::radians(fov), aspect, near, far);
        is_dirty_proj = false;
    }
}


/*
========================
    PUBLIC
========================
*/
/*
    COSTRUCTOR
*/
PMGL::camera::camera(trasform _tras, float _aspect, float _near, float _far, float _fov) : trasf(_tras), fov(_fov), aspect(_aspect), near(_near), far(_far)
{
    set_dirty_view();
    set_dirty_proj();
}



/*
    GETTER
*/
//Trasform
const PMGL::trasform& PMGL::camera::get_trasform() const{
    return trasf;
}

//Projection
float PMGL::camera::get_aspect() const{
    return aspect;
}

float PMGL::camera::get_near() const{
    return near;
}

float PMGL::camera::get_far() const{
    return far;
}

float PMGL::camera::get_fov() const{
    return fov;
}

//FULL MATRIX
const glm::mat4 &PMGL::camera::getViewProjectionMatrix() const
{
    if(is_dirty_view || is_dirty_proj){
        update_proj();
        update_view();
        view_projection_matrix = projection * view;
    }
    return view_projection_matrix;
}



/*
========================
    SETTER
========================
*/

void PMGL::camera::set_trasformation(const trasform &t){
    set_dirty_view();
    trasf = t;
}

void PMGL::camera::set_aspect(const float asp){
    set_dirty_proj();
    aspect = asp;
}

void PMGL::camera::set_near(const float n){
    set_dirty_proj();
    near = n;
}

void PMGL::camera::set_far(const float f){
    set_dirty_proj();
    far = f;

}

void PMGL::camera::set_fov(const float f){
    set_dirty_proj();
    fov = f;
}
