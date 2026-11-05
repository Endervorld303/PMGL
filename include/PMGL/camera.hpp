#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include "trasform.hpp"

namespace PMGL{
    class camera{
        private:
        trasform trasf;
        float fov;
        float aspect;
        float near;
        float far;

        mutable glm::mat4 view = glm::mat4(1.0f);
        mutable glm::mat4 projection = glm::mat4(1.0f);
        mutable glm::mat4 view_projection_matrix = glm::mat4(1.0f);
        mutable bool is_dirty_view = true;
        mutable bool is_dirty_proj = true;

        void set_dirty_view(){is_dirty_view = true;};
        void set_dirty_proj(){is_dirty_proj = true;};
        void update_view() const;
        void update_proj() const;
        
        public:

        //COSTRUCTOR
        camera(trasform _tras = trasform(glm::vec3(0.0,0.0,0.0)), float _aspect = 0, float _near = 0, float _far = 0, float _fov = 45.0f);

        

        /*
            GETTER
        */
        //Trasform
        const trasform& get_trasform() const;

        //Projecton
        float get_aspect() const;
        float get_near() const;
        float get_far() const;
        float get_fov() const;

        //Final matrix
        const glm::mat4& getViewProjectionMatrix() const;
        




        /*
            SETTER
        */
        //Trasform
        void set_trasformation(const trasform &t); 
        

        //Projection
        void set_aspect(const float asp);
        void set_near(const float n);
        void set_far(const float f);
        void set_fov(const float f);

    };
}


#endif