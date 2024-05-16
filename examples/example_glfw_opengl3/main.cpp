// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <Windows.h>

#include "graph_matrix_6_functions.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

class circle_coordinates {
public:
    int x;
    int y;
};

class line_coordinates {
public:
    float x_1;
    float y_1;
    float x_2;
    float y_2;
    //could perhaps put the coordinates for the end points of the 2 arrows here too for directed graph displaying...
};

bool mouse_in_circle(circle_coordinates cir, float circle_radius, float mouse_x, float mouse_y) {
    if (mouse_x >= cir.x - circle_radius && mouse_x <= cir.x + circle_radius && mouse_y >= cir.y - circle_radius && mouse_y <= cir.y + circle_radius) {
        return true;
    }

    return false;
}

//2D vector for constructing adjacency matrix that you will eventually send to the graph algorithms when done constructing the graph
//1 if an edge from node_1->node_2 0 if not
//draw arrows not just lines to represent the direction of the edge 
std::vector < std::vector <int> > adj_matrix_under_construction;

//int main(int, char**)
int WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd
)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif


    // Create window with graphics context
    //GLFWwindow* window = glfwCreateWindow(1280, 720, "Graph Traversal Sequencer", NULL, NULL);
    GLFWwindow* window = glfwCreateWindow(1280, 800, "Graph Traversal Sequencer", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //various graph function mode booleans
    bool add_node_mode = false;
    bool add_edge_mode = false;
    bool ready_for_line_end_coordinate = false;
    bool have_both_line_coordinates = false;
    bool remove_node_mode = false;
    bool remove_edge_mode = false;
    bool no_draw_mode = true; //beginning default is no draw
    //for drawing line edge connections
    ImVec2 line_start;
    ImVec2 line_end;
    int line_start_node_index = -1;
    int line_end_node_index = -1;
    std::vector <circle_coordinates> circles; //COULD CHANGE NAME TO nodes, leaving as circles for now
    std::vector <line_coordinates> lines; //edges
    float circle_radius = 20;
    int start_node = 0;

    //option to draw all edges in both directions without having to specify 2 edges
    bool undirected_graph = false;

    //vector to store corresponding midi notes in
    std::vector <int> midi_notes;

    //vector of IM_COL32 colors to rotate through for coordinating node and midi note label colors
    std::vector < ImU32 > circle_midi_note_colors;
    const ImU32 red_color = IM_COL32(255, 0, 0, 255);
    circle_midi_note_colors.push_back(red_color);
    const ImU32 blue_color = IM_COL32(0, 255, 0, 255);
    circle_midi_note_colors.push_back(blue_color);
    const ImU32 green_color = IM_COL32(0, 0, 255, 255);
    circle_midi_note_colors.push_back(green_color);
    const ImU32 yellow_color = IM_COL32(255, 255, 0, 255);
    circle_midi_note_colors.push_back(yellow_color);
    const ImU32 purple_color = IM_COL32(128, 0, 128, 255);
    circle_midi_note_colors.push_back(purple_color);
    const ImU32 orange_color = IM_COL32(255, 165, 0, 255);
    circle_midi_note_colors.push_back(orange_color);

    //all possible paths through graph holders
    std::string all_possible_paths_output_str = "";
    std::vector <std::string> all_output_strings;
    std::vector <std::string> all_possible_paths_strings;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //fit next window size to the main application window size
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        //ImGui::Begin("Graph Traversal Sequencer", nullptr, ImGuiWindowFlags_NoMove);
        ImGui::Begin("Graph Traversal Sequencer", nullptr); 

        //*****************combo box code below**************************
        // Expose flags as checkbox for the demo
        static ImGuiComboFlags flags = 0;
        // Using the generic BeginCombo() API, you have full control over how to display the combo contents.
        // (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
        // stored in the object itself, etc.)
        //combo box item names for selecting various graph functions 
        const char* items[] = { "NO DRAW", "ADD NODE", "ADD EDGE", "REMOVE NODE", "REMOVE EDGE" };
        static int item_current_idx = 0; // Here we store our selection data as an index.
        const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
        ImGui::PushItemWidth(300);
        if (ImGui::BeginCombo("GRAPH BUILDING OPTIONS", combo_preview_value, flags))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(items[n], is_selected))
                    item_current_idx = n;
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected) { //COMBO MENU SELECTED
                    ImGui::SetItemDefaultFocus();
                    add_node_mode = false;
                    add_edge_mode = false;
                    remove_node_mode = false;
                    remove_edge_mode = false;
                    no_draw_mode = true;
                }
            }

            if (item_current_idx == 0) { //NO DRAW
                add_node_mode = false;
                add_edge_mode = false;
                remove_node_mode = false;
                remove_edge_mode = false;
                no_draw_mode = true;
            }
            else if (item_current_idx == 1) { //ADD NODE
                add_node_mode = true;
                add_edge_mode = false;
                remove_node_mode = false;
                remove_edge_mode = false;
                no_draw_mode = false;
            }
            else if (item_current_idx == 2) { //ADD EDGE
                add_node_mode = false;
                add_edge_mode = true;
                remove_node_mode = false;
                remove_edge_mode = false;
                no_draw_mode = false;
            }
            else if (item_current_idx == 3) { //REMOVE NODE
                add_node_mode = false;
                add_edge_mode = false;
                remove_node_mode = true;
                remove_edge_mode = false;
                no_draw_mode = false;
            }
            else if (item_current_idx == 4) { //REMOVE EDGE
                add_node_mode = false;
                add_edge_mode = false;
                remove_node_mode = false;
                remove_edge_mode = true;
                no_draw_mode = false;
            }

            ImGui::EndCombo();
        }
        //*****************combo box code above**************************

        //****************directed/undirected combo box below********************
        //directed or undirected graph menu
        const char* items_2[] = { "DIRECTED GRAPH", "UNDIRECTED GRAPH" };
        static int item_current_idx_2 = 0; // Here we store our selection data as an index.
        const char* combo_preview_value_2 = items_2[item_current_idx_2];  // Pass in the preview value visible before opening the combo (it could be anything)
        if (ImGui::BeginCombo("DIRECTED/UNDIRECTED GRAPH", combo_preview_value_2, flags))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items_2); n++)
            {
                const bool is_selected = (item_current_idx_2 == n);
                if (ImGui::Selectable(items_2[n], is_selected))
                    item_current_idx_2 = n;
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected) { //COMBO MENU SELECTED
                    ImGui::SetItemDefaultFocus();
                    undirected_graph = false;
                }
            }

            if (item_current_idx_2 == 0 && undirected_graph) { //DIRECTED
                undirected_graph = false;
            }
            else if (item_current_idx_2 == 1 && !undirected_graph) { //UNDIRECTED
                undirected_graph = true;
            }

            ImGui::EndCombo();
        }

        //****************directed/undirected combo box above********************

        //start node input box
        static char buf2[64] = "0"; ImGui::InputText("START NODE", buf2, 64, ImGuiInputTextFlags_CharsDecimal);
        //get start node to generate all possible paths from from user input
        start_node = atoi(buf2);
        //check that start node entered is a valid node, if not valid, set to closest valid value
        if (start_node < 0) {
            start_node = 0;
            buf2[0] = '0';
            buf2[1] = '\0';
        }
        else if (start_node > circles.size() - 1) {
            start_node = circles.size() - 1;
            std::ostringstream temp;
            temp << circles.size() - 1;
            std::string max_node_val = temp.str();
            for (int i = 0; i < max_node_val.size(); i++) {
                buf2[i] = max_node_val[i];
            }
            buf2[max_node_val.size()] = '\0';
        }

        //midi note input boxes
        for (int i = 0; i < adj_matrix_under_construction.size(); i++) {
            if (i >= midi_notes.size()) {
                midi_notes.push_back(i);
            }
            ImGui::PushID(i);
            //looping through 6 colors that correspond to the midi note input box colors
            //coordinated with the circle node colors
            ImGui::PushStyleColor(ImGuiCol_Text, circle_midi_note_colors[i % circle_midi_note_colors.size()]); 
            std::string string_label = "midi note for node ";
            std::ostringstream temp;
            temp << i;
            string_label = string_label + temp.str();
            ImGui::InputInt(string_label.c_str(), &midi_notes[i]);
            ImGui::PopStyleColor(); //setting color back to default white
            ImGui::PopID();
        }

        ImGui::Text("\n\n");

        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 255, 0, 255));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
        if (ImGui::Button("GENERATE \nSEQUENCE") && circles.size() > 0) { //check if there no node drawn, then don't generate sequence
            all_possible_paths_strings.clear(); //reset if generate sequence is clicked again 
            all_output_strings.clear(); //reset if generate sequence is clicked again

            graphPT g = NULL;
            destroyGraph(g);

            if (undirected_graph) {
                g = newGraph(adj_matrix_under_construction.size(), 1);
            }
            else {
                g = newGraph(adj_matrix_under_construction.size(), 0);
            }

            //mapped_vals parameter will be midi_notes
            //midi_notes are by default the node #

            //add edges to graph
            for (int i = 0; i < adj_matrix_under_construction.size(); i++) {
                for (int j = 0; j < adj_matrix_under_construction[0].size(); j++) {
                    if (adj_matrix_under_construction[i][j] == 1) {
                        addEdge(g, i, j);
                    }
                }
            }

            //generate all possible paths
            //pass all_possible_paths_strings to function call to hold all of the paths
            print_all_possible_paths_outer(g, start_node, midi_notes, 0, all_possible_paths_strings); //added 0 for don't print hamiltonians_instead
        }
        ImGui::PopStyleColor(2);

        ImGui::Text("\n\n");

        ImGui::Text("# of possible paths: %d", all_possible_paths_strings.size());
        std::string all_possible_paths_as_one_string = "";
        for (int i = 0; i < all_possible_paths_strings.size(); i++) {
            std::string path_label = "PATH ";
            std::ostringstream temp;
            temp << i + 1;
            path_label = path_label + temp.str();
            char one_possible_path_str_as_char_arr[32768]; //around 6,500 possible entries for 1 possible path
            strcpy(one_possible_path_str_as_char_arr, all_possible_paths_strings[i].c_str());
            ImGui::InputText(path_label.c_str(), one_possible_path_str_as_char_arr, 32768, ImGuiInputTextFlags_ReadOnly);
            all_possible_paths_as_one_string = all_possible_paths_as_one_string + all_possible_paths_strings[i];
        }
        //count the # of commas at this point to find how many notes are in ALL POSSIBLE PATHS ALTOGETHER sequence to display
        int num_notes = std::count(all_possible_paths_as_one_string.begin(), all_possible_paths_as_one_string.end(), ',');
        ImGui::Text("# of notes in all possible paths altogether: %d", num_notes);

        //get factors of the # of notes
        std::vector <int> factors_of_num_notes;
        get_factors(num_notes, factors_of_num_notes);
        std::string factors_of_num_notes_str = "factors of # of notes: ";
        for (int i = 0; i < factors_of_num_notes.size(); i++) {
            std::ostringstream temp;
            temp << factors_of_num_notes[i];
            if (i == factors_of_num_notes.size() - 1) {
                factors_of_num_notes_str = factors_of_num_notes_str + temp.str();
            }
            else {
                factors_of_num_notes_str = factors_of_num_notes_str + temp.str() + ", ";
            }
        }
        char factors_of_num_notes_str_as_char_arr[65536];
        strcpy(factors_of_num_notes_str_as_char_arr, factors_of_num_notes_str.c_str());
        ImGui::Text(factors_of_num_notes_str_as_char_arr);

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
        ImGui::Text("************************************************************************");

        //remove last comma
        all_possible_paths_as_one_string = all_possible_paths_as_one_string.substr(0, all_possible_paths_as_one_string.size() - 2);

        char all_possible_paths_as_one_string_as_char_arr[262144]; //around 52,000 note entries in sequence possible with 262,144 memory
        strcpy(all_possible_paths_as_one_string_as_char_arr, all_possible_paths_as_one_string.c_str());
        ImGui::InputText("ALL POSSIBLE PATHS ALTOGETHER", all_possible_paths_as_one_string_as_char_arr, 262144, ImGuiInputTextFlags_ReadOnly);
        all_possible_paths_as_one_string = "";

        ImGui::Text("************************************************************************");

        ImGui::PopStyleColor();

        auto draw = ImGui::GetWindowDrawList(); //get reference to IMGUI's draw list to add things to draw to it
        if (add_node_mode) {
            draw->AddCircle(ImVec2(io.MousePos.x, io.MousePos.y), circle_radius, IM_COL32(255, 0, 0, 255), 100, 1.0f);
        }

        ImGui::Text("# of nodes: %d", circles.size());
        ImVec2 screen_size = io.DisplaySize;

        //fit next window size to the main application window size
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        //display adjacency matrix for user
        ImGui::PushStyleColor(ImGuiCol_Text, yellow_color);
        ImGui::Text("Adjacency Matrix:");
        for (int i = 0; i < adj_matrix_under_construction.size(); i++) {
            std::string one_row = "";
            for (int j = 0; j < adj_matrix_under_construction[0].size(); j++) {
                std::ostringstream temp;
                temp << adj_matrix_under_construction[i][j];
                std::string adj_matrix_entry_to_add = temp.str();
                one_row = one_row + adj_matrix_entry_to_add + ", ";
            }
            const char* c_string_one_row = one_row.c_str();
            ImGui::Text(c_string_one_row);
        }
        ImGui::PopStyleColor();

        //add circle node with mouse click
        if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered() && add_node_mode) {
            //CHECK IF THERE IS ALREADY A NODE CIRCLE AT THESE COORDINATES INCLUDING RADIUS
            bool already_a_node_in_this_space = false;
            for (int i = 0; i < circles.size(); i++) {
                //circle_radius * 4 seems good for not allowing 2 node circles to be too close to each other
                if (mouse_in_circle(circles[i], circle_radius * 4, io.MousePos.x, io.MousePos.y)) {
                    already_a_node_in_this_space = true;
                }
            }

            if (!already_a_node_in_this_space) {
                circle_coordinates cir; //could use an ImVec2 instead of making a circle_coordinates class...
                cir.x = io.MousePos.x;
                cir.y = io.MousePos.y;
                circles.push_back(cir);
                //add node to adjacency matrix that you are building
                //circles.size() - 1 is the current index
                //so to add a node just push back '0's in all of the new spots of the 2D vector
                std::vector <int> new_row;
                for (int i = 0; i < circles.size(); i++) { //fill new row that you are adding with as many '0's as there are circles in the circles vector
                    new_row.push_back(0);
                }
                //then push back an extra '0' for each row that already exists
                //to extend each of those pre-existing rows to include the new node
                //no connecting edges to this new node at the moment because just added it, so '0'
                for (int j = 0; j < adj_matrix_under_construction.size(); j++) {
                    adj_matrix_under_construction[j].push_back(0);
                }

                adj_matrix_under_construction.push_back(new_row); //an entire row I guess we'll call it
            }
        }
        else if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered() && remove_node_mode) {
            bool mouse_is_touching_a_circle = false; 

            int i = 0;
            while (i < circles.size()) {
                if (mouse_in_circle(circles[i], circle_radius, io.MousePos.x, io.MousePos.y)) {
                    int j = 0;
                    while (j < lines.size()) {
                        //can use mouse_in_circle function even though no mouse is involved thinking about it here
                        //check if either end of an edge line is in the node circle that you are about to remove
                        if (mouse_in_circle(circles[i], circle_radius, lines[j].x_1, lines[j].y_1) || mouse_in_circle(circles[i], circle_radius, lines[j].x_2, lines[j].y_2)) {
                            lines.erase(lines.begin() + j);
                        }
                        else {
                            j++;
                        }
                    }
                    j = 0;

                    circles.erase(circles.begin() + i);
                    //remove midi note associated with the node circle
                    midi_notes.erase(midi_notes.begin() + i);

                    //remove every entry in the ith column
                    int num_nodes_before_deleting = adj_matrix_under_construction.size();
                    for (int j = 0; j < num_nodes_before_deleting; j++) {
                        adj_matrix_under_construction[j].erase(adj_matrix_under_construction[j].begin() + i);
                    }
                    //then remove the ith row
                    adj_matrix_under_construction.erase(adj_matrix_under_construction.begin() + i);

                    i = 0;
                }
                else {
                    i++;
                }
            }
        }
        else if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered() && add_edge_mode) {
            //CHECK IF TOUCHING CIRCLE NODE AND TAKE NOTE OF WHICH NODE
            if (!ready_for_line_end_coordinate) {
                //check if touching a node circle and get index of node circle
                for (int i = 0; i < circles.size(); i++) {
                    if (mouse_in_circle(circles[i], circle_radius, io.MousePos.x, io.MousePos.y)) {
                        ImGui::Text("touching node %d", i);
                        line_start_node_index = i; //save edge start index
                        //use the center of the start node as the start position of the line
                        line_start = { (float)circles[line_start_node_index].x, (float)circles[line_start_node_index].y };
                        ready_for_line_end_coordinate = true;
                    }
                }
            }
            else {
                //check if touching a node circle and get index of node circle
                for (int i = 0; i < circles.size(); i++) {
                    //also check that it is not the same node circle as the line_start
                    if (i == line_start_node_index) {
                        continue;
                    }
                    if (mouse_in_circle(circles[i], circle_radius, io.MousePos.x, io.MousePos.y)) {
                        ImGui::Text("touching circle %d", i);
                        line_end_node_index = i; //save edge end index
                        //CHECK IF THERE IS ALREADY A CONNECTION BETWEEN THE 2 NODES, THEN SKIP ADDING ONE
                        if (adj_matrix_under_construction[line_start_node_index][line_end_node_index] == 1) {
                            ready_for_line_end_coordinate = false; //reset to look for start node next
                            break;
                        }
                        line_end = { (float)circles[line_end_node_index].x, (float)circles[line_end_node_index].y };
                        have_both_line_coordinates = true;

                        break;
                    }
                }

                if (have_both_line_coordinates) {
                    ready_for_line_end_coordinate = false;
                    line_coordinates ln;
                    ln.x_1 = line_start.x;
                    ln.y_1 = line_start.y;
                    ln.x_2 = line_end.x;
                    ln.y_2 = line_end.y;
                    lines.push_back(ln);
                    //add edge to adjacency matrix
                    if (undirected_graph) {
                        adj_matrix_under_construction[line_start_node_index][line_end_node_index] = 1;
                        adj_matrix_under_construction[line_end_node_index][line_start_node_index] = 1;
                    }
                    else {
                        adj_matrix_under_construction[line_start_node_index][line_end_node_index] = 1;
                    }

                    have_both_line_coordinates = false;
                }
            }
        }
        else if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered() && remove_edge_mode) {
            //remove edge 
            //user will click each of the 2 nodes that the edge connects
            //so basically the same logic as when you added an edge but remove edge instead
            //CHECK IF TOUCHING CIRCLE NODE AND TAKE NOTE OF WHICH NODE
            if (!ready_for_line_end_coordinate) {
                //check if touching a node circle and get index of node circle
                for (int i = 0; i < circles.size(); i++) {
                    if (mouse_in_circle(circles[i], circle_radius, io.MousePos.x, io.MousePos.y)) {
                        ImGui::Text("touching circle %d", i);
                        line_start_node_index = i; //save edge start index
                        //use the center of the start node as the start position of the line
                        line_start = { (float)circles[line_start_node_index].x, (float)circles[line_start_node_index].y };
                        ready_for_line_end_coordinate = true;
                    }
                }
            }
            else {
                //check if touching a node circle and get index of node circle
                for (int i = 0; i < circles.size(); i++) {
                    //also check that it is not the same node circle as the line_start
                    if (i == line_start_node_index) {
                        continue;
                    }
                    if (mouse_in_circle(circles[i], circle_radius, io.MousePos.x, io.MousePos.y)) {
                        ImGui::Text("touching circle %d", i);

                        line_end_node_index = i; //save edge end index
                        line_end = { (float)circles[line_end_node_index].x, (float)circles[line_end_node_index].y };
                        have_both_line_coordinates = true;

                        break;
                    }
                }

                if (have_both_line_coordinates) {
                    ready_for_line_end_coordinate = false;
                    line_coordinates ln;
                    ln.x_1 = line_start.x;
                    ln.y_1 = line_start.y;
                    ln.x_2 = line_end.x;
                    ln.y_2 = line_end.y;

                    //FIND LINE AND ERASE
                    for (int i = 0; i < lines.size(); i++) {
                        if (ln.x_1 == lines[i].x_1 && ln.y_1 == lines[i].y_1 && ln.x_2 == lines[i].x_2 && ln.y_2 == lines[i].y_2) {
                            lines.erase(lines.begin() + i);
                            break;
                        }
                        else if (ln.x_2 == lines[i].x_1 && ln.y_2 == lines[i].y_1 && ln.x_1 == lines[i].x_2 && ln.y_1 == lines[i].y_2) {
                            if (undirected_graph) {
                                lines.erase(lines.begin() + i);
                            }
                            break;
                        }
                    }

                    //remove edge from adjacency matrix
                    adj_matrix_under_construction[line_start_node_index][line_end_node_index] = 0;
                    if (undirected_graph) {
                        adj_matrix_under_construction[line_end_node_index][line_start_node_index] = 0;
                    }
                    have_both_line_coordinates = false;
                }
            }
        }

        bool mouse_is_touching_a_circle = false;
        for (int i = 0; i < circles.size(); i++) { //draw node circles
            //looping through 6 colors that correspond to the midi note input box colors
            draw->AddCircle(ImVec2(circles[i].x, circles[i].y), circle_radius, circle_midi_note_colors[i % circle_midi_note_colors.size()], 100, 1.0f);
            //checking if mouse is inside of a previously existing circle
            if (mouse_in_circle(circles[i], circle_radius, io.MousePos.x, io.MousePos.y)) {
                ImGui::Text("touching node %d", i);
                mouse_is_touching_a_circle = true;
            }
        }
        if (!mouse_is_touching_a_circle) {
            ImGui::Text("not touching any node");
        }

        for (int i = 0; i < lines.size(); i++) { //draw edge lines
            //ADD LINES THAT TOUCH THE NODE CIRCLE TO REPRESENT THE DIRECTIONAL ARROW IF DIRECTED GRAPH
            if (lines[i].x_2 > lines[i].x_1) {
                //calculating a perpendicular vector {x, y} perp {y, -x} for reference
                double x_dist = lines[i].x_2 - lines[i].x_1;
                if (x_dist == 0) { //can't have 0 distance or arrows don't work out
                    x_dist = -1; //needs to be -1 so that the arrows end up on the correct side of the circle
                }
                //y distance is negative if going up because of the way that y is counted (increasing going down on the screen)
                double y_dist = lines[i].y_2 - lines[i].y_1;

                if (y_dist == 0) { //can't have 0 distance or arrows don't work out
                    y_dist = 1;
                }
                double slope = y_dist / x_dist;

                double perp_line_start_x = lines[i].x_2;
                double perp_line_start_y = lines[i].y_2;
                double perp_line_end_x = lines[i].x_2 + y_dist;
                double perp_line_end_y = lines[i].y_2 - x_dist;
                ImVec2 perp_line_start = { (float)perp_line_start_x, (float)perp_line_start_y };
                ImVec2 perp_line_end = { (float)perp_line_end_x, (float)perp_line_end_y };

                double moved_down_amt = sqrt(pow(circle_radius, 2) * (1 / ((pow(slope, 2)) + 1)));
                double perp_line_start_moved_down_some_x = lines[i].x_2 - moved_down_amt;
                double perp_line_start_moved_down_some_y = lines[i].y_2 - (moved_down_amt * slope); 
                ImVec2 perp_line_start_moved_down_some = { (float)perp_line_start_moved_down_some_x, (float)perp_line_start_moved_down_some_y };
                double perp_line_end_moved_down_some_x = lines[i].x_2 + y_dist - moved_down_amt;
                double perp_line_end_moved_down_some_y = lines[i].y_2 - x_dist - (moved_down_amt * slope); 
                ImVec2 perp_line_end_moved_down_some = { (float)perp_line_end_moved_down_some_x, (float)perp_line_end_moved_down_some_y };

                //MOVE IT DOWN FURTHER TO BE ABLE TO DRAW THE ARROW TO TOUCH THE EDGE OF THE CIRCLE
                //also need to include the moved down amount from the original perpendicular vector
                //THIS WORKS FOR DRAWING A PERPENDICULAR LINE AT THE BOTTOM OF THE CIRCLE THE WIDTH OF THE CIRCLE
                double moved_down_perp_line_amt = sqrt(pow(circle_radius, 2) * (1 / ((pow((-1 / slope), 2)) + 1)));
                double perp_line_start_moved_down_some_both_directions_x = lines[i].x_2 + moved_down_perp_line_amt - (2 * moved_down_amt);
                double perp_line_start_moved_down_some_both_directions_y = lines[i].y_2 + (moved_down_perp_line_amt * (-1 / slope)) - ((2 * moved_down_amt) * slope);
                ImVec2 perp_line_start_moved_down_some_both_directions = { (float)perp_line_start_moved_down_some_both_directions_x, (float)perp_line_start_moved_down_some_both_directions_y };
                double perp_line_end_moved_down_some_both_directions_x = lines[i].x_2 - moved_down_perp_line_amt - (2 * moved_down_amt);
                double perp_line_end_moved_down_some_both_directions_y = lines[i].y_2 - (moved_down_perp_line_amt * (-1 / slope)) - ((2 * moved_down_amt) * slope);
                ImVec2 perp_line_end_moved_down_some_both_directions = { (float)perp_line_end_moved_down_some_both_directions_x, (float)perp_line_end_moved_down_some_both_directions_y };
                if (!undirected_graph) {
                    draw->AddLine(perp_line_start_moved_down_some_both_directions, perp_line_end_moved_down_some_both_directions, IM_COL32(0, 0, 255, 255));
                }
                //SO NOW THAT YOU HAVE THE 3 POINTS THAT YOU NEED TO DRAW THE ARROW, DRAW THE 2 LINES
                if (!undirected_graph) {
                    draw->AddLine(perp_line_start_moved_down_some_both_directions, perp_line_start_moved_down_some, IM_COL32(0, 0, 255, 255));
                    draw->AddLine(perp_line_end_moved_down_some_both_directions, perp_line_start_moved_down_some, IM_COL32(0, 0, 255, 255));
                }
                //HAVE THE EDGE LINES TERMINATE AT THE EDGE OF THE CIRCLE 
                ImVec2 line_start = { (float)(lines[i].x_1 + moved_down_amt), (float)(lines[i].y_1 + (moved_down_amt * slope)) };
                ImVec2 line_end = { (float)(lines[i].x_2 - moved_down_amt), (float)(lines[i].y_2 - (moved_down_amt * slope)) };
                draw->AddLine(line_start, line_end, IM_COL32(0, 255, 0, 255));

            }
            //ADJUST DIRECTION FOR WHEN ARROW IS ON THE OTHER SIDE OF THE CIRCLE WHEN THE EDGE GOES FROM THE RIGHT TO THE LEFT
            //reverse every + and - sign since oriented in the opposite x direction
            else {
                //calculating a perpendicular vector {x, y} perp {y, -x} for reference
                double x_dist = lines[i].x_2 - lines[i].x_1;
                if (x_dist == 0) { //can't have 0 distance or arrows don't work out 
                    x_dist = -1; //needs to be -1 so that the arrows end up on the correct side of the circle
                }

                //y distance is negative if going up because of the way that y is counted (increasing going down on the screen)
                double y_dist = lines[i].y_2 - lines[i].y_1;
                if (y_dist == 0) { //can't have 0 distance or arrows don't work out
                    y_dist = 1;
                }
                double slope = y_dist / x_dist;
                double perp_line_start_x = lines[i].x_2;
                double perp_line_start_y = lines[i].y_2;
                double perp_line_end_x = lines[i].x_2 - y_dist;
                double perp_line_end_y = lines[i].y_2 + x_dist;
                ImVec2 perp_line_start = { (float)perp_line_start_x, (float)perp_line_start_y };
                ImVec2 perp_line_end = { (float)perp_line_end_x, (float)perp_line_end_y };

                double moved_down_amt = sqrt(pow(circle_radius, 2) * (1 / ((pow(slope, 2)) + 1)));
                double perp_line_start_moved_down_some_x = lines[i].x_2 + moved_down_amt;
                double perp_line_start_moved_down_some_y = lines[i].y_2 + (moved_down_amt * slope); //wait maybe it's - after all
                ImVec2 perp_line_start_moved_down_some = { (float)perp_line_start_moved_down_some_x, (float)perp_line_start_moved_down_some_y };
                double perp_line_end_moved_down_some_x = lines[i].x_2 - y_dist + moved_down_amt;
                double perp_line_end_moved_down_some_y = lines[i].y_2 + x_dist + (moved_down_amt * slope); //wait maybe it's - after all
                ImVec2 perp_line_end_moved_down_some = { (float)perp_line_end_moved_down_some_x, (float)perp_line_end_moved_down_some_y };

                //MOVE IT DOWN FURTHER TO BE ABLE TO DRAW THE ARROW TO TOUCH THE EDGE OF THE CIRCLE
                //also need to include the moved down amount from the original perpendicular vector
                //THIS WORKS FOR DRAWING A PERPENDICULAR LINE AT THE BOTTOM OF THE CIRCLE THE WIDTH OF THE CIRCLE!
                double moved_down_perp_line_amt = sqrt(pow(circle_radius, 2) * (1 / ((pow((-1 / slope), 2)) + 1)));
                double perp_line_start_moved_down_some_both_directions_x = lines[i].x_2 - moved_down_perp_line_amt + (2 * moved_down_amt);
                double perp_line_start_moved_down_some_both_directions_y = lines[i].y_2 - (moved_down_perp_line_amt * (-1 / slope)) + ((2 * moved_down_amt) * slope);
                ImVec2 perp_line_start_moved_down_some_both_directions = { (float)perp_line_start_moved_down_some_both_directions_x, (float)perp_line_start_moved_down_some_both_directions_y };
                double perp_line_end_moved_down_some_both_directions_x = lines[i].x_2 + moved_down_perp_line_amt + (2 * moved_down_amt);
                double perp_line_end_moved_down_some_both_directions_y = lines[i].y_2 + (moved_down_perp_line_amt * (-1 / slope)) + ((2 * moved_down_amt) * slope);
                ImVec2 perp_line_end_moved_down_some_both_directions = { (float)perp_line_end_moved_down_some_both_directions_x, (float)perp_line_end_moved_down_some_both_directions_y };
                if (!undirected_graph) {
                    draw->AddLine(perp_line_start_moved_down_some_both_directions, perp_line_end_moved_down_some_both_directions, IM_COL32(0, 0, 255, 255));
                }
                //SO NOW THAT YOU HAVE THE 3 POINTS THAT YOU NEED TO DRAW THE ARROW, DRAW THE 2 LINES
                if (!undirected_graph) {
                    draw->AddLine(perp_line_start_moved_down_some_both_directions, perp_line_start_moved_down_some, IM_COL32(0, 0, 255, 255));
                    draw->AddLine(perp_line_end_moved_down_some_both_directions, perp_line_start_moved_down_some, IM_COL32(0, 0, 255, 255));
                }
                //HAVE THE EDGE LINES TERMINATE AT THE EDGE OF THE CIRCLE 
                ImVec2 line_start = { (float)(lines[i].x_1 - moved_down_amt), (float)(lines[i].y_1 - (moved_down_amt * slope)) };
                ImVec2 line_end = { (float)(lines[i].x_2 + moved_down_amt), (float)(lines[i].y_2 + (moved_down_amt * slope)) };
                draw->AddLine(line_start, line_end, IM_COL32(0, 255, 0, 255));
            }
        }

        ImGui::Text("\n");

        //button to show instruction window
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 0, 0, 255));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));

        if (ImGui::Button("SHOW TUTORIAL")) {
            show_another_window = true;
        }

        ImGui::PopStyleColor(2);

        ImGui::PopItemWidth();

        ImGui::End();

        if (show_another_window) {
            ImGui::Begin("How To Use", &show_another_window);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            ImGui::TextWrapped("GRAPH BUILDING OPTIONS to select");
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::TextWrapped("NO DRAW");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::TextWrapped("no node or edge is added/removed");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::TextWrapped("ADD NODE");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::TextWrapped("place node at position of mouse click on screen");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::TextWrapped("ADD EDGE");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::TextWrapped("click \"originating node\" of edge that you wish to add from graph, then click \"end of arrow\" node to add edge from \"originating node\" to \"end of arrow\" node");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::TextWrapped("REMOVE NODE");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::TextWrapped("remove node at position of mouse click and all edges connected");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::TextWrapped("REMOVE EDGE");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::TextWrapped("click \"originating node\" of edge that you wish to remove from graph, then click \"end of arrow\" node to remove edge");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            ImGui::TextWrapped("\nDIRECTED/UNDIRECTED GRAPH");
            ImGui::PopStyleColor();
            ImGui::TextWrapped("Select undirected if you want to add an edge in both directions when adding an edge");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            ImGui::TextWrapped("\nSTART NODE");
            ImGui::PopStyleColor();
            ImGui::TextWrapped("Specify the start node (index of node to begin traversing graph from)");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            ImGui::TextWrapped("\nmidi note for node _");
            ImGui::PopStyleColor();
            ImGui::TextWrapped("Can specify a midi note for each individual node or defaults to node index number, for instance music note middle C is midi note # 60");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            ImGui::TextWrapped("\nGENERATE SEQUENCE");
            ImGui::PopStyleColor();
            ImGui::TextWrapped("Finds all possible paths through graph. Lists each path separately and then concatenates into ALL POSSIBLE PATHS ALTOGETHER box to copy");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            ImGui::TextWrapped("\n# of notes in all possible paths altogether");
            ImGui::PopStyleColor();
            ImGui::TextWrapped("Displays # of notes when putting together all of the individual possible graph traversal sequences");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            ImGui::TextWrapped("\nfactors of # of notes");
            ImGui::PopStyleColor();
            ImGui::TextWrapped("Just displaying the factors of the # of notes in the ALL POSSIBLE PATHS ALTOGETHER sequence for reference");
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            ImGui::TextWrapped("\nAdjacency Matrix");
            ImGui::PopStyleColor();
            ImGui::TextWrapped("Displays matrix representation of graph. For each row, the column # represents a node able to be traveled to from the row node");

            ImGui::Text("\n");
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 255, 0, 255));
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
            if (ImGui::Button("CLOSE TUTORIAL")) {
                show_another_window = false;
            }
            ImGui::PopStyleColor(2);

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    	
        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
