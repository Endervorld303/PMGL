#include "PMGL/base.hpp"
#include "PMGL/trasform.hpp"


/*
*==============================================================
*                 IMPLEMENTATION OF TRASFORM HEADER
*==============================================================
*/
namespace PMGL{
    trasform::trasform(glm::vec3 pos, glm::quat rot, glm::vec3 sc){
        position = pos;
        rotation = rot;
        scale = sc;
    }

    //Position
    glm::vec3 trasform::get_position() const{return position;}
    void trasform::set_position(const glm::vec3 &new_pos){
        position = new_pos;
        set_dirty();
        set_dirty_view();
    }

    void trasform::translate(const glm::vec3 &delta){
        position += delta;
        set_dirty();
    }

    

    //Rotation
    glm::quat trasform::get_rotation() const{return rotation;}
    glm::vec3 trasform::get_euler_rotation() const{return glm::eulerAngles(rotation);}

    void trasform::set_rotation(const glm::quat &rot){
        rotation = rot;
        set_dirty();
        set_dirty_view();
    }

    void trasform::set_euler_rotation(const glm::vec3 &eul_rot){
        glm::quat new_rot = glm::quat(eul_rot); 
        set_dirty();
        set_dirty_view();
    }

    void trasform::rotate(const glm::quat &delta){
        rotation = rotation * delta;
        set_dirty();
        set_dirty_view();
    }
    void trasform::rotate_world(const glm::quat &delta){
        rotation = delta * rotation;
        set_dirty();
        set_dirty_view();
    }
    void trasform::rotate_euler(const glm::vec3 &eul_delta){
        rotate(glm::quat(eul_delta));   
    }

    void trasform::rotate_euler_world(const glm::vec3 &eul_delta){
        rotate_world(glm::quat(eul_delta));
    }


    //Scale 
    glm::vec3 trasform::get_scale() const{return scale;}
    void trasform::set_scale(glm::vec3 new_scale){scale = new_scale;}


    //Model
    const glm::mat4& trasform::get_model_matrix() const{
        if (is_dirty)
        {
            glm::mat4 T = glm::translate(glm::mat4(1.0f),position);
            glm::mat4 R = glm::toMat4(rotation);
            glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
            model = T * R * S;
            is_dirty = false;
        }

        return model;
    }

    const glm::mat4& trasform::get_rigid_inverse_matrix() const{
        if(is_dirty_view){
            glm::mat4 R = glm::toMat4(glm::conjugate(rotation));
            glm::mat4 T = glm::translate(glm::mat4(1.0f), -position);
            view = R * T;
            is_dirty_view = false;
        }
        return view;
    }

    //Get Axis
    glm::vec3 trasform::get_up() const{return rotation * glm::vec3(0.0, 1.0, 0.0);}

    glm::vec3 trasform::get_right() const{return rotation * glm::vec3(1.0, 0.0, 0.0);}
    
    glm::vec3 trasform::get_forward() const{return rotation * glm::vec3(0.0, 0.0, -1.0);}

    /*
    * =============================
    *   FUNCTION ON TRASFORM
    * =============================
    */
    trasform interpolate(const trasform &a, const trasform &b, float t, bool use_slerp){
        trasform ret;
        ret.set_position(glm::mix(a.get_position(), b.get_position(), t));
        ret.set_scale(glm::mix(a.get_scale(), b.get_scale(), t));

        if(!use_slerp){
            ret.set_rotation(glm::mix(a.get_rotation(), b.get_rotation(), t));
        }else{
            ret.set_rotation(glm::slerp(a.get_rotation(), b.get_rotation(), t));
        }
        return ret;
    }

}