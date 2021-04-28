#include "Graphics\window.h"
#include "Camera\camera.h"
#include "Shaders\shader.h"
#include "Model Loading\mesh.h"
#include "Model Loading\texture.h"
#include "Model Loading\meshLoaderObj.h"
#include"Windows.h"
#include"MMSystem.h"

#define PI 3.14159265358979323846  


void processKeyboardInput ();

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float C_Ai = 5.0f; //2.5f;							//amplitude of the wave
float D_Ai = 5.0f; //2.5f;							//amplitude of the wave

glm::vec2 D_Di = glm::vec2(0.5f,0.5f);
glm::vec2 F_Di = glm::vec2(0.3f, 0.8f);

float C_L = 17.0f; //8.5f;							//wave length
float D_L = 17.0f; //8.5f;							//wave length

float C_Wi = (2 * PI) / C_L;				// w = 2*pi/L
float D_Wi = (2 * PI) / D_L;				// w = 2*pi/L

float C_S = 2 * PI; //PI/1;						    //wave speed
float D_S = 2 * PI; //PI/1;						    //wave speed

float C_phase = C_S * ((2 * PI) / C_L);
float D_phase = D_S * ((2 * PI) / D_L);

float time;

bool flagT = 0;
float elapsedTime = 0;

glm::vec2 Ci = glm::vec2(0.0f, 0.0f);

// y(x,z,t) = 2*ai*(((sin(Di*(x,z)*wi + t *phase) + 1)/2)^2)


Window window("Ocean Man", 800, 800);
Camera camera;

glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPos = glm::vec3(-280.0f, 100.0f, 0.0f);



