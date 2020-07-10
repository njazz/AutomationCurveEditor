// dear imgui: standalone example application for GLFW + OpenGL2, using legacy fixed pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the example_glfw_opengl2/ folder**
// See imgui_impl_glfw.cpp for details.

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>

#include "AutomationCurveWidget.hpp"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Test Window", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //

    float scroll = 0;
    float zoom = 1;

    //
    auto c1 = AutomationCurve::ACurve::CreatePtr();
    c1->InitConstant(.5);
    c1->AddPoint(.5, .75);

    AutomationCurve::MultiCurve mc;

    mc.curves["One"] = c1;
    auto c2 = AutomationCurve::ACurve::CreatePtr();
    *c2=*c1;
    c2->AddPoint(.25, 0);
    mc.curves["Two"] = c2; //AutomationCurve::ACurve();

    mc.SetColor("One", AutomationCurve::MultiCurve::CurveColor(255, 64, 0));
    mc.SetColor("Two", AutomationCurve::MultiCurve::CurveColor(0, 64, 255));
    
    AutomationCurve::CurveEditor editor(c1);

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //

//        ImGui::Begin("Widget");
//
//        ImGui::SetNextItemWidth(150);
//        ImGui::SliderFloat("Scroll", &scroll, 0, 1);
//        ImGui::SameLine();
//        ImGui::SetNextItemWidth(150);
//        ImGui::SliderFloat("Zoom", &zoom, 0, 1);
//
//        AutomationCurve::ImWidgetOverview("Test widget Overview", ImVec2(800, 96), c1, &scroll, &zoom);
//        AutomationCurve::ImWidget("Test widget", ImVec2(800, 480), c1, scroll, zoom);
//
//        AutomationCurve::ImWidgetListView("View", c1);
//
//        ImGui::End();

        //

        ImGui::Begin("Multi");

        AutomationCurve::ImWidgetOverviewMulti("Test widget Overview", ImVec2(800, 96), mc, &scroll, &zoom);
        AutomationCurve::ImWidgetMulti("Multi", ImVec2(800, 480), mc, scroll, zoom);

        ImGui::End();
        
        ImGui::Begin("Select");
        for (auto& e: mc.curves){
            if (ImGui::Button(e.first.c_str()))
                mc.SetActiveCurveName(e.first);//activeCurve = e.first;
            ImGui::SameLine();
            float col[3];
            col[0] = mc.GetColor(e.first).r/255.;
            col[1] = mc.GetColor(e.first).g/255.;
            col[2] = mc.GetColor(e.first).b/255.;
            if(ImGui::ColorEdit3(("color##"+std::to_string((intptr_t)&e)).c_str(), col))
            {
                AutomationCurve::MultiCurve::CurveColor col2;
                col2.r = col[0]*255;
                col2.g = col[1]*255;
                col2.b = col[2]*255;
                mc.SetColor(e.first, col2);
            }
        }
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
