#include <iostream>
#include <SFML/Graphics.hpp>
#include "SFML/Audio.hpp"
//#include "Object.h"
#include <Windows.h>
#include "Space.h"
#include "Border.h"
#include <ctime>
#include <random>
#include <fstream>

#define DEFAULT_NAME "CONFIG "
#define DEFAULT_TYPE ".dat"
#define VERSION "3.0 - Release"


using namespace sf;
using namespace std;


//Глобальные переменные -------------------------------------------

int XS, YS;

int S_OBJECT = 20000; //(20000) Среднее пространство, для одного объекта

float StartSpeed = 0.08; // (0.08) Стартовая скорость распространения частиц

int GlobalCounterOfConfigs = 0; //Количество конфигураций

float G = 1; //Гравитационная постоянная

float GameSpeed = 5000; //(800)




struct Configuration //Структура конфигурации
{
	vector<Object> obj_vec;
	float st_speed;
	float g;
	string Name;

	Configuration(vector<Object> new_vec, string Name, float st_speed, float g)
	{
		this->obj_vec = new_vec;
		this->st_speed = st_speed;
		this->g = g;
		this->Name = Name;
	}

	Configuration(string Name)
	{
		this->Name = Name;
	}

	void AddObject(Object &obj)
	{
		obj_vec.push_back(obj);
	}

	void FillSpace(Space &space) //Заполняет пространство собой
	{
		space.ClearObjects();

		for (auto var : obj_vec)
		{
			space.AddObject(var);
		}
	}

};

vector<Configuration> Configs; //Вектор конфигураций
vector<Object> Buffer_vect; //Буфер объектов (для возмонжости запуска симуляции заново)






//Прототипы функций -----------------------------------------------

void MainMenu(Space &space);
void AdditionalMenu(Space &space);
void WriteOnBuffer(vector<Object> vect);
void RandomGenerateMolekular(Space &space, int count = 0);
void RandomGenerate(Space &space, int count = 0, float density = 1);
void PrintConfigs();
void PrintObjects(vector<Object> ToPrintVect);
void ReadConfigs();
void ReStart(Space &space);
void Documentation();
void SaveConfig(vector<Object> ToSaveVec, string Name);
void ChangeObject(Object &obj);
Object CreateNewObject();
void ChangeConfig(Space &space, vector<Object> temp_vec, int mode = 0);
void SetConfig(Space &space, int index, int setValues);
void Settings(Space &space);
void AdditionalMenu(Space &space);
void MainMenu(Space &space);



//Реализации функций -----------------------------------------------

void WriteOnBuffer(vector<Object> vect)
{
	Buffer_vect = vect;
}

void RandomGenerateMolekular(Space &space, int count)
{
	vector<Object> random_vec(count);

	int A_SIZE = sqrt(S_OBJECT * count); //Сторона области генерации (квадрат)

	//space.border.X = -A_SIZE / 2;
	//space.border.Y = A_SIZE / 2;
	//space.border.XS = A_SIZE;
	//.border.YS = A_SIZE;

	space.Objects.clear();

	for (auto &var : random_vec)
	{
		var.SetPosition(-A_SIZE / 2 + rand() % (A_SIZE), -A_SIZE / 2 + rand() % (A_SIZE));
		var.SetRadius(6);

		float SpeedX = 0 + rand() % (14);
		float SpeedY = 0 + rand() % (14);

		SpeedX /= 100;
		SpeedY /= 100;

		var.SetSpeed(0, 0);

		var.SetColor2(1);

		space.AddObject(var);
	}

	WriteOnBuffer(random_vec);
}

void RandomGenerate(Space &space, int count, float density)
{
	vector<Object> random_vec(count);
	float angle;
	int color;

	int A_SIZE = sqrt(S_OBJECT * count); //Расчёт стороны области генерации (квадрат)

	space.Objects.clear();

	for (auto &var : random_vec)
	{
		var.SetPosition(-A_SIZE / 2 + rand() % (A_SIZE), -A_SIZE / 2 + rand() % (A_SIZE));
		var.SetRadius(1 + rand() % (5));
		var.SetMass(var.GetRadius() * density);

		angle = atan2(-var.GetYPos(), var.GetXPos()); //Угол относительно начала координат

		var.SetSpeed(cos(angle) * StartSpeed, sin(angle) * StartSpeed);

		color = 0 + rand() % (7);

		var.SetColor2(color);

		space.AddObject(var);
	}

	WriteOnBuffer(random_vec);
}

