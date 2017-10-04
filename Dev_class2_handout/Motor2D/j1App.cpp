#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1App.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(scene);

	// render last to swap buffer
	AddModule(render);
}

// Destructor
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	bool ret = true;

	// ==================================================== JSON
	void json_set_allocation_functions(JSON_Malloc_Function malloc_fun, JSON_Free_Function free_fun);
	configuration = json_parse_file("Configuration.json");
	if (configuration == NULL)
	{
		LOG("ERROR LOADING JSON FILE ==================");
		ret = false;
	}
	else
	{
		LOG("LOADING JSON FILE ========================");
		configuration_object = json_value_get_object(configuration);
	}

	p2List_item<j1Module*>* item;
	item = modules.start;
	
	while(item != NULL && ret == true)
	{
		// ==================================================== JSON
		ret = item->data->Awake(json_object_dotget_object(App->configuration_object, item->data->name.GetString()));
		item = item->next;
	}

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// Save & Load -------------------------- 
bool j1App::WantToSave()
{
	bool ret = true;

	if (ret == true)
	{
		Save();
		LOG("File saved correctly");
	}
	else
		LOG("Could not save to file");

	return(ret);
}

bool j1App::WantToLoad()
{
	bool ret = true;

	if (ret == true)
	{
		Load();
		LOG("File loaded correctly");
	}
	else
		LOG("Coud not load from file");

	return(ret);
}

bool j1App::Save()
{
	bool ret = true;

	SaveToFile();

	return(ret);
}

bool j1App::Load()
{
	bool ret = true;

	LoadCameraConfig();

	return(ret);
}

bool j1App::LoadCameraConfig()
{
	bool ret = true;

	// ==================================================== JSON
	void json_set_allocation_functions(JSON_Malloc_Function malloc_fun, JSON_Free_Function free_fun);
	camera_config = json_parse_file("camera.json");
	if (camera_config == NULL)
	{
		LOG("ERROR LOADING JSON FILE ==================");
		ret = false;
	}
	else
	{
		LOG("LOADING JSON FILE ========================");
		camera_object = json_value_get_object(camera_config);
	}

	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		// ==================================================== JSON
		ret = item->data->Load(json_object_dotget_object(App->configuration_object, item->data->name.GetString()));
		item = item->next;
	}

	return ret;
}

bool j1App::SaveToFile()
{
	bool ret = true;

	// ==================================================== JSON
	void json_set_allocation_functions(JSON_Malloc_Function malloc_fun, JSON_Free_Function free_fun);
	

	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		// ==================================================== JSON
		ret = item->data->Save(json_object_dotget_object(App->configuration_object, item->data->name.GetString()));
		item = item->next;
	}

	return ret;
}