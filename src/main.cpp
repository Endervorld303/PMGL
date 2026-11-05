#include "PMGL/base.hpp"
#include "PMGL/window.hpp"

#include "PMGL/shader.hpp"
#include "PMGL/trasform.hpp"
#include "PMGL/camera.hpp"
#include "PMGL/light.hpp"
#include "PMGL/model.hpp"
#include "PMGL/window.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <stb_image.h>
#include <filesystem>

const float radius = 10.0f;
const float sensitivity = 0.002f;

float aspect = 800.0f / 600.0f;
float deltaTime = 0;
float fov = 45.0f;


PMGL::vector2d mouse_update(PMGL::window &w);


int main(){
   
   //Init
   PMGL::init_PMGL(3, 3, GLFW_OPENGL_CORE_PROFILE);

   //Setting up window
   PMGL::window win(800, 600, "PMGL");
   win.active();
   win.set_vsync(1);

   //Setting up GLAD loader
   if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
         std::cout << "Failed to initialize GLAD \n";
         return -1;
   }
   
   win.set_cursor_mode(PMGL::cursor_mode::normal);

   PMGL::shader shad1(std::vector<std::string>{"./resource/shaders/base.vert","./resource/shaders/base.frag", "./resource/shaders/light_base.frag", "./resource/shaders/textureMix.frag"});
   PMGL::shader lightShader(std::vector<std::string>{"./resource/shaders/base.vert", "./resource/shaders/lightOrigin.frag"});
   float vertices[] = {
   
      //Vertices              //Normal             Textrue
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,     1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,        1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,     1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,      0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,      0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,       0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,       1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,       1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,       1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,       0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,        0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,         1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,      1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,      0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,       0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,      0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,        1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,      1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,      1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,       0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,       0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,       0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,      1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,      0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,      1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,      1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,       1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,      0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,      0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,      0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,      1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,        1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,       1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,      0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,    0.0f, 1.0f
   };

   unsigned int indeces[] = {
      0, 1, 3,
      1, 2, 3
   };

      //VBA
   unsigned int VAO, VBO, EBO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1,&VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(VAO);

      //Crando il VBO e EBO e mandando i vertici e gli indici alla GPU
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      
   //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);
      
   //Impostanzione del Vao basato sul VBO
   //Attributi vertici
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
   glEnableVertexAttribArray(2);


   unsigned int lightVAO;
   glGenVertexArrays(1, &lightVAO);
   glBindVertexArray(lightVAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   stbi_set_flip_vertically_on_load(true);
   unsigned int texture1 = PMGL::load_texture("./resource/texture/container2.png");
   unsigned int specularTexture = PMGL::load_texture("./resource/texture/container2_specular.png");
   unsigned int emissionMap = PMGL::load_texture("./resource/texture/matrix.jpg");
   float shininess = 64;

   shad1.use();
   shad1.setInt("mat.diffuse", 0);
   shad1.setInt("mat.specular", 1);
   shad1.setInt("mat.emission", 2);
   shad1.setVec3("mat.u_color", glm::vec3(1.0f));
   shad1.setFloat("mat.shininess", shininess);
   
   PMGL::model backpack("./resource/objects/backpack/backpack.obj");

   glEnable(GL_DEPTH_TEST);

   PMGL::trasform cube[] = {
      PMGL::trasform(glm::vec3(0.0)),
      PMGL::trasform(glm::vec3(1.0, -2, 0.4)),
      PMGL::trasform(glm::vec3(5, 1, -9)),
      PMGL::trasform(glm::vec3(-6.0, 12, 5))
   };

   PMGL::directional_light directional = PMGL::directional_light(glm::vec3(-0.2f, -0.666f, 0.8f), PMGL::light_color(glm::vec3(1.0, 0.2,0.88), 0.5, 0.8, 0.2));
   
   std::vector<PMGL::point_light> point = {
      PMGL::point_light(glm::vec3(5, 4, -12), PMGL::light_color(glm::vec3(0.0, 0.5,0.77), 0.5, 0.8, 0.2), 0.02, 0.8, 0.45),
      PMGL::point_light(glm::vec3(-5.0, 11, 6), PMGL::light_color(glm::vec3(1.0, 0.6, 0.4), 0.5, 0.8, 0.2), 0.8, 0.8, 0.8),
   };

   PMGL::camera cam(glm::vec3(0.0f, 0.0f, 3.0f), aspect, 0.1, 100.0f, fov);
   PMGL::vector2d mouse = {0,0};

   //Main
   float lastFrame = 0;
   while(!win.should_close()){
      win.pollEvents();
      float currentFrame = glfwGetTime();
      mouse = mouse_update(win);
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;   
      //std::cout << (float)(1/deltaTime) << " FPS\n";
      glClearColor(0.5f, 0.5f, 0.5f, 0.1f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      PMGL::trasform tcam = cam.get_trasform();
      glm::quat qPitch = glm::angleAxis((float)-mouse.y * sensitivity, glm::vec3(1.0f, 0.0f, 0.0f));
      glm::quat qYaw = glm::angleAxis((float)-mouse.x * sensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
      mouse = {0,0};
      tcam.set_rotation(glm::normalize(qYaw *  tcam.get_rotation() * qPitch));
      cam.set_trasformation(tcam);
      

      for(auto l : point){
         glm::mat4 lightModel(1.0f);
         lightModel = glm::translate(lightModel, l.position);
         lightModel = glm::scale(lightModel, glm::vec3(0.2f));
         lightShader.use();
         lightShader.setMat4fv("model", lightModel);
         lightShader.setMat4fv("projection_view", cam.getViewProjectionMatrix());
         lightShader.setVec3("lightColor", l.color.color);
         glBindVertexArray(lightVAO);
         glDrawArrays(GL_TRIANGLES, 0, 36);
      }
      shad1.use();
      shad1.setFloat("time", glfwGetTime());
      shad1.setFloat("mix_var", abs(sin((glfwGetTime() / 0.5f) + 0.2f)));
      shad1.setVec3("view_pos", cam.get_trasform().get_position());
      shad1.setMat4fv("projection_view", cam.getViewProjectionMatrix());

      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
      shad1.setMat4fv("model", model);
      backpack.draw(shad1);

      PMGL::apply_light_shader(shad1, directional);
      PMGL::apply_light_vector_shader(shad1, point);

            
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, specularTexture);
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, emissionMap);
      
      
      
      

      
      glBindVertexArray(VAO);
      for(unsigned int i = 0; i < 4; i++){

         float angle = 20.0f * i;
         float current_angle = (float) glfwGetTime() * glm::radians(angle);
         glm::quat quat_rot = glm::angleAxis(current_angle, glm::normalize(glm::vec3(1.0f, 0.3, 0.5)));
         cube[i].set_rotation(quat_rot);
         
         glm::mat4 model = cube[i].get_model_matrix();
         glm::mat4 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model)));
         shad1.setMat4fv("model", model);
         shad1.setMat3f("normal_matrix", normal_matrix);

         glDrawArrays(GL_TRIANGLES, 0, 36);
      }
      win.end_frame();
   }

      //Terminate
   PMGL::terminate_PMGL();
   return 0;
}






