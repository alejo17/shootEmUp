#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <dirent.h>
#include <vector>
#include "stream_audio.h"

class Folder_Sound {
public:
	Folder_Sound (const char * dir, std::string type){
		struct dirent *dirpent;
		DIR *dirp;
		dirp= opendir(dir);
		if(dirp){
			while((dirpent = readdir(dirp)) != NULL){
				string aux = dirpent->d_name;
				if(aux.substr(aux.find(".")+1).compare(type)==0)
					names.push_back(dirpent->d_name);
			}
			closedir(dirp);
		}

		for (int i = 0; i < names.size(); ++i){
			sf::SoundBuffer buffer;
			buffer.loadFromFile(string(dir)+ "/" +names[i]);
			sounds.push_back(buffer);
		}
		
	}
	bool play (int pos){
		if (pos <= names.size() && pos >=0){
			stream.load(sounds[pos]);
			stream.play();
			return 1;
		}
		return 0;
	}
	void play_random(){
		srand (time(NULL));
		play(rand() % names.size());
	}

	~Folder_Sound(){;}
private:
	std::vector <sf::SoundBuffer> sounds;
	std::vector <std::string> names;
	MyStream stream;
};