// $ g++ -o gladtest1 gladtest1.cpp -lglfw -lGL -lX11 -lpthread -lXrandr -lXxf86vm -lXinerama -lXcursor -lXi -ldl -lglad

// $ g++ -o gladtest1 gladtest1.cpp -lglfw -lGL -lglad

#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
#include <stdio.h>
 
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
 
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		//将窗口设置为关闭，跳出循环
		glfwSetWindowShouldClose(window, true);
	}
}
 
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
 
int main()
{
	//glfw初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
	//glfw创建窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		printf("创建窗口失败");
		//终止
		glfwTerminate();
		return -1;
	}
	//显示窗口
	glfwMakeContextCurrent(window);
 
	//设置回调，当窗口大小调整后将调用该回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
	// glad初始化
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("加载失败");
		return -1;
	}
 
	// 使用循环达到循环渲染效果
	while (!glfwWindowShouldClose(window))
	{
		//自定义输入事件
		processInput(window);
		//交互缓冲区，否则显示空白
		glfwSwapBuffers(window);
		//输入输出事件,否则无法对窗口进行交互
		glfwPollEvents();
	}
 
	//终止渲染 关闭并清理glfw本地资源
	glfwTerminate();
	return 0;
}
 
