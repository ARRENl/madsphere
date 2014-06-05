#include <windows.h>  //win32 API
#include <d3dx9.h>
#include <vector>
#include "camera.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// CONSTANTS

IDirect3DDevice9* Device = 0; //graphic device(videocard)
ID3DXMesh* Sphere = 0;
ID3DXMesh* Skydome = 0;
ID3DXMesh* Landshaft = 0;

Camera TheCamera(Camera::AIRCRAFT);

std::vector<D3DMATERIAL9> Mtrls(0);
std::vector<IDirect3DTexture9*> Textures(0);
std::vector<D3DMATERIAL9> Mtrls1(0);
std::vector<IDirect3DTexture9*> Textures1(0);
std::vector<D3DMATERIAL9> Mtrls2(0);
std::vector<IDirect3DTexture9*> Textures2(0);

const int Width = 640; //WindowSizes
const int Height = 480;

HWND MainWindowHandle = 0; //main window handle
bool InitWindowsApp(HINSTANCE instanceHandle, int show); //initialization of my window
int Run(); //processing the message loop
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); //The window procedure that processes received our window messages

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// BODY of my project

bool Setup()
{
	HRESULT hr = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	ID3DXBuffer* adjBuffer = 0;
	DWORD numMtrls = 0;
	//SmallFireBall.X - 1 metter fire tex
	hr = D3DXLoadMeshFromX( 
		"SphereWaterBall.X",//filename (0,5 metter water tex)
		D3DXMESH_MANAGED, //flags
		Device,//my d3d device
		&adjBuffer,//Adjacency(смежность)
		&mtrlBuffer,
		NULL,//ignore this
		&numMtrls,//number of materials in mesh
		&Sphere
		);
	if (FAILED(hr))
	{
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}
	if (mtrlBuffer != 0 && numMtrls != 0)//set material from buffer(from .x file)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->
			GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			// При загрузке в свойстве MatD3D не устанавливается 
			// значение для фонового света, поэтому установим его сейчас
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// Сохраняем i-ый материал
			Mtrls.push_back(mtrls[i].MatD3D);

			// Проверяем, связана ли с i-ым материалом текстура
			if (mtrls[i].pTextureFilename != 0)
			{
				// Да, загружаем текстуру для i-ой подгруппы
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					Device,
					mtrls[i].pTextureFilename,
					&tex);

				// Сохраняем загруженную текстуру
				Textures.push_back(tex);
			}
			else
			{
				// Нет текстуры для i-ой подгруппы
				Textures.push_back(0);
			}
		}
	}
	mtrlBuffer->Release();
	/////////////////////////////////////////////////////////////////
	hr = D3DXLoadMeshFromX(
		"SKY_DOME.X",//filename (1000 metter hemisphere skydome)
		D3DXMESH_MANAGED, //flags
		Device,//my d3d device
		&adjBuffer,//Adjacency(смежность)
		&mtrlBuffer,
		NULL,//ignore this
		&numMtrls,//number of materials in mesh
		&Skydome
		);
	if (FAILED(hr))
	{
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}
	if (mtrlBuffer != 0 && numMtrls != 0)//set material from buffer(from .x file)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->
			GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			// При загрузке в свойстве MatD3D не устанавливается 
			// значение для фонового света, поэтому установим его сейчас
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// Сохраняем i-ый материал
			Mtrls1.push_back(mtrls[i].MatD3D);

			// Проверяем, связана ли с i-ым материалом текстура
			if (mtrls[i].pTextureFilename != 0)
			{
				// Да, загружаем текстуру для i-ой подгруппы
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					Device,
					mtrls[i].pTextureFilename,
					&tex);

				// Сохраняем загруженную текстуру
				Textures1.push_back(tex);
			}
			else
			{
				// Нет текстуры для i-ой подгруппы
				Textures1.push_back(0);
			}
		}
	}
	mtrlBuffer->Release();
	///////////////////////////////////////////////////////////////
	hr = D3DXLoadMeshFromX(
		"landshaft.X",//filename (1000 metter landshaft plane)
		D3DXMESH_MANAGED, //flags
		Device,//my d3d device
		&adjBuffer,//Adjacency(смежность)
		&mtrlBuffer,
		NULL,//ignore this
		&numMtrls,//number of materials in mesh
		&Landshaft
		);
	if (FAILED(hr))
	{
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}
	if (mtrlBuffer != 0 && numMtrls != 0)//set material from buffer(from .x file)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->
			GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			// При загрузке в свойстве MatD3D не устанавливается 
			// значение для фонового света, поэтому установим его сейчас
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// Сохраняем i-ый материал
			Mtrls2.push_back(mtrls[i].MatD3D);

			// Проверяем, связана ли с i-ым материалом текстура
			if (mtrls[i].pTextureFilename != 0)
			{
				// Да, загружаем текстуру для i-ой подгруппы
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					Device,
					mtrls[i].pTextureFilename,
					&tex);

				// Сохраняем загруженную текстуру
				Textures2.push_back(tex);
			}
			else
			{
				// Нет текстуры для i-ой подгруппы
				Textures2.push_back(0);
			}
		}
	}
	mtrlBuffer->Release();
	// 
	// Setting texture filter(anisatropic dosent work, dont know why)
	//
	/*Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);*/
	//
	// Setting lighting
	//
	//const D3DXCOLOR      WHITE(D3DCOLOR_XRGB(255, 255, 255));
	D3DXCOLOR Gray (D3DCOLOR_XRGB(128, 128, 128)); //Sphere color
	D3DXCOLOR Black(D3DCOLOR_XRGB(0, 0, 0));
	D3DCOLORVALUE Green { 0.1, 0.6, 0.1 };
	D3DCOLORVALUE Red{ 0.9, 0.1, 0.1 };
	D3DCOLORVALUE White{ 1.0, 1.0, 1.0 };
	D3DCOLORVALUE Blue{ 0.3, 1.0, 0.6 };
	
	D3DLIGHT9 point;                            //Direction of light
	::ZeroMemory(&point, sizeof(point));
	
	//dir.Type = D3DLIGHT_DIRECTIONAL;
	point.Type = D3DLIGHT_POINT;
	point.Diffuse = White;
	point.Specular = 0.9f * White;				//Reflecting
	point.Ambient = 0.1f * White;
	//dir.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	point.Position = D3DXVECTOR3(0.0f, 100.0f, 0.0f);;
	point.Range = sqrt(FLT_MAX);
	point.Falloff = 1.0;
	point.Attenuation0 = 0.1;                   // range of attenuation (need to experiment on this)
	point.Attenuation1 = 0.0;					// range of attenuation
	point.Attenuation2 = 0.0;					// range of attenuation
	
	Device->SetLight(0, &point);
	Device->LightEnable(0, true);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);
	Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);   // Antialiasing on
	
	////
	//// Position and aim the camera in.
	////
	//D3DXVECTOR3 position(0.0f, -2.0f, -2.0f);
	//D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	//D3DXMATRIX V;
	//D3DXMatrixLookAtLH(&V, &position, &target, &up);
	//Device->SetTransform(D3DTS_VIEW, &V);

	//
	// Set projection matrix.
	//
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	//
	// Switch to wireframe mode.
	//

	//Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return true;
}
bool InitD3D(int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** Device)
{
//
// Init D3D: 
//

HRESULT hr = 0;

// Step 1: Create the IDirect3D9 object.

IDirect3D9* d3d9 = 0;
d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

if (!d3d9)
{
	::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
	return false;
}

// Step 2: Check for hardware vp.

D3DCAPS9 caps;
d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

int vp = 0;
if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
else
vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.

D3DPRESENT_PARAMETERS d3dpp;
d3dpp.BackBufferWidth = width;
d3dpp.BackBufferHeight = height;
d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
d3dpp.BackBufferCount = 1;
d3dpp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;  // Antialiasing 8x
d3dpp.MultiSampleQuality = 0;
d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
d3dpp.hDeviceWindow = MainWindowHandle;
d3dpp.Windowed = windowed;
d3dpp.EnableAutoDepthStencil = true;
d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
d3dpp.Flags = 0;
d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

// Step 4: Create the device.

hr = d3d9->CreateDevice(
	D3DADAPTER_DEFAULT, // primary adapter
	deviceType,         // device type
	MainWindowHandle,               // window associated with device
	vp,                 // vertex processing
	&d3dpp,             // present parameters
	Device);            // return created device

if (FAILED(hr))
{
	// try again using a 16-bit depth buffer
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		deviceType,
		MainWindowHandle,
		vp,
		&d3dpp,
		Device);

	if (FAILED(hr))
	{
		d3d9->Release(); // done with d3d9 object
		::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
		return false;
	}
}