void PrintConfigs()
{
	system("cls");

	int i = 1;

	cout << "Configurations:\n\n";

	for (auto var : Configs)
	{
		cout << i << ". " << var.Name << "	  Objects: " << var.obj_vec.size() << "	  Start Speed: " << var.st_speed << "	  G: " << var.g << "\n\n";
		i++;
	}
}

void PrintObjects(vector<Object> ToPrintVect)
{
	cout << "Objects: \n\n";
	int i = 1;

	for (auto var : ToPrintVect)
	{
		cout << i << " - " << var.GetName() << "	Mass: " << var.GetMass() << "	Radius: " << var.GetRadius() << "	Position: (" << var.GetXPos() << " ; " << var.GetYPos() << ")	Speed: (" << var.GetXSpeed() << " ; " << var.GetYSpeed() << ")\n\n";
		i++;
	}

	cout << "COUNT: " << ToPrintVect.size() << "\n\n";
}

void ReadConfigs()
{
	string str = DEFAULT_NAME + to_string(Configs.size() + 1);
	str += DEFAULT_TYPE;

	ifstream Reader;

	vector<Object> temp_vec; //Вектор, в который будет вестись запись прочитанного
	float x, y; //Переменные записи
	float g, start_speed;
	string NameConf;


	char Buffer[60]; //Буфер прочтённых символов

	while (true)
	{
		str = DEFAULT_NAME + to_string(Configs.size() + 1);
		str += DEFAULT_TYPE; //Название искомого файла

		Reader.open(str);
		if (!Reader.is_open())
			break;

		else
		{
			Reader.getline(Buffer, 60);
			NameConf = Buffer;

			Reader.getline(Buffer, 60);
			temp_vec.resize(atoi(Buffer));

			for (auto &var : temp_vec)
			{
				Reader.getline(Buffer, 60);
				var.SetName(Buffer);

				Reader.getline(Buffer, 60);
				var.SetMass(stof(Buffer));

				Reader.getline(Buffer, 60);
				var.SetRadius(stof(Buffer));

				Reader.getline(Buffer, 60);
				x = stof(Buffer);

				Reader.getline(Buffer, 60);
				y = stof(Buffer);

				var.SetPosition(x, y);

				Reader.getline(Buffer, 60);
				x = stof(Buffer);

				Reader.getline(Buffer, 60);
				y = stof(Buffer);

				var.SetSpeed(x, y);

				Reader.getline(Buffer, 60);
				var.SetColor2(atoi(Buffer));

				Reader.getline(Buffer, 60);
				start_speed = stof(Buffer);

				Reader.getline(Buffer, 60);
				g = stof(Buffer);

			}

			Configs.push_back(Configuration(temp_vec, NameConf, start_speed, g));

			temp_vec.clear();
		}

		Reader.close();
	}

}

void ReStart(Space &space)
{
	space.Objects = Buffer_vect;
}

void Documentation()
{
	int ch;
	system("cls");
	cout << "Navigation: \n\nYou can use to choose or input: numbers and <<.>>.\nAfter you have to press <<Enter>>\n\n";
	cout << "Simulation Control: \n\nYou can move camera with WASD, also you can zoom camera with mouse-scroll\n\n";
	cout << "Hot-Keys (During Simualation only):\n\nEsc - Call Main Menu\nTab - Call Additional Menu\nT - Restart your config\nO - on/off orbits mode\nSpace - on/off pause\n\n";
	cout << "Press any button to exit to the main menu: ";
	cin >> ch;
}

void SaveConfig(vector<Object> ToSaveVec, string Name)
{
	Configs.push_back(Configuration(ToSaveVec, Name, StartSpeed, G));

	ofstream Dataf;

	string str = DEFAULT_NAME + to_string(Configs.size() - 1);
	str += DEFAULT_TYPE;

	Dataf.open(str);

	Dataf << Name << endl;

	Dataf << ToSaveVec.size() << endl;

	for (auto var : ToSaveVec)
	{
		Dataf << var.GetName() << endl;
		Dataf << var.GetMass() << endl;
		Dataf << var.GetRadius() << endl;
		Dataf << var.GetXPos() << endl;
		Dataf << var.GetYPos() << endl;
		Dataf << var.GetXSpeed() << endl;
		Dataf << var.GetYSpeed() << endl;
		Dataf << var.GetColor() << endl;
		Dataf << StartSpeed << endl;
		Dataf << G << endl;
	}

	Dataf.close();

}

