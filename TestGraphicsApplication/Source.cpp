#include "Source.h"
#define WIDTH 1500
#define HEIGHT 1100



void ImGuiFunc() {
	ImGui::Begin("Hello, World!");
	ImGui::Text("Text!");
	//printf("Hello");
	ImGui::End();
}



int main(int argv, char* argc[]) {
	init(WIDTH, HEIGHT, "Default");

	
	//todo
	//1.) create imgui rendering, while giving control to the application
	//2.) bind uniforms to appropriate shader (maybe track the MatrixUniform function to go through and bind them)




	Object* skull = new Object("../Models/Skull/skullLow.dae");
	Object* plane = new Object("../Models/Plane/plane.dae");
	plane->Translate(vec3(0, -.1f, 0));
	plane->Rotate(vec3(1, 0, 0), 180);

	Light* mainLight = new Light(vec3(1,1,1),.25);
	mainLight->translate = TranslateVec(&mainLight->translate, vec3(0, 1, .25));

	Object* isosphere = new Object("../Models/Icosphere.dae");

	Shader* meshShad = new Shader("../Mesh.shader", true);

	vec3* ambientResult = new vec3(mainLight->color * mainLight->intensity);
	meshShad->UniformVector("ambientColor", ambientResult);
	meshShad->UniformVector("lightPos", &mainLight->translate);
	meshShad->UniformVector("lightColor", &mainLight->color);
	Camera* cam = new Camera(vec3(0.0f,0.0f,1.0));
	cam->NewProjection(32, .1f, 100);
	cam->UpdateViewMatrix();

	////Imgui stuff
	//ImGui::CreateContext();

	//ImGuiIO& io = ImGui::GetIO(); (void)io;

	//ImGui::StyleColorsDark();

	//ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
	//ImGui_ImplOpenGL3_Init("#version 140");



	ImGuiDefineFunc(ImGuiFunc);
	while (!ShouldCloseWindow()) {

		float dt = GetDeltaTime();
		ClearWindow();

		//
		//glViewport(0, 0, WIDTH, HEIGHT);
		//glClear(GL_COLOR_BUFFER_BIT);


		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();

		//ImGui::Begin("Hello, World!");
		//ImGui::Text("Text!");
		//ImGui::End();
	
		if (GetKey(keys::A_KEY)) {
			cam->Translate(vec3(cos(-radians(cam->GetY()) + radians(90.0f)) * dt, 0.0f, -sin(-radians(cam->GetY()) + radians(90.0f)) * dt));
		}
		if (GetKey(keys::D_KEY)) {
			cam->Translate(-vec3(cos(-radians(cam->GetY()) + radians(90.0f)) * dt, 0.0f, -sin(-radians(cam->GetY()) + radians(90.0f)) * dt));
		}
		if (GetKey(keys::W_KEY)) {
			cam->Translate(vec3(cos(-radians(cam->GetY())) * dt, 0.0f, -sin(-radians(cam->GetY())) * dt));
		}
		if (GetKey(keys::S_KEY)) {
			cam->Translate(-vec3(cos(-radians(cam->GetY())) * dt, 0.0f, -sin(-radians(cam->GetY())) * dt));
		}
		if (GetKey(keys::CTRL_KEY)) {
			cam->Translate(vec3(0, -1, 0)*dt);
		}

		if (GetKey(keys::SPACE_KEY)) {
			cam->Translate(vec3(0, 1, 0) * dt);
		}
		float x, y;
		GetMousePos(&x, &y);
		if (GetKey(keys::Q_KEY)) {
			SetDisabledMouse(true);
			cam->Rotate(10*-dt * x, vec3(0, 1, 0));
			cam->Rotate(10*-dt * y, cam->GetRightAxis());
		}
		else {
			SetDisabledMouse(false);
		}

		skull->Draw(meshShad, cam);
		plane->Draw(meshShad, cam);

		cam->UpdateViewMatrix();
		
		
		//Error Handleing
		string error = PollError();
		while (error != "Empty") {
			cout << error << "\n";
			error = PollError();
		}
	
		//ImGuiNewFrame();


		ImGuiRender();
		DrawWindow();
		PollEvents();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	return 0;
}