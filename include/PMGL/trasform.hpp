#ifndef TRASFORM_INCLUDE
#define TRASFORM_INCLUDE


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>

namespace PMGL{

    /*
    * =======================================
    *           CLASS TRANSFORM
    * =======================================
    */
    class trasform{

        private:
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::quat rotation = glm::quat(1.0, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
        mutable glm::mat4 model = glm::mat4(1.0f);
        mutable glm::mat4 view = glm::mat4(1.0f);
        mutable bool is_dirty = true;
        mutable bool is_dirty_view = true;
        void set_dirty(){is_dirty = true;}
        void set_dirty_view(){is_dirty_view = true;}

        public:
        //Costructor
        trasform(glm::vec3 pos = glm::vec3(0.0f), glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3 sc = glm::vec3(1.0f));

        //Position
        glm::vec3 get_position() const;
        void set_position(const glm::vec3 &new_pos);
        void translate(const glm::vec3 &delta);

        //Rotation
        glm::quat get_rotation() const;
        glm::vec3 get_euler_rotation() const;
        void set_rotation(const glm::quat &rot);
        void set_euler_rotation(const glm::vec3 &eul_rot);
        void rotate(const glm::quat &delta);
        void rotate_world(const glm::quat &delta);
        void rotate_euler(const glm::vec3 &eul_delta);
        void rotate_euler_world(const glm::vec3 &eul_delta);

        //Scale
        glm::vec3 get_scale() const;
        void set_scale(const glm::vec3 new_scale);


        //Get axis 
        glm::vec3 get_up() const;
        glm::vec3 get_right() const;
        glm::vec3 get_forward() const;

        //Model
        const glm::mat4& get_model_matrix() const;
        const glm::mat4& get_rigid_inverse_matrix() const;

        //All
    };


    /*
    * =============================
    *   FUNCTION ON TRASFORM
    * =============================
    */

    trasform interpolate(const trasform& a, const trasform& b, float t, bool use_slerp = false);
}


#endif