void ChangeObject(Object &obj)
{
	system("cls");
	int ch;
	string name;
	char nm;
	float mass;
	float x, y, x_speed, y_speed;
	float radius;

	cout << "Name: " << obj.GetName() << "\n\nMass: " << obj.GetMass() << "\n\nRadius: " << obj.GetRadius() << "\n\nPosition: (" << obj.GetXPos() << " ; " << obj.GetYPos() << ")\n\nSpeed: (" << obj.GetXSpeed() << " ; " << obj.GetYSpeed() << ")\n\n\n";

	cout << "\n\n1 - Change Name\n2 - Change Mass\n3 - Change Radius\n4 - Change Position\n5 - Change Speed\n\n0 - Exit\n\n";

	cin >> ch;

	switch (ch)
	{
	case 1:
		cout << "Input New Name: ";
		cin >> nm;
		name = nm;
		obj.SetName(name);
		ChangeObject(obj);
		break;

	case 2:
		cout << "Input New Mass: ";
		cin >> mass;
		obj.SetMass(mass);
		ChangeObject(obj);
		break;

	case 3:
		cout << "Input New Radius: ";
		cin >> radius;
		obj.SetRadius(radius);
		ChangeObject(obj);
		break;

	case 4:
		cout << "\n\nNew Position:\nX: ";
		cin >> x;
		cout << "\nY: ";
		cin >> y;
		obj.SetPosition(x, y);
		ChangeObject(obj);
		break;

	case 5:
		cout << "\n\nNew Speed:\nX: ";
		cin >> x_speed;
		cout << "\nY: ";
		cin >> y_speed;
		obj.SetSpeed(x_speed, y_speed);
		ChangeObject(obj);
		break;

	default:

		break;
	}
}

Object CreateNewObject()
{
	system("cls");
	Object result;
	string name;
	char nm[50];
	float mass;
	float x, y, x_speed, y_speed;
	float radius;
	int color;

	cout << "Input name: ";
	cin >> nm;
	name = nm;

	cout << "\n\nMass: ";
	cin >> mass;
	cout << "\n\nRadius: ";
	cin >> radius;
	cout << "\n\nPosition:\nX: ";
	cin >> x;
	cout << "\nY: ";
	cin >> y;
	cout << "\n\nSpeed:\nX: ";
	cin >> x_speed;
	cout << "\nY: ";
	cin >> y_speed;
	cout << "\n\nChoose Color:\n\n1 - Red\n2 - Green\n3 - Blue\n4 - White\n5 - Yellow\n6 - Cyan\n7 - Magenta\n\n";
	cin >> color;


	result.SetName(name);
	result.SetMass(mass);
	result.SetRadius(radius);
	result.SetPosition(x, y);
	result.SetSpeed(x_speed, -y_speed);
	result.SetColor2(color);

	return result;

}

void ChangeConfig(Space &space, vector<Object> temp_vec, int mode)
{
	int ch;
	size_t index;
	string sv_name;
	char sv_nm;

	system("cls");

	PrintObjects(temp_vec);

	cout << "1 - Add new Object\n\n2 - Delete Object\n\n3 - Change Object\n\n4 - Save Configuration\n\n5 - Start\n\n\n0 - Go Back\n\n";
	cin >> ch;

	switch (ch)
	{
	case 1:
		temp_vec.push_back(CreateNewObject());
		ChangeConfig(space, temp_vec);
		break;

	case 2:
		cout << "Input Index (If you don`t want to delete the object, input 0 or negative number): ";
		cin >> index;
		if (index > 0 && index <= temp_vec.size())
			temp_vec.erase(begin(temp_vec) + index - 1);

		ChangeConfig(space, temp_vec);
		break;

	case 3:
		cout << "Input Index (If you don`t want to change the object, input 0 or any negative number): ";
		cin >> index;
		if (index > 0 && index <= temp_vec.size())
			ChangeObject(temp_vec[index - 1]);

		ChangeConfig(space, temp_vec);

	case 4:
		cout << "Input name for this save: ";
		cin >> sv_nm;
		cin.get();
		sv_name = sv_nm;
		SaveConfig(temp_vec, sv_name);
		ChangeConfig(space, temp_vec, mode);
		break;

	case 5:
		space.Objects = temp_vec;
		WriteOnBuffer(temp_vec);
		break;

	case 0:
		if (mode != 0)
			AdditionalMenu(space);
		else
			MainMenu(space);
		break;

	default:
		ChangeConfig(space, temp_vec, mode);
		break;
	}

}

void SetConfig(Space &space, int index, int setValues)
{
	if (setValues == 1)
	{
		G = Configs[index].g;
		StartSpeed = Configs[index].st_speed;
	}

	Configs[index].FillSpace(space);

	WriteOnBuffer(Configs[index].obj_vec);
}

