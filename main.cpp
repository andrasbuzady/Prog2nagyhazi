#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <limits>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "Activity.h"
#include "BikeRide.h"
#include "RunActivity.h"
#include "GpsPoint.h"

int getMenuChoice() {
	int valasztas;
	std::string input;
	std::cout << "Opciok:\n";
	std::cout << "1. PrintInfo\n";
	std::cout << "2. Legmagasabb elert szint\n";
	std::cout << "3. Leghosszabb tura\n";
	std::cout << "4. Idoszak alapu kereses \n";
	std::cout << "0. Kilepes\n";
	std::cout << "Adja meg mit szeretne: ";
	while (true) {
		std::getline(std::cin, input);
		std::stringstream ss(input);

		if ((ss >> valasztas) && (ss.eof())) {

			return valasztas;
		}
		else {
			throw std::runtime_error("Hibas valasztas! Csak egesz szamot fogadok el.");
		}
		return valasztas;
	}
}


int main() {
	std::vector<Activity*> activities;

	Activity* a1 = new BikeRide();
	try {
		a1->fajlbeolvas("etyekidombokig.txt");
		activities.push_back(a1);
	}
	catch (const std::exception& e) {
		std::cerr << "Hiba: " << e.what() << std::endl;
	}

	Activity* a2 = new BikeRide();
	try {
		a2->fajlbeolvas("Balatoneszakipart.txt");
		activities.push_back(a2);
	}
	catch (const std::exception& e) {
		std::cerr << "Hiba: " << e.what() << std::endl;
	}

	Activity* a3 = new RunActivity();
	try {
		a3->fajlbeolvas("margitszigetkor.txt");
		activities.push_back(a3);
	}
	catch (const std::exception& e) {
		std::cerr << "Hiba: " << e.what() << std::endl;
	}

	Activity* a4 = new RunActivity();
	try {
		a4->fajlbeolvas("futas.txt");
		activities.push_back(a4);
	}
	catch (const std::exception& e) {
		std::cerr << "Hiba: " << e.what() << std::endl;
	}

	bool running = true;
	while (running) {
		try {
			int choice = getMenuChoice();
			switch (choice) {
			case 1: {
				std::cout << "Az informaciok: \n";
				for (size_t i = 0; i < activities.size(); i++) {
					const Activity* act = activities[i];
					act->printSummary();
					std::cout << "\n\n\n";
				}
				break;
			}
			case 2: {
				std::cout << "A legmagasabb elert szint lett kivalasztva \n";
				try {
					const Activity* highestActivity = activities[0];
					double highestElevation = activities[0]->getHighestElevation();

					for (size_t i = 1; i < activities.size(); i++) {
						double currentElevation = activities[i]->getHighestElevation();
						if (currentElevation > highestElevation) {
							highestElevation = currentElevation;
							highestActivity = activities[i];
						}
					}
					std::cout << "A tura neve: " << highestActivity->getName() << "\n";
					std::cout << "A legmagasabb elert szint: " << highestElevation << " m\n";
					std::cout << "\n\n\n";
				}
				catch (const std::exception& e) {
					std::cerr << "Hiba: " << e.what() << std::endl;
				}
				break;
			}
			case 3: {
				std::cout << "A leghosszabb tura lett kivalasztva \n";
				try {
					const Activity* longestActivity = activities[0];
					double longestDistance = activities[0]->calculateDistance();

					for (size_t i = 1; i < activities.size(); i++) {
						double currentDistance = activities[i]->calculateDistance();
						if (currentDistance > longestDistance) {
							longestDistance = currentDistance;
							longestActivity = activities[i];
						}
					}
					std::cout << "A tura neve: " << longestActivity->getName() << "\n";
					std::cout << std::fixed << std::setprecision(1);
					std::cout << "A leghosszabb tura: " << longestDistance << " km\n";
					std::cout << "\n\n\n";
				}
				catch (const std::exception& e) {
					std::cerr << "Hiba: " << e.what() << std::endl;
				}
				break;
			}
			case 4: {
				std::cout << "Idoszak alapu kereses lett kivalasztva \n";
				std::string year;
				std::cout << "Adja meg az evet: ";
				std::getline(std::cin, year);
				int flag = 0;
				for (size_t i = 0; i < activities.size(); i++) {
					const Activity* act = activities[i];
					if (act->getPoints()[0].timestamp.substr(0, 4) == year) {
						act->printSummary();
						std::cout << "\n";
						flag = 1;
					}
				}
				if (flag == 0) {
					std::cout << "Nincs ilyen evben tura!\n";
				}
				break;
			}
			case 0: {
				std::cout << "Kilepes...\n";
				running = false;
				std::cout << "\n\n\n";
				break;
			}
			default: {
				throw std::runtime_error("Hibas opciot valasztott!");
			}
			}
			std::cout << "=============================\n";
		}
		catch (const std::runtime_error& error) {
			std::cerr << "Hiba: " << error.what() << std::endl;
			std::cout << "=============================\n";
		}
	}

	for (size_t i = 0; i < activities.size(); i++) {
		Activity* act = activities[i];
		delete act;
	}
	activities.clear();
	_CrtDumpMemoryLeaks();

	getchar();

	return 0;
}