d3d9->Release(); // done with d3d9 object

return true;
}
void Cleanup()
{
	Sphere->Release();
	Sphere = 0;
	Device->Release();
	Device = 0;
	/*
	_cosmos->Release();
	_cosmos = 0;
	*/
}

bool Display(float timeDelta)
{
	if (Device)
	{
		
		//
		// Update: Update the camera.
		//

		if (::GetAsyncKeyState('W') & 0x8000f)
			TheCamera.walk(4.0f * timeDelta * 4); //*4 to make moving faster

		if (::GetAsyncKeyState('S') & 0x8000f)
			TheCamera.walk(-4.0f * timeDelta * 4);

		if (::GetAsyncKeyState('A') & 0x8000f)
			TheCamera.strafe(-4.0f * timeDelta * 4);

		if (::GetAsyncKeyState('D') & 0x8000f)
			TheCamera.strafe(4.0f * timeDelta * 4);

		if (::GetAsyncKeyState('R') & 0x8000f)
			TheCamera.fly(4.0f * timeDelta * 4);

		if (::GetAsyncKeyState('F') & 0x8000f)
			TheCamera.fly(-4.0f * timeDelta * 4);

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			TheCamera.pitch(1.0f * timeDelta * 4);

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			TheCamera.pitch(-1.0f * timeDelta * 4);

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			TheCamera.yaw(-1.0f * timeDelta * 4);

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			TheCamera.yaw(1.0f * timeDelta * 4);

		if (::GetAsyncKeyState('N') & 0x8000f)
			TheCamera.roll(1.0f * timeDelta * 4);

		if (::GetAsyncKeyState('M') & 0x8000f)
			TheCamera.roll(-1.0f * timeDelta * 4);

		// Update the view matrix representing the cameras 
		// new position/orientation.
		D3DXMATRIX V;
		TheCamera.getViewMatrix(&V);
		Device->SetTransform(D3DTS_VIEW, &V);
		
		//
		// Draw the Scene:
		//
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff005096, 1.0f, 0);		//background color (blue here) (format: 0xAARRGGBB)
		//Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();
		// Draw Sphere using DrawSubset method with 0 as the argument.
		for (int i = 0; i < Mtrls.size(); i++)
		{
			Device->SetMaterial(&Mtrls[i]);
			Device->SetTexture(0, Textures[i]);
			Sphere->DrawSubset(i);
			
			Device->SetMaterial(&Mtrls1[i]);
			Device->SetTexture(0, Textures1[i]);
			Skydome->DrawSubset(i);

			Device->SetMaterial(&Mtrls2[i]);
			Device->SetTexture(0, Textures2[i]);
			Landshaft->DrawSubset(i);
		}

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd) // main() for windows
{
	if (!InitWindowsApp(hInstance, nShowCmd)) //creating and initialization win application
	{
		::MessageBox(0, "Init - Failed", "Error", MB_OK);
		return 0;
	}
	if (!InitD3D(Width, Height, true, D3DDEVTYPE_HAL, &Device))//creating and initialization D3D Device
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}
	return Run();                                                         //shows and handles events for the game
	Cleanup();																//// release memory after use
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{
	WNDCLASS wc; //window class stucture and his initialization

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName = 0;
	wc.lpszClassName = "Mad Sphere v0.1";

	if (!::RegisterClass(&wc)) //registration of window class
	{
		::MessageBox(0, "RegisterClass - Failed", 0, 0);
		return false;
	}

	MainWindowHandle = ::CreateWindow("Mad Sphere v0.1", "Mad Sphere v0.1", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, instanceHandle, 0); //creating window

	if (MainWindowHandle == 0) //checking window error
	{
		::MessageBox(0, "CreateWindow - Failed", 0, 0);
		return false;
	}

	::ShowWindow(MainWindowHandle, show); //show window
	::UpdateWindow(MainWindowHandle); //update window

	return true;
	
}

int Run()
{
	MSG msg; //windows messages structure
	static float lastTime = (float)timeGetTime();
	while (true)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			::TranslateMessage(&msg); //message translation
			::DispatchMessage(&msg); //sending a message to the relevant procedure
		} else {
			//Game code execution
			// выполнение кода игры
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;
			Display(timeDelta);
			lastTime = currTime;
		}	
	}
	return msg.wParam;
	
}

LRESULT CALLBACK WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) //message processing
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		//::MessageBox(0, "Hello, World", "Hello", MB_OK);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(MainWindowHandle);
		return 0;

	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_MOUSEMOVE:
		TheCamera.OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	return ::DefWindowProc(windowHandle, msg, wParam, lParam); // All other messages go to the system procedure for processing messages.
}