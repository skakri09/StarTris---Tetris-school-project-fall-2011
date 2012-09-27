#include "GameManager.h"

using namespace std;

int main(int argc, char* args[])
{	
	{
		{
			// Creates a pointer to CGameManager
			CGameManager* StarTris;
			try
			{
				cout << "Allocating memory for GameManager..." << endl << endl; 
				
				// Makes StarTris a new CGameManager
				StarTris = new CGameManager();
				// Notifies if the allocation of memory for StarTris was successful
				cout << "...Allocation of memory for GameManager successful." << endl << endl;	
			}
			catch(const bad_alloc& oBadAllocation)
			{
				// Notifies if the allocation was unsuccessful
				cout << "Exception occured: " << oBadAllocation.what() << endl;
			}
			// Initializing StarTris
			StarTris-> Initialize();
			
			// Frees the memory of StarTris if it exists
			if(StarTris)
			{
				delete StarTris;
				StarTris = 0;
			}
		}
		SDL_Quit();
	}
	return 0;
}