int main()
{	
	
	glClearColor(0.2f, 0.8f, 1.0f, 1.0f);
			
	
		
	//building and compiling shader program
	Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
	Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");
	Shader waterDirShader("Shaders/water_directional_vertex.glsl", "Shaders/water_fragment.glsl");
	Shader waterCircShader("Shaders/water_circular_vertex.glsl", "Shaders/water_fragment.glsl");
	Shader waterDirCircShader("Shaders/water_DirCirc_vertex.glsl", "Shaders/water_fragment.glsl");
	Shader waterThreeShader("Shaders/water_three_vertex.glsl", "Shaders/water_fragment.glsl");
	Shader waterFourShader("Shaders/water_four_vertex.glsl", "Shaders/water_fragment.glsl");
	Shader rockShader("Shaders/rock_vertex.glsl", "Shaders/water_fragment.glsl");


	//Textures
	GLuint tex = loadBMP("Resources/Textures/wood.bmp");
	GLuint tex2 = loadBMP("Resources/Textures/rock.bmp");
	GLuint tex3 = loadBMP("Resources/Textures/sun.bmp");
	GLuint tex4 = loadBMP("Resources/Textures/water.bmp");
	GLuint tex5 = loadBMP("Resources/Textures/ship.bmp");
	GLuint tex6 = loadBMP("Resources/Textures/ship_sail.bmp");


	glEnable(GL_DEPTH_TEST);

	//Test custom mesh loading
	std::vector<Vertex> vert;
	vert.push_back(Vertex());
	vert[0].pos = glm::vec3(10.5f, 10.5f, 0.0f);
	vert[0].textureCoords = glm::vec2(1.0f, 1.0f);

	vert.push_back(Vertex());
	vert[1].pos = glm::vec3(10.5f, -10.5f, 0.0f);
	vert[1].textureCoords = glm::vec2(1.0f, 0.0f);

	vert.push_back(Vertex());
	vert[2].pos = glm::vec3(-10.5f, -10.5f, 0.0f);
	vert[2].textureCoords = glm::vec2(0.0f, 0.0f);

	vert.push_back(Vertex());
	vert[3].pos = glm::vec3(-10.5f, 10.5f, 0.0f);
	vert[3].textureCoords = glm::vec2(0.0f, 1.0f);

	vert[0].normals = glm::normalize(glm::cross(vert[1].pos - vert[0].pos, vert[3].pos - vert[0].pos));
	vert[1].normals = glm::normalize(glm::cross(vert[2].pos - vert[1].pos, vert[0].pos - vert[1].pos));
	vert[2].normals = glm::normalize(glm::cross(vert[3].pos - vert[2].pos, vert[1].pos - vert[2].pos));
	vert[3].normals = glm::normalize(glm::cross(vert[0].pos - vert[3].pos, vert[2].pos - vert[3].pos));

	std::vector<int> ind = { 0, 1, 3,   
		1, 2, 3 };

	std::vector<Texture> textures;
	textures.push_back(Texture());
	textures[0].id = tex;
	textures[0].type = "texture_diffuse";

	std::vector<Texture> textures2;
	textures2.push_back(Texture());
	textures2[0].id = tex2;
	textures2[0].type = "texture_diffuse";

	std::vector<Texture> textures3;
	textures3.push_back(Texture());
	textures3[0].id = tex3;
	textures3[0].type = "texture_diffuse";

	std::vector<Texture> textures4;
	textures4.push_back(Texture());
	textures4[0].id = tex4;
	textures4[0].type = "texture_diffuse";

	std::vector<Texture> textures5;
	textures5.push_back(Texture());
	textures5[0].id = tex5;
	textures5[0].type = "texture_diffuse";

	std::vector<Texture> textures6;
	textures6.push_back(Texture());
	textures6[0].id = tex6;
	textures6[0].type = "texture_diffuse";


	//Mesh mesh(vert, ind, textures3);

	// Create Obj files - easier :)
	// we can add here our textures :)
	MeshLoaderObj loader;
	Mesh sun = loader.loadObj("Resources/Models/sphere.obj");
	Mesh plane = loader.loadObj("Resources/Models/grid.obj", textures4);
	Mesh ship = loader.loadObj("Resources/Models/ship.obj", textures6);
	Mesh rock = loader.loadObj("Resources/Models/rock2.obj", textures2);
	Mesh rock2 = loader.loadObj("Resources/Models/rock2.obj", textures2);
	Mesh rock3 = loader.loadObj("Resources/Models/rock2.obj", textures2);
	Mesh boat = loader.loadObj("Resources/Models/boat.obj", textures);

	PlaySound("audio/ambient.wav", NULL, SND_ASYNC | SND_ALIAS);

	//check if we close the window or press the escape button
	while (!window.isPressed(GLFW_KEY_ESCAPE) &&
		glfwWindowShouldClose(window.getWindow()) == 0)
	{
		window.clear();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		

		processKeyboardInput();

		//test mouse input
		if (window.isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			std::cout << "Pressing mouse button" << std::endl;
		}

		time = glfwGetTime();

		 //// Code for the light ////

		sunShader.use();

		glm::mat4 ProjectionMatrix = glm::perspective(90.0f, window.getWidth() * 1.0f / window.getHeight(), 0.1f, 10000.0f);
		glm::mat4 ViewMatrix = glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());

		GLuint MatrixID = glGetUniformLocation(sunShader.getId(), "MVP");

		//Test for one Obj loading = light source

		glm::mat4 ModelMatrix = glm::mat4(1.0);
		//ModelMatrix = glm::translate(ModelMatrix, lightPos);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f,100.0f,0.0f));
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		
		if (flagT) {
			elapsedTime += deltaTime;
		}


		ModelMatrix = glm::rotate(ModelMatrix, elapsedTime * 40.0f , glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-280.0f, 0.0f, 0.0f));

		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		

		sun.draw(sunShader);

		//// End code for the light ////

		shader.use();

		///// Test Obj files for box ////

		GLuint MatrixID2 = glGetUniformLocation(shader.getId(), "MVP");
		GLuint ModelMatrixID = glGetUniformLocation(shader.getId(), "model");

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -20.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
		
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		ship.draw(shader);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(60.0f, -15.0f, -90.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(6.1f, 6.1f, 6.1f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		boat.draw(shader);

		rockShader.use();

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(120.0f, -20.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(7.1f, 7.1f, 7.1f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(rockShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(rockShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniform3f(glGetUniformLocation(rockShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(rockShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(rockShader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		rock.draw(rockShader);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-120.0f, -20.0f, -70.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(7.1f, 7.1f, 7.1f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(rockShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(rockShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniform3f(glGetUniformLocation(rockShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(rockShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(rockShader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		rock3.draw(rockShader);
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-105.0f, -20.0f, 70.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(7.1f, 7.1f, 7.1f));
		ModelMatrix = glm::rotate(ModelMatrix,20.0f, glm::vec3(3.1f, 0.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(rockShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(rockShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniform3f(glGetUniformLocation(rockShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(rockShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(rockShader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		rock3.draw(rockShader);


		if (flag == 0) {

			
			waterDirShader.use();

			///// Test plane Obj file //////
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -20.0f, 0.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(waterDirShader.getId(),"MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(waterDirShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);

			glUniform2f(glGetUniformLocation(waterDirShader.getId(), "D_Di"),D_Di.x,D_Di.y);
			glUniform1f(glGetUniformLocation(waterDirShader.getId(), "D_Ai"),D_Ai);
			glUniform1f(glGetUniformLocation(waterDirShader.getId(), "D_Wi"), D_Wi);
			glUniform1f(glGetUniformLocation(waterDirShader.getId(), "time"), time);
			glUniform1f(glGetUniformLocation(waterDirShader.getId(), "D_phase"), D_phase);

			glUniform3f(glGetUniformLocation(waterDirShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(waterDirShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(waterDirShader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

			//l2
			plane.draw(waterDirShader);

		} else if(flag == 1){



			waterCircShader.use();

			///// Test plane Obj file //////
			

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -20.0f, 0.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(waterCircShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(waterCircShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);

			glUniform2f(glGetUniformLocation(waterCircShader.getId(), "Ci"), Ci.x, Ci.y);
			glUniform1f(glGetUniformLocation(waterCircShader.getId(), "C_Ai"), C_Ai);
			glUniform1f(glGetUniformLocation(waterCircShader.getId(), "C_Wi"), C_Wi);
			glUniform1f(glGetUniformLocation(waterCircShader.getId(), "time"), time);
			glUniform1f(glGetUniformLocation(waterCircShader.getId(), "C_phase"), C_phase);

			glUniform3f(glGetUniformLocation(waterCircShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(waterCircShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(waterCircShader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

			//l2
			plane.draw(waterCircShader);
		} else if(flag == 2){
		


			waterDirCircShader.use();

			///// Test plane Obj file //////
			

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -20.0f, 0.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(waterDirCircShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(waterDirCircShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);

			glUniform2f(glGetUniformLocation(waterDirCircShader.getId(), "Ci"), Ci.x, Ci.y);
			glUniform1f(glGetUniformLocation(waterDirCircShader.getId(), "C_Ai"), C_Ai);
			glUniform1f(glGetUniformLocation(waterDirCircShader.getId(), "C_Wi"), C_Wi);
			glUniform1f(glGetUniformLocation(waterDirCircShader.getId(), "time"), time);
			glUniform1f(glGetUniformLocation(waterDirCircShader.getId(), "C_phase"), C_phase);
			glUniform2f(glGetUniformLocation(waterDirCircShader.getId(), "D_Di"), D_Di.x, D_Di.y);
			glUniform1f(glGetUniformLocation(waterDirCircShader.getId(), "D_Ai"), D_Ai);
			glUniform1f(glGetUniformLocation(waterDirCircShader.getId(), "D_Wi"), D_Wi);
			glUniform1f(glGetUniformLocation(waterDirCircShader.getId(), "D_phase"), D_phase);

			glUniform3f(glGetUniformLocation(waterDirCircShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(waterDirCircShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(waterDirCircShader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

			//l2
			plane.draw(waterDirCircShader);

		}
		else if (flag == 3) {

			waterThreeShader.use();

			///// Test plane Obj file //////


			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -20.0f, 0.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(waterThreeShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(waterThreeShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);

			glUniform2f(glGetUniformLocation(waterThreeShader.getId(), "Ci"), Ci.x, Ci.y);
			glUniform1f(glGetUniformLocation(waterThreeShader.getId(), "C_Ai"), C_Ai);
			glUniform1f(glGetUniformLocation(waterThreeShader.getId(), "C_Wi"), C_Wi);
			glUniform1f(glGetUniformLocation(waterThreeShader.getId(), "time"), time);
			glUniform1f(glGetUniformLocation(waterThreeShader.getId(), "C_phase"), C_phase);
			glUniform2f(glGetUniformLocation(waterThreeShader.getId(), "D_Di"), D_Di.x, D_Di.y);
			glUniform1f(glGetUniformLocation(waterThreeShader.getId(), "D_Ai"), D_Ai);
			glUniform1f(glGetUniformLocation(waterThreeShader.getId(), "D_Wi"), D_Wi);
			glUniform1f(glGetUniformLocation(waterThreeShader.getId(), "D_phase"), D_phase);

			glUniform3f(glGetUniformLocation(waterThreeShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(waterThreeShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(waterThreeShader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

			//l2
			plane.draw(waterThreeShader);

		}

		else if (flag == 4) {

		waterFourShader.use();

		///// Test plane Obj file //////


		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -20.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(waterFourShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(waterFourShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);

		glUniform2f(glGetUniformLocation(waterFourShader.getId(), "Ci"), Ci.x, Ci.y);
		glUniform1f(glGetUniformLocation(waterFourShader.getId(), "C_Ai"), C_Ai);
		glUniform1f(glGetUniformLocation(waterFourShader.getId(), "C_Wi"), C_Wi);
		glUniform1f(glGetUniformLocation(waterFourShader.getId(), "time"), time);
		glUniform1f(glGetUniformLocation(waterFourShader.getId(), "C_phase"), C_phase);
		glUniform2f(glGetUniformLocation(waterFourShader.getId(), "D_Di"), D_Di.x, D_Di.y);
		glUniform1f(glGetUniformLocation(waterFourShader.getId(), "D_Ai"), D_Ai);
		glUniform1f(glGetUniformLocation(waterFourShader.getId(), "D_Wi"), D_Wi);
		glUniform1f(glGetUniformLocation(waterFourShader.getId(), "D_phase"), D_phase);
		glUniform2f(glGetUniformLocation(waterFourShader.getId(), "F_Di"), F_Di.x, F_Di.y);

		glUniform3f(glGetUniformLocation(waterFourShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(waterFourShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(waterFourShader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//l2
		plane.draw(waterFourShader);

		}

		

		window.update();
		
	}
	
}

void processKeyboardInput()
{
	float cameraSpeed = 30 * deltaTime;

	//translation
	if (window.isPressed(GLFW_KEY_TAB))
		flagT = !flagT;
	if (window.isPressed(GLFW_KEY_W))
		camera.keyboardMoveFront(cameraSpeed);
	if (window.isPressed(GLFW_KEY_S))
		camera.keyboardMoveBack(cameraSpeed);
	if (window.isPressed(GLFW_KEY_A))
		camera.keyboardMoveLeft(cameraSpeed);
	if (window.isPressed(GLFW_KEY_D))
		camera.keyboardMoveRight(cameraSpeed);
	if (window.isPressed(GLFW_KEY_R))
		camera.keyboardMoveUp(cameraSpeed);
	if (window.isPressed(GLFW_KEY_F))
		camera.keyboardMoveDown(cameraSpeed);

	//rotation
	if (window.isPressed(GLFW_KEY_LEFT))
		camera.rotateOy(cameraSpeed);
	if (window.isPressed(GLFW_KEY_RIGHT))
		camera.rotateOy(-cameraSpeed);
	if (window.isPressed(GLFW_KEY_UP))
		camera.rotateOx(cameraSpeed);
	if (window.isPressed(GLFW_KEY_DOWN))
		camera.rotateOx(-cameraSpeed);
}