void Settings(Space &space)
{
	system("cls");
	int ch;
	float g;
	float start_speed;

	cout << "Settings: \n\n";
	cout << "G: " << G << "\n\nStart Speed: " << StartSpeed << "\n\n\n";
	cout << "1 - Change Gravity Constant\n2 - Change Start Speed\n3 - Change FreeSpace Of Random Generation\n4 - Return to Main Menu\n";
	cin >> ch;

	switch (ch)
	{
	case 1:
		system("cls");
		cout << "Input new G: ";
		cin >> g;
		G = g;
		Settings(space);
		break;

	case 2:
		system("cls");
		cout << "Input new start speed: ";
		cin >> start_speed;
		StartSpeed = start_speed;
		Settings(space);
		break;

	case 3:
		system("cls");
		cout << "Input new FreeSpace Of Random Generation (Default Value is 20): ";
		cin >> S_OBJECT;
		S_OBJECT *= 1000;
		Settings(space);
		break;

	default:
		MainMenu(space);
		break;
	}
}

void AdditionalMenu(Space &space)
{
	int ch;
	string sv_name;
	char sv_nm[50];
	system("cls");
	cout << "Additonal Menu:\n\n1 - Continue\n\n2 - Change Current status of this Configuration\n\n3 - Save current status of this Configuration\n\n4 - Save start status of this Configuration\n\n5 - Settings\n\n";
	cin >> ch;

	switch (ch)
	{
	case 2:
		ChangeConfig(space, space.Objects, 1);
		AdditionalMenu(space);
		break;

	case 3:
		cout << "Input name for this save: ";
		cin >> sv_nm;
		sv_name = sv_nm;
		SaveConfig(space.Objects, sv_name);
		AdditionalMenu(space);
		break;

	case 4:
		cout << "Input name for this save: ";
		cin >> sv_nm;
		sv_name = sv_nm;
		SaveConfig(Buffer_vect, sv_name);
		AdditionalMenu(space);
		break;

	case 5:
		Settings(space);
		AdditionalMenu(space);
		break;

	default:

		break;
	}

	system("cls");

}

void MainMenu(Space &space)
{
	int ch;
	vector<Object> j;

	system("cls");


	cout << "THE G - Simulator\nversion: " << VERSION << "\n\n";

	cout << "You can: \n\n1 - Random Generate\n\n2 - Load configuration\n\n3 - Create new configuration\n\n4 - Settings\n\n5 - Documentation\n\n";

	cin >> ch;


	switch (ch)
	{
	case 1:
		int count;
		float dencity;
		system("cls");
		cout << "Input Object Count: ";
		cin >> count;
		cout << "\n\nInput Dencity: ";
		cin >> dencity;

		RandomGenerate(space, count, dencity);
		break;

	case 2:
		PrintConfigs();
		int isUpgrade;
		size_t index;
		cout << "\n\nInput Index (If you don`t want to change the object, input 0 or any negative number): ";
		cin >> index;
		if (index > 0 && index < Configs.size() + 1)
		{
			cout << "\n\n";
			cout << "Are you want to set environment values?\n1 - Yes\n2 - No\n\n";
			cin >> isUpgrade;
			index--;
			SetConfig(space, index, isUpgrade);
		}
		else
		{
			MainMenu(space);
		}

		break;

	case 3:

		ChangeConfig(space, j);
		break;

	case 4:
		Settings(space);
		break;

	case 5:
		Documentation();
		MainMenu(space);
		break;

	default:
		cout << "\n\nIncorrect Input!";
		MainMenu(space);
		break;
	}

	system("cls");

}





