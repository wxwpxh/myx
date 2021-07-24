// g++ -o glewtest2 glewtest2.cpp `pkg-config --cflags --static --libs glew glfw3`
#include <string>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream> 
using namespace std;
 
int main(void)
{
 
	// 初始化GLFW  
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
 
	//GLFW
	glfwInit();
 
	//限制了OpenGL为3.3版本导致一些新特性无法使用
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);   //不可改变大小  
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);   //没有边框和标题栏  
 
	//创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
 
	//GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	
	/*循环绘制 */
	while (!glfwWindowShouldClose(window))
	{
		//清屏背景
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
 
		glfwSwapBuffers(window);
 
		/* Poll for and process events */
		glfwPollEvents();
	}
 
	glfwTerminate();
	return 0;
}
