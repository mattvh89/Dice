#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>

#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0

const std::string LONG_SPACE = "                                                                                                    ";
const std::string HELP_STRING = "Usage: prgname [starting die faces]";
const std::string EXIT_MESSAGE = "Hope you enjoyed your game!";

void mainMenu(int faces = 0);
void roll(int d, int n);

int main(int argc, char** argv)
{
	int startingFaces = 0;
	std::stringstream ss;

	// Check if a valid command line argument was given, which should be a single positive integer
	if (argc > 2)																	// should only take 1 argument so argc == 2 because first argument is progname name
	{
		std::cout << HELP_STRING << std::endl;
		return EXIT_FAILURE;
	}
	if (argc == 2)																	// correct number of arguments, so try to parse it into an integer
	{
		ss << argv[1];
		ss >> startingFaces;

		if (ss.fail() or startingFaces < 2)											// if it's not a valid integer or it's less than 2, print the help string and quit with error code
		{
			std::cout << HELP_STRING << std::endl;
			return EXIT_FAILURE;
		}
	}

	mainMenu(startingFaces);														// Run the actual program

	std::cout << EXIT_MESSAGE << std::endl;

	return EXIT_SUCCESS;
}

void mainMenu(int p_faces)
{
	HANDLE stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(stdOutHandle, &csbi);
	const COORD START_COORD = csbi.dwCursorPosition;
	int dieFaces = 0, 
		numRolls = 0;

	do
	{
		SetConsoleCursorPosition(stdOutHandle, { 0, START_COORD.Y });
		std::cout << LONG_SPACE;
		SetConsoleCursorPosition(stdOutHandle, { 0, START_COORD.Y });

		std::cout << "How many faces? ";											// Prompt for how many faces

		if (p_faces > 1)															// if a valid cl arg was passed we know how many faces
		{																			// and can skip the input, but output the number
			dieFaces = p_faces;														// of dice faces still for continuity
			p_faces = 0;															// set faces = 0 so the user is able to change the face value
			std::cout << dieFaces;
			goto skip_faces_input;													// skip inputting dieFaces
		}

		std::cin >> dieFaces;														// input dieFaces

		if (std::cin.fail())														// validate the in put, just quit if invalid by jumping to the loop condition wihout 
		{																			// modifying the valie of numRolls or dieFaces
			std::cin.ignore();
			std::cin.clear();
			continue;
		}
		if (dieFaces < 2) continue;													// need at least 2 faces so if input is less than that, again quit by continuing to the loop condition
																					// without modifying numRolls
	skip_faces_input:
		do																			// We have a valid number of die faces, stay here while numRolls >= 1
		{
			SetConsoleCursorPosition(stdOutHandle, { 0, (SHORT)(START_COORD.Y + 1) });
			std::cout << LONG_SPACE;
			SetConsoleCursorPosition(stdOutHandle, { 0, (SHORT)(START_COORD.Y + 1) });

			std::cout << "Number of rolls: ";										// prompt for the number of rolls
			std::cin >> numRolls;													// input number of rolls
			if (std::cin.fail())													// validate input, invalid ranges will be handled by roll function
			{
				std::cin.ignore();
				std::cin.clear();
				continue;															// exit by continuing to loop condition without modifying numRolls
			}

			SetConsoleCursorPosition(stdOutHandle, { 0, (SHORT)(START_COORD.Y + 2) });
			std::cout << LONG_SPACE;
			SetConsoleCursorPosition(stdOutHandle, { 0, (SHORT)(START_COORD.Y + 2) });

			roll(dieFaces, numRolls);												// Finally roll the dice
		} while (numRolls > 0);
	} while (dieFaces > 1);
	SetConsoleCursorPosition(stdOutHandle, { 0, (SHORT)(START_COORD.Y + 3) });		// Set cursor for the exit message
}

void roll(int d, int n)
{
	if (d < 2 or n < 1) return;

	std::random_device randomDevice;
	std::mt19937 engine(randomDevice());
	std::uniform_int_distribution<> generator(1, d);
	int* ptrIntArray = new int[n];

	for (int i = 0; i < n; ++i)
		*(ptrIntArray + i) = generator(engine);

	std::qsort((void*)ptrIntArray, n, sizeof(int), 
		[](const void* p_num1, const void* p_num2)
		{
			int num1 = *((const int*)p_num1);
			int num2 = *((const int*)p_num2);
			return (num1 > num2) - (num1 < num2);
		});

	for (int i = 0; i < n; ++i)
		std::cout << *(ptrIntArray + i) << " ";
	std::cout << std::endl;

	delete [] ptrIntArray;
}
