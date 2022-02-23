//
// Created by captainchen on 2022/2/7.
//

#ifndef UNTITLED_RENDERER_H
#define UNTITLED_RENDERER_H

#include <thread>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <spscqueue/include/rigtorp/SPSCQueue.h>
#include <glm/glm.hpp>

/// 渲染命令
enum RenderCommand {
    COMPILE_SHADER,//编译着色器
    DRAW_ARRAY,//绘制
};

/// 渲染任务基类
class RenderTaskBase{
public:
    RenderTaskBase(){}
    virtual ~RenderTaskBase(){}
public:
    RenderCommand render_command_;//渲染命令
    bool need_return_result = false;//是否需要返回结果
};

/// 需要结果的阻塞性任务
class RenderTaskNeedReturnResult: public RenderTaskBase{
public:
    RenderTaskNeedReturnResult(){need_return_result=true;}
    ~RenderTaskNeedReturnResult(){}
};

/// 编译着色器任务
class RenderTaskCompileShader: public RenderTaskNeedReturnResult{
public:
    RenderTaskCompileShader(){}
    ~RenderTaskCompileShader(){}
public:
    const char* vertex_shader_source_= nullptr;
    const char* fragment_shader_source_= nullptr;
public:
    GLuint result_program_id_=0;//存储编译Shader结果的ProgramID
};

/// 绘制任务
class RenderTaskDrawArray: public RenderTaskBase {
public:
    RenderTaskDrawArray(){}
    ~RenderTaskDrawArray(){}
public:
    GLuint program_id_=0;//着色器ProgramID
    const void* positions_=nullptr;//顶点位置
    GLsizei   positions_stride_=0;//顶点数据大小
    const void* colors_=nullptr;//顶点颜色
    GLsizei   colors_stride_=0;//颜色数据大小
};


class GLFWwindow;
class Renderer {
public:
    Renderer(GLFWwindow* window);
    ~Renderer();

    void PushRenderTask(RenderTaskBase* render_task){
        render_task_queue_.push(render_task);
    }

    /// 渲染任务队列是否空，用来判定这一帧渲染完成.
    bool IsQueueEmpty(){
        return render_task_queue_.empty();
    }
private:
    void RenderMain();
private:
    GLFWwindow* window_;
    std::thread render_thread_;//渲染线程
    rigtorp::SPSCQueue<RenderTaskBase*> render_task_queue_;//渲染任务队列
};



#endif //UNTITLED_RENDERER_H