int main()
{
	XS = GetSystemMetrics(SM_CXSCREEN);
	YS = GetSystemMetrics(SM_CYSCREEN);

	srand(time(0));

	float camPosX = 0, camPosY = 0; //Переменные управления камерой
	float camSpeed = 0.2;
	float camSizeSpeed = 0.006;


	bool isFirstFrame = true; //Переменные состояния симуляции
	bool isPause = false;
	bool isShowOrbits = false;


	system("color 0a");

	ReadConfigs(); //Чтение сохранённых конфигураций

	RenderWindow window(sf::VideoMode(XS, YS), "G-Simulator 3.0 Release", Style::Default);


	Border border(-100, 100, 200, 200); //Границы (Для молекулярного моделирования)

	Space space(1);

	Object Sun(0, 0, 40, 382.94);
	Sun.SetColor(Color::Yellow);

	Object Mercury(464.5, 0, 5, 0.00055274, 0, -0.907);
	Mercury.SetColor(Color::Cyan);

	Object Venus(867.919, 0, 9, 0.00815, 0, 0.664);
	Venus.SetColor(Color::White);

	Object Earth(1200, 0, 10, 0.01, 0, -0.545);
	Earth.SetColor(Color::Blue);

	Object Moon(1220.0843, 0, 1, 0.000123, 0, -0.5897);
	Moon.SetColor(Color::White);

	Object Mars(1828, 0, 10, 0.01, 0, -0.457);
	Mars.SetColor(Color::Red);

	Object Jupiter(6244, 0, 13.21, 3.17, 0, -0.247);
	Jupiter.SetColor(Color::White);



	Configuration SolarSystem("Solar System"); //Конфигурация солнечной системы (встроенная)

	SolarSystem.AddObject(Sun);
	SolarSystem.AddObject(Earth);
	SolarSystem.AddObject(Mercury);
	SolarSystem.AddObject(Venus);
	SolarSystem.AddObject(Mars);
	SolarSystem.AddObject(Jupiter);
	// SolarSystem.AddObject(Moon);


	Configs.push_back(SolarSystem);


	View view; //SFML окружение
	view.reset(sf::FloatRect(0, 0, XS, YS));
	view.setCenter(camPosX, camPosY);
	Clock clock;


	MainMenu(space);


	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / GameSpeed;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		//Обработка всех нажатий
		{
			//Управление камерой
			{
				if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
				{
					camPosX += camSpeed;
					view.move(camSpeed * time, 0);
					isShowOrbits = false;
				}


				if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
				{
					camPosX += -camSpeed;
					view.move(-camSpeed * time, 0);
					isShowOrbits = false;
				}


				if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
				{
					camPosY += -camSpeed;
					view.move(0, -camSpeed * time);
					isShowOrbits = false;
				}


				if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
				{
					camPosY += camSpeed;
					view.move(0, camSpeed  * time);
					isShowOrbits = false;
				}


				if (event.type == sf::Event::MouseWheelScrolled)
				{
					if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
					{
						view.zoom(1 - (event.mouseWheelScroll.delta * camSizeSpeed) * 0.5);
						camSpeed = 1 * (1 - (event.mouseWheelScroll.delta * camSizeSpeed));
						isShowOrbits = false;
					}
				}
			}

			//Состояние симуляции
			{
				if (Keyboard::isKeyPressed(Keyboard::Escape))
				{
					space.Objects.clear();
					MainMenu(space);
					isPause = false;
					isShowOrbits = false;
					clock.restart();
					time = time / 800;
					camPosX = 0, camPosY = 0; //С каждым новым запуском симуляции позиция камеры отходит на начальную
					view.setCenter(camPosX, camPosY);
				}

				if (Keyboard::isKeyPressed(Keyboard::Tab))
				{
					AdditionalMenu(space);
					clock.restart();
					time = time / GameSpeed;
				}

				if (Keyboard::isKeyPressed(Keyboard::T))
				{
					ReStart(space);
					clock.restart();
					time = time / GameSpeed;
				}

				if (Keyboard::isKeyPressed(Keyboard::Space))
				{
					isPause = !isPause;
					this_thread::sleep_for(chrono::milliseconds(200));
					clock.restart();
					time = time / GameSpeed;
				}

				if (Keyboard::isKeyPressed(Keyboard::O))
				{
					isShowOrbits = !isShowOrbits;
					this_thread::sleep_for(chrono::milliseconds(200));
					clock.restart();
					time = time / GameSpeed;
				}
			}
		}

		if (!isPause)
		{
			space.ForceCalc(); //Просчёт движения объектов

			if (!isFirstFrame)
			{
				for (size_t i = 0; i < space.Objects.size(); i++)
				{
					CircleShape cp = space.Objects[i].getObject();

					window.draw(cp);

					if (space.Objects.size() > 8)
						cp.move(space.Objects[i].GetXSpeed() * time, space.Objects[i].GetYSpeed() * time);

					else
						cp.move(space.Objects[i].GetXSpeed(), space.Objects[i].GetYSpeed());


					//Vector2f pos = cp.getPosition();
					//cp.setPosition(pos.x + space.Objects[i].GetXSpeed() * time, pos.y + space.Objects[i].GetYSpeed() * time);
					space.Objects[i].setObject(cp);
				}
			}

			else
			{
				isFirstFrame = false;
			}


			window.display();
			window.setView(view);

			if (!isShowOrbits)
				window.clear();
		}
	}

	return 0;
}
