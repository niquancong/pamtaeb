#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "Pamtaeb.h"

int select_path(char* result){
	// code modified from cplusplus.com user Duthomhas at
	// http://www.cplusplus.com/forum/windows/169960/

	// opens up file explorer and lets user select a file

	char filename[ MAX_PATH ];

	OPENFILENAME ofn;
	ZeroMemory( &filename, sizeof( filename ) );
	ZeroMemory( &ofn,      sizeof( ofn ) );
	ofn.lStructSize  = sizeof( ofn );
	ofn.hwndOwner    = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter  = "Beatmap File\0*.osu\0Any File\0*.*\0";
	ofn.lpstrFile    = filename;
	ofn.nMaxFile     = MAX_PATH;
	ofn.lpstrTitle   = "Open";
	ofn.Flags        = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (!GetOpenFileNameA( &ofn ))
	{
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE   : std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE  : std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION  : std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE  : std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE  : std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE  : std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE : std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE  : std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE     : std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK          : std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE      : std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE      : std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL  : std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME : std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE : std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default                    : std::cout << "You cancelled.\n";
		}
		exit(-1);
	}
	memcpy(result, filename, MAX_PATH);
	return 0;
}

int verify_extension(char* path, const char* extension){
	// verifies if path ends with a certain extension

	int status = 0;
	std::string path_string(path);
	std::string extension_string(extension);

	// search for the extension last n chars in path_string
	// where n is the number of chars in extension
	if(path_string.find(extension, path_string.length() - extension_string.length()) == std::string::npos){
		status = -1;
	}
	return status;
}

int verify_input(char* path, std::ifstream* file, const char* extension){
	// sanity check for file path input

	int status = 0;

	if(!(*file).is_open()){
		status = -1;
	}

	if(verify_extension(path, extension) != 0){
		status = -1;
	}

	return status;
}

int reverse_filename(char* result, char* path){
	// reverses filename from a path, the extension is preserved

	int status = 0;
	std::string path_string(path);

	std::size_t filename_start = path_string.find_last_of("/\\");
	std::size_t filename_end = path_string.find_last_of(".", filename_start);
	std::size_t filename_length;

	if(filename_end == std::string::npos){
		std::cout << "Warning: attempting to reverse a filename with no extension!" << std::endl;
		filename_length = path_string.length() - filename_start;
	}else{
		filename_length = filename_end - filename_start;
	}
	
	path_string.copy(result, filename_start);
	// not done yet but i've lost motivation for today xd

	return status;
}

int main(int argc, char** argv){
	// initializations
	char* map_input_path = new char[MAX_PATH];
	char* map_output_path = new char[MAX_PATH];

	// open .osu file
	std::cout << "Select the .osu file you wish to reverse: " << std::endl;
	select_path(map_input_path);

	// beatmap processing
	std::cout << "Beatmap selected: " << map_input_path << std::endl;
	std::ifstream map_input(map_input_path);
	if(verify_input(map_input_path, &map_input, ".osu") != 0){
		std::cout << "Beatmap selection failed!" << std::endl;
		return -1;
	}

	reverse_filename(map_output_path, map_input_path);
	std::ofstream map_output(map_output_path);
	

	// cleanup
	map_input.close();
	delete[] map_input_path;
	delete[] map_output_path;
	return 0;
}