PMGL::vector2d mouse_update(PMGL::window &w){
   const PMGL::Input& ih = w.getInput();
   PMGL::vector2d mouse = ih.mouseDelta();
   return mouse;
}

/*

void processInput(GLFWwindow *window){
   PMGL::trasform tcam = cam.get_trasform();
   float cameraSpeed = 2.5f * deltaTime;
   if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      tcam.translate((tcam.get_rotation() * WORLD_FRONT) * cameraSpeed);
   if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      tcam.translate(-(tcam.get_rotation() * WORLD_FRONT) * cameraSpeed);


   glm::vec3 camRight = tcam.get_rotation() * glm::vec3(1.0f, 0.0f, 0.0f);

   if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      tcam.translate(-(camRight * cameraSpeed));
   if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      tcam.translate((camRight * cameraSpeed));

   cam.set_trasformation(tcam);
}

void scroll_back(GLFWwindow *window, double xoffset, double yoffset){
   fov -= (float)yoffset;
   if(fov < 1.0f){
      fov = 1.0f;
   }
   if(fov > 45.0f){
      fov = 45.0f;
   }
   cam.set_fov(fov);
}

void framebuffer_size_callback(GLFWwindow *window, int w, int h){
   glViewport(0,0, w, h);
   aspect = (float)w / (float)h;
}


GLFWwindow *secureCreateWindow(int w, int h, const std::string name, GLFWmonitor *monitor, GLFWwindow *share){
   GLFWwindow *window = glfwCreateWindow(w, h, name.c_str(), monitor, share);
   if(window == NULL){
      std::cout << "Failed creation of GLFW Window\n";
      glfwTerminate();
      exit(-1);
   }
      
   return window;
}
*/