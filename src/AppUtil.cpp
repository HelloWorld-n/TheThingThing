#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <vector>

namespace AppSettings {
	bool help = false;
	bool movement = false;
	void apply(std::vector<std::string> args){
		for (std::string arg: args){
			std::vector<std::string> option_help = {
				"--help",
			};
			if (
				std::any_of(
					std::begin(option_help),
					std::end(option_help), 
					[arg](std::string thing){return arg.compare(thing) == 0;}
				)
			){
				help = true;
			}

			std::vector<std::string> option_movement = {
				"--movement",
				"--move",
			};
			if (
				std::any_of(
					std::begin(option_movement),
					std::end(option_movement), 
					[arg](std::string thing){return arg.compare(thing) == 0;}
				)
			){
				movement = true;
			}			
		}
	